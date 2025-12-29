#pragma once

#include <statistics/Statistics.h>

namespace ea {

    struct IslandStatistics : public Statistics {
    private:
        /**
         * statistics of all runs
         */
        std::vector<std::vector<StatsEntry>> stats;
        /**
         * statistics of the current run
         */
        std::vector<StatsEntry> current;
        /**
         * evolution of the best solutions in all runs
         */
        std::vector<std::vector<IndividualRecord>> sols;
        /**
         * evolution of the best solution in the current run
         */
        std::vector<IndividualRecord> currentSols;
        /**
         * last best solution in the current run
         */
        Individual last;
    public:
        /**
         * Initializes statistics for a batch of runs
         */
        IslandStatistics() {
             setDiversityMeasure(nullptr);
        }

        ~IslandStatistics() = default;

        void clear() override {
            stats.clear();
            sols.clear();
            current.clear();
            currentSols.clear();
            runActive = false;
        }

        void newRun() override {
            if (runActive)
                closeRun();
            current.clear();
            currentSols.clear();
            runActive = true;
        }

        void closeRun() override {
            if (runActive) {
                stats.push_back(std::move(current));
                sols.push_back(std::move(currentSols));
                
                current.clear(); 
                currentSols.clear();
            }
             runActive = false;
        }

        /**
         * Takes statistics of the population at a given time
         * @param evals number of evaluations so far
         * @param pop the population
         */
        void takeStats(long evals, individuals_v & pop) {
            auto & best = pop.at(0);
            double mean = best.getFitness();
            size_t l = pop.size();

            for (size_t i = 1; i < l; i++) {
                auto & ind = pop.at(i);
                //if (comparator(ind, best) < 0) {
                if (comparator(ind, best)) {
                    best = ind;
                }
                mean += ind.getFitness();
            }
            mean /= l;

            double h = 0.0;
            if (diversity != nullptr)
                h = diversity->apply(pop);

            current.push_back(StatsEntry{ evals, best.getFitness(), mean, h });

            if ((currentSols.empty()) || comparator(best, last)) {
                currentSols.push_back(IndividualRecord{ evals, best });
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
            
            const auto& data = stats[i];
            for (const auto & s : data) {
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
                        
            const auto& soldata = sols[i];
            for (const auto & p : soldata) {
                jsonsolsevals.push_back(p.evals);
                jsonsolsfitness.push_back(p.individual.getFitness());
                const auto & g = p.individual.getGenome();
                json jsongenome = json::array();
                size_t n = g->length();
                for (size_t j = 0; j < n; j++) {
                    if (g->getType() == GeneType::INT) {
                        jsongenome.push_back(g->genes[j].i);
                    }else{
                        jsongenome.push_back(g->genes[j].d);
                    }
                }
                jsonsolsgenome.push_back(jsongenome);
            }
            jsonsols["evals"] = std::move(jsonsolsevals);
            jsonsols["fitness"] = std::move(jsonsolsfitness);
            jsonsols["genome"] = std::move(jsonsolsgenome);
                        
            jsondata["isols"] = std::move(jsonsols);
                        
            return jsondata;
        }

        json toJSON() const override {
            json jsondata = json::array();
            size_t n = stats.size();
            for (size_t i = 0; i < n; i++)
                jsondata.push_back(toJSON(i));
            return jsondata;
        }

        Individual& getBest(int i) override {
            auto data = sols.at(i);
            return data.back().individual;
        }

        Individual& getBest() override {
            size_t numruns = stats.size();
            Individual& best = getBest(0);
            for (size_t j = 1; j < numruns; j++) {
                Individual& cand = getBest(j);
                //if (comparator(cand, best) < 0)
                if (comparator(cand, best))
                    best = cand;
            }
            return best;
        }

        friend std::ostream& operator<<(std::ostream& os, const IslandStatistics& is);

    };

    std::ostream& operator<<(std::ostream& os, const IslandStatistics& is){
        size_t runs = is.stats.size();
        for (size_t i = 0; i < runs; i++) {
            const auto runstats = is.stats.at(i);
            os << "Run " << i << "\n=======\n" << "#evals\tbest\tmean\n------\t----\t----\n";
            std::for_each(runstats.begin(), runstats.end(), [&os](auto & s) {
                os << s.evals << "\t" << s.best << "\t" << s.mean << "\n";
                });
        }
        return os;
    }

}
