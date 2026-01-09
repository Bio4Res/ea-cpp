#include <iostream>
#include <print>
#include <sstream>
#include <fstream>
#include <nlohmann/json.hpp>  //external dependency included here 
#include <thread>
#include <util/enumutils.h>

#ifdef EA_PARALLEL
#include <tbb/global_control.h> //TBB as the backend for paralelism
#endif

#include <fitness/ObjectiveFunction.h>
#include <fitness/BinaryEncodedContinuousObjectiveFunction.h>
#include <problem/discrete/binary/onemax/OneMax.h>
#include <problem/continuous/Rastrigin.h>
#include <problem/continuous/Rosenbrock.h>
#include <problem/continuous/Sphere.h>
#include <problem/continuous/Ackley.h>
#include <problem/continuous/Griewank.h>
#include <problem/continuous/Schaffer2.h>
#include <problem/continuous/Schwefel.h>
#include <problem/continuous/Solomon.h>
#include <problem/continuous/XinSheYang2.h>
#include <statistics/DiversityMeasure.h>
#include <statistics/EntropyDiversity.h>
#include <config/EAConfiguration.h>
#include <base/EvolutionaryAlgorithm.h>
#include <statistics/VarianceDiversity.h>

using json = nlohmann::json;

ENUM(enumObjectiveFunctionType, int, ONEMAX, TRAP, MMDP, LEADING_ONES, TSP, LOP, SPHERE, BIN_SPHERE, ES_ESPHERE, ACKLEY, RASTRIGIN, ROSENBROCK, GRIEWANK, SCHAFFER2, SCHWEFEL, SOLOMON, XINSHEYANG2)

std::unique_ptr<ea::ObjectiveFunction> create(std::string &problem, const int & arg1, const double & arg2 ) {

    size_t found = problem.find('-', 0);
    if (found != std::string::npos)
        problem.replace(found, 1, "_");

    switch (enumObjectiveFunctionType::_from_string_nocase(problem.c_str())) {

    case enumObjectiveFunctionType::ONEMAX:
        return  std::make_unique <ea::OneMax>(arg1);
    case enumObjectiveFunctionType::ACKLEY:
        return std::make_unique <ea::Ackley>(arg1, arg2);
    case enumObjectiveFunctionType::RASTRIGIN:
        return std::make_unique <ea::Rastrigin>(arg1, arg2);
    case enumObjectiveFunctionType::ROSENBROCK:
        return std::make_unique <ea::Rosenbrock>(arg1, arg2);
    case enumObjectiveFunctionType::SPHERE:
        return std::make_unique <ea::Sphere>(arg1, arg2);

    case enumObjectiveFunctionType::GRIEWANK:
        return std::make_unique <ea::Griewank>(arg1, arg2);
    case enumObjectiveFunctionType::SCHAFFER2:
        return std::make_unique <ea::Schaffer2>(arg1, arg2);
    case enumObjectiveFunctionType::SCHWEFEL:
        return std::make_unique <ea::Schwefel>(arg1, arg2);
    case enumObjectiveFunctionType::SOLOMON:
        return std::make_unique <ea::Solomon>(arg1, arg2);
    case enumObjectiveFunctionType::XINSHEYANG2:
        return std::make_unique <ea::XinSheYang2>(arg1, arg2);

        /*
            case enumObjectiveFunctionType::TRAP:
                return new DeceptiveTrap(Integer.parseInt(problem.get(1)), Integer.parseInt(problem.get(2)));

            case enumObjectiveFunctionType::MMDP:
                return new MMDP(Integer.parseInt(problem.get(1)));

            case enumObjectiveFunctionType::LEADING-ONES:
                return new LeadingOnes(Integer.parseInt(problem.get(1)));

            case enumObjectiveFunctionType::TSP:
                ATSP atsp = new ATSP(Integer.parseInt(problem.get(1)));
                return new TSPObjectiveFunction(atsp);

            case enumObjectiveFunctionType::LOP:
                LinearOrderingProblem lop = new LinearOrderingProblem(Integer.parseInt(problem.get(1)));
                return new LOPObjectiveFunction(lop);

            case enumObjectiveFunctionType::BIN-SPHERE:
                return new BinaryEncodedContinuousObjectiveFunction(Integer.parseInt(problem.get(3)), new Sphere(Integer.parseInt(problem.get(1)), Double.parseDouble(problem.get(2))));

            case enumObjectiveFunctionType::ES-SPHERE:
                return new ESObjectiveFunction(new Sphere(Integer.parseInt(problem.get(1)), Double.parseDouble(problem.get(2))));
        */
    default:
        return nullptr;
    }

}





 /**
  * Creates a diversity measure for a given problem
  * @param problem name of the objective function
  * @return a diversity measure suited to the objective function indicated
  */
/*
ENUM(enumDiversityMeasureType, int, ONEMAX, TRAP, MMDP, LEADING_ONES, TSP, LOP, SPHERE, BIN_SPHERE, ES_ESPHERE)


std::unique_ptr<ea::DiversityMeasure> createMeasure(std::string& problem) {

    size_t found = problem.find('-', 0);
    if (found != std::string::npos)
        problem.replace(found, 1, "_");
    switch (enumDiversityMeasureType::_from_string_nocase(problem.c_str())) {

    case enumDiversityMeasureType::ONEMAX:
    case enumDiversityMeasureType::TRAP:
    case enumDiversityMeasureType::MMDP:
    case enumDiversityMeasureType::LEADING_ONES:
    case enumDiversityMeasureType::TSP:
    case enumDiversityMeasureType::LOP:
    case enumDiversityMeasureType::BIN_SPHERE:
        return std::make_unique<ea::EntropyDiversity>();
    case enumDiversityMeasureType::ES_SPHERE:
    case enumDiversityMeasureType::SPHERE:
        return std::make_unique<ea::VarianceDiversity>();
    default:
        return nullptr;
    }
}
*/


//MAIN AVANZADO

struct Experiment {
    int maxruns;
    std::string problem;
    int numvars;
    float range;
    int numbits;
};

void from_json(const json& j, Experiment & e) {
    j.at("numruns").get_to(e.maxruns);
    j.at("problem").get_to(e.problem);
    j.at("numvars").get_to(e.numvars);
    j.at("range").get_to(e.range);
    j.at("numbits").get_to(e.numbits);
}



int main(int argc, char* argv[]) {

    std::string expName = (argc < 2) ? "experiment.json" : argv[1];

#ifdef EA_PARALLEL
    // Detecta el número de núcleos disponibles
    int max_threads = std::thread::hardware_concurrency();
    tbb::global_control global_limit(
        tbb::global_control::max_allowed_parallelism, 
        max_threads/2
    );
#endif


    try {
        //lee el json  
        std::ifstream expReader(expName);
        if (expReader.fail()) {
            std::cerr << "ERROR when opening " << expName << "\n";
            return 1;
        }
        json je = json::parse(expReader);
        expReader.close();
    
        Experiment experiment = je.get<Experiment>();
        std::println("{}",je);
    
        std::string configf = (experiment.numbits > 0) ? "bitstring.json" : "numeric.json";
        std::ifstream reader(configf);
        if (reader.fail()) {
            std::cerr << "ERROR when opening " << configf << "\n";
            return 1;
        }

        json jc = json::parse(reader);
        reader.close();

        config::EAConfiguration conf = jc.get<config::EAConfiguration>();
    
        long seed;
        std::ifstream lastseedf("lastseed.txt");
        if(lastseedf.fail()){
            seed = conf.seed;
        }
        else {
            lastseedf >> seed;
            conf.seed = ++seed;
        }
        lastseedf.close();

        int numruns = conf.numruns;
        std::print("{}", conf);

        ea::EvolutionaryAlgorithm myEA(conf);
        auto cf  = create(experiment.problem, experiment.numvars, experiment.range);
        if (experiment.numbits > 0) {
            ea::ContinuousObjectiveFunction * cf_cast = dynamic_cast<ea::ContinuousObjectiveFunction *>(cf.get());
            if(cf_cast)
                cf.release();
            else
                throw std::runtime_error("Function must be continuous for binary encoding");
            auto unique_cf_cast = std::unique_ptr<ea::ContinuousObjectiveFunction>(cf_cast);
            myEA.setObjectiveFunction(std::make_unique<ea::BinaryEncodedContinuousObjectiveFunction>(experiment.numbits, std::move(unique_cf_cast)));
            myEA.setDiversityMeasure(std::make_unique<ea::EntropyDiversity>());
        }
        else {
            myEA.setObjectiveFunction(std::move(cf));
            myEA.setDiversityMeasure(std::make_unique<ea::VarianceDiversity>());
        }

        std::cout << "-------------------------------------------------------------------------------------\n";
        std::cout << "Running the EA on " << experiment.problem << "(" << experiment.numvars << ", " << experiment.range << ")";
        if (experiment.numbits > 0) {
            std::cout << " binarized with " << experiment.numbits << " bits per variable";
        }
        else {
            std::cout << " on a continuous domain";
        }
        std::cout << " with seed " << seed << "\n";
        std::cout << "-------------------------------------------------------------------------------------\n";

        for (int i = 0; i < numruns; i++) {
            myEA.run();
            std::cout << "\nRun " << i << ": " <<
                myEA.stats.getTime(i) << "s\t" << myEA.stats.getBest(i).getFitness();
        }
    
        std::string statsfilename = std::format("{}-{}-{}-stats.json", experiment.problem, experiment.numvars, seed);
        std::ofstream file(statsfilename);
        std::format_to(std::ostreambuf_iterator<char>(file), "{}", myEA.stats);
        file.close();

        seed += numruns - 1;
        if (seed < experiment.maxruns) {
            std::ofstream seedFile("lastseed.txt");
            seedFile << seed;
            seedFile.close();
        }
        else {
            std::remove("lastseed.txt");
        }

    }
    catch (std::exception& e) { std::cout << e.what(); }
   
}
