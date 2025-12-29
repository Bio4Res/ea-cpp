#pragma once

#include <statistics/Statistics.h>
#include <base/Island.h>

namespace ea {
    struct EAStatistics : public Statistics {
    private:
        /**
         * statistics of every island
         */
        std::vector<Island>& islands;
        /**
         * list of seeds used in each run
         */
        std::vector<long> seeds;
        /**
         * last seed used
         */
        long currentSeed = 0;
        /**
         * to measure computational times
         */
        std::vector<double> runtime;
        /**
         * time at the beginning of a run
         */
        std::chrono::steady_clock::time_point tic;
        /**
         * time at the end of a run
         */
        std::chrono::steady_clock::time_point toc;

    public:

        /**
         * Creates statistics for a collection of islands
         * @param islands the islands whose statistics will be collected.
         */
        EAStatistics(std::vector<Island>& theislands) : islands(theislands) {

        }


        void clear() override {
            const size_t n = islands.size();
            for (size_t i = 0; i < n; ++i) {
                islands[i].getStatistics().clear();
            }
            seeds.clear();
            runtime.clear();
            runActive = false;
        }

        void setComparator(const std::function<bool(const Individual&, const Individual&)>& comparator) override {
            Statistics::setComparator(comparator);
            const size_t n = islands.size();
            for (size_t i = 0; i < n; ++i) {
                islands[i].getStatistics().setComparator(comparator);
            }
        }

        void setDiversityMeasure(DiversityMeasure * dm) override {
            Statistics::setDiversityMeasure(dm);
            const size_t n = islands.size();
            for (size_t i = 0; i < n; ++i) {
                islands[i].getStatistics().setDiversityMeasure(dm);
            }
        }

        void closeRun() override {
            if (runActive) {
                seeds.push_back(currentSeed);
                
                const size_t n = islands.size();
                for (size_t i = 0; i < n; ++i)
                    islands[i].getStatistics().closeRun();

                toc = std::chrono::steady_clock::now();
                std::chrono::duration<double> diff = toc - tic;
                runtime.push_back(diff.count());
            }
            runActive = false;
        }

        void newRun() override {
            if (runActive)
                closeRun();
            currentSeed = EAUtilRandom::instance().getSeed();
            
            const size_t n = islands.size();
            for (size_t i = 0; i < n; ++i)
                islands[i].getStatistics().newRun();
            
            runActive = true;
            tic = std::chrono::steady_clock::now();
        }

        json toJSON(int i) const override {
            json mijson; 
            mijson["run"] = i;
            mijson["seed"] = seeds[i];
            mijson["time"] = runtime[i];
            
            json jsondata = json::array();
            const size_t n = islands.size();
            for (size_t j = 0; j < n; ++j)
                jsondata.push_back(islands[j].getStatistics().toJSON(i));
            
            mijson["rundata"] = std::move(jsondata);
            return mijson;
        }

        json toJSON() const override {
            json jsondata = json::array(); 
            const size_t n = seeds.size();
            for (size_t i = 0; i < n; ++i)
                jsondata.push_back(toJSON(i));
            return jsondata;
        }

        /**
         * Returns the CPU time of a certain run
         * @param i the index of the run
         * @return the CPU time of the i-th run
         */
        double getTime(int i) const {
            return runtime[i];
        }

        Individual& getBest(int i) override {
            Individual* best = &islands[0].getStatistics().getBest(i);
            const size_t n = islands.size();
            
            for (size_t j = 1; j < n; ++j) {
                Individual& cand = islands[j].getStatistics().getBest(i);
                if (comparator(cand, *best))
                    best = &cand;
            }
            return *best;
        }

        Individual& getBest() override {
            const size_t numruns = seeds.size();
            if (numruns == 1) return getBest(0);
            
            Individual* best = &getBest(0);
            for (size_t j = 1; j < numruns; ++j) {
                Individual& cand = getBest(j);
                if (comparator(cand, *best))
                    best = &cand;
            }
            return *best;
        }

    };
}