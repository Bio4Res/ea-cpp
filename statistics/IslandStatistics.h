#pragma once

#include <statistics/Statistics.h>

namespace ea {

    struct IslandStatistics : public Statistics {
    private:
        /**
         * statistics of all runs
         */
        std::vector<std::vector<StatsEntry> *> stats;
        /**
         * statistics of the current run
         */
        std::vector<StatsEntry> * current = nullptr;
        /**
         * evolution of the best solutions in all runs
         */
        std::vector<std::vector<IndividualRecord> *> sols;
        /**
         * evolution of the best solution in the current run
         */
        std::vector<IndividualRecord>* currentSols = nullptr;
        /**
         * last best solution in the current run
         */
        std::shared_ptr<Individual> last = nullptr;
    public:
        /**
         * Initializes statistics for a batch of runs
         */
        IslandStatistics() {
            clear(); //FIX ME: not needed?
            setDiversityMeasure(nullptr);
        }

        ~IslandStatistics() {
            clear();
        }

        void clear() override {
            std::for_each(stats.begin(), stats.end(), [](auto* elem) { delete elem; });
            stats.clear();
            std::for_each(sols.begin(), sols.end(), [](auto* elem) { delete elem; });
            sols.clear();
            if (current != nullptr)
                delete current;
            if (currentSols != nullptr)
                delete currentSols;
            runActive = false;
        }

        void newRun() override {
            if (runActive)
                closeRun();
            current = new std::vector<StatsEntry>;
            currentSols = new std::vector<IndividualRecord>;
            runActive = true;
        }

        void closeRun() override {
            if (runActive) {
                stats.push_back(current);
                sols.push_back(currentSols);
            }
            current = nullptr;
            currentSols = nullptr;
            runActive = false;
        }

        /**
         * Takes statistics of the population at a given time
         * @param evals number of evaluations so far
         * @param pop the population
         */
        void takeStats(long evals, individuals_v & pop) {
            auto & best = pop.at(0);
            double mean = best->getFitness();
            size_t l = pop.size();

            for (int i = 1; i < l; i++) {
                auto & ind = pop.at(i);
                //if (comparator(ind, best) < 0) {
                if (comparator(*ind, *best)) {
                    best = ind;
                }
                mean += ind->getFitness();
            }
            mean /= l;

            double h = 0.0;
            if (diversity != nullptr)
                h = diversity->apply(pop);

            current->push_back(StatsEntry{ evals, best->getFitness(), mean, h });

            if ((currentSols->size() == 0) || /*(comparator(best, last) < 0))*/ comparator(*best, *last)) {
                currentSols->push_back(IndividualRecord{ evals, best });
                last = best;
            }
        }


        /**
         * Returns the data of a certain run in JSON format
         * @param i the run index
         * @return a JSON object with the data of the i-th run
         */

        json toJSON(int i) const override {
            json jsondata;

            json jsonstats;
            json jsonevals = json::array();
            json jsonbest = json::array();
            json jsonmean = json::array();
            json jsondiv = json::array();
            
            auto * data = stats[i];
            for (auto & s : *data) {
                jsonevals.push_back(s.evals);
                jsonbest.push_back(s.best);
                jsonmean.push_back(s.mean);
                jsondiv.push_back(s.diversity);
            }
                        jsonstats["evals"] = jsonevals;
                        jsonstats["best"]  = jsonbest;
                        jsonstats["mean"] = jsonmean;
                        jsonstats["diversity"] = jsondiv;
                        
                        jsondata["idata"] = jsonstats;

                        json jsonsols;
                        json jsonsolsevals = json::array();
                        json jsonsolsfitness = json::array();
                        json jsonsolsgenome = json::array();
                        
                        auto * soldata = sols[i];
                        for (auto & p : *soldata) {
                            jsonsolsevals.push_back(p.evals);
                            jsonsolsfitness.push_back(p.individual->getFitness());
                            const auto & g = p.individual->getGenome();
                            json jsongenome = json::array();
                            size_t n = g->length();
                            for (int j = 0; j < n; j++) {
                                if (auto* v = std::get_if<int>(&(g->genes[j])); v != nullptr) {
                                    jsongenome.push_back(*v);
                                }else{
                                    jsongenome.push_back(std::get<double>(g->genes[j]));
                                }
                            }
                            jsonsolsgenome.push_back(jsongenome);
                        }
                        jsonsols["evals"] = jsonsolsevals;
                        jsonsols["fitness"] = jsonsolsfitness;
                        jsonsols["genome"] = jsonsolsgenome;
                        
                        jsondata["isols"] = jsonsols;
                        
                        return jsondata;
        }

        json toJSON() const override {
            json jsondata = json::array();
            size_t n = stats.size();
            for (int i = 0; i < n; i++)
                jsondata.push_back(toJSON(i));
            return jsondata;
        }

        Individual& getBest(int i) override {
            auto * data = sols.at(i);
            return *data->back().individual;
            //int n = static_cast<int>(data->size());
            //return data->at(n - 1).individual;
        }

        Individual& getBest() override {
            size_t numruns = stats.size();
            Individual& best = getBest(0);
            for (int j = 1; j < numruns; j++) {
                Individual& cand = getBest(j);
                //if (comparator(cand, best) < 0)
                if (comparator(cand, best))
                    best = cand;
            }
            return best;
        }

        friend std::ostream& operator<<(std::ostream& os, const IslandStatistics& is);
        /*
        std::string toString() {
            std::string str = "";
            auto runs = stats.size();
            for (int i = 0; i < runs; i++) {
                auto* runstats = stats.at(i);
                str += "Run " + std::to_string(i) + "\n=======\n";
                str += "#evals\tbest\tmean\n------\t----\t----\n";
                std::for_each(runstats->begin(), runstats->end(), [&str](auto& s) {
                    str += std::to_string(s.evals) + "\t" + std::to_string(s.best) + "\t" + std::to_string(s.mean) + "\n";
                    });
            }
            return str;
        }
        */

    };

    std::ostream& operator<<(std::ostream& os, const IslandStatistics& is){
        size_t runs = is.stats.size();
        for (int i = 0; i < runs; i++) {
            auto * runstats = is.stats.at(i);
            os << "Run " << i << "\n=======\n" << "#evals\tbest\tmean\n------\t----\t----\n";
            std::for_each(runstats->begin(), runstats->end(), [&os](auto & s) {
                os << s.evals << "\t" << s.best << "\t" << s.mean << "\n";
                });
        }
        return os;
    }

}
