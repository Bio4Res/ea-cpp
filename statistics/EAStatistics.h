#pragma once

#include <statistics/Statistics.h>
#include <base/Island.h>

namespace ea {
    struct EAStatistics : public Statistics {
    private:
        /**
         * statistics of every island
         */
        std::vector<std::shared_ptr<Island>>& islands;
        /**
         * list of seeds used in each run
         */
        std::deque<long> seeds;
        /**
         * last seed used
         */
        long currentSeed = 0;
        /**
         * to measure computational times
         */
        std::deque<double> runtime;
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
        EAStatistics(std::vector<std::shared_ptr<Island>>& theislands) : islands(theislands) {

        }


        void clear() override {
            for (const auto& i : islands) {
                i->getStatistics()->clear();
            }
            seeds.clear();
            runtime.clear();
            runActive = false;
        }


        void setComparator(std::function<bool(const Individual&, const Individual&)> comparator) override {
            Statistics::setComparator(comparator);
            for (auto& i : islands) {
                i->getStatistics()->setComparator(comparator);
            }
        }

        void setDiversityMeasure(std::shared_ptr<DiversityMeasure> d) override {
            Statistics::setDiversityMeasure(d);
            for (auto& i : islands) {
                i->getStatistics()->setDiversityMeasure(d);
            }
        }

        void closeRun() override {
            if (runActive) {
                seeds.push_back(currentSeed);
                for (auto& i : islands)
                    i->getStatistics()->closeRun();

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
            for (auto& i : islands)
                i->getStatistics()->newRun();
            runActive = true;
            //tic = System.nanoTime();
            tic = std::chrono::steady_clock::now();

        }

        json toJSON(int i) const override {
            json mijson; 
            mijson["run"] = i;
            mijson["seed"] = seeds.at(i);
            mijson["time"] = runtime.at(i);
            json jsondata = json::array();
            for (auto & island : islands)
                jsondata.push_back(island->getStatistics()->toJSON(i));
            mijson["rundata"] = jsondata;
            return mijson;
        }

        json toJSON() const override {
            json jsondata = json::array(); 
            size_t n = seeds.size();
            for (int i = 0; i < n; i++)
                jsondata.push_back(toJSON(i));
            return jsondata;
        }

        /**
         * Returns the CPU time of a certain run
         * @param i the index of the run
         * @return the CPU time of the i-th run
         */
        double getTime(int i) {
            return runtime[i];
        }

        Individual& getBest(int i) override {
            size_t n = islands.size();
            auto& best = islands.at(0)->getStatistics()->getBest(i);
            for (int j = 1; j < n; j++) {
                auto& cand = islands.at(j)->getStatistics()->getBest(i);
                //if (comparator(cand, best) < 0)
                if (comparator(cand, best))
                    best = cand;
            }
            return best;
        }

        Individual& getBest() override {
            size_t numruns = seeds.size();
            auto& best = getBest(0);
            for (int j = 1; j < numruns; j++) {
                auto& cand = getBest(j);
                //if (comparator(cand, best) < 0)
                if (comparator(cand, best))
                    best = cand;
            }
            return best;
        }

    };
}