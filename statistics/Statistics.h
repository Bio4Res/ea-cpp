#pragma once

#include <memory>
#include <functional>
#include <base/Individual.h>
#include <statistics/DiversityMeasure.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace ea {

    struct IndividualRecord {
        long evals;
        Individual individual;
    };


    struct StatsEntry {
        long evals;
        double best;
        double mean;
        double diversity;
    };


    struct Statistics {
    protected:
        /**
         * whether a run is active or not
         */
        bool runActive = false;

        /**
         * to compare solutions
         */
        std::function<bool(const Individual&, const Individual&)> comparator;

        /**
         * to compute diversity
         */
        DiversityMeasure * diversity{ nullptr };

        /**
         * Sets the comparator used to compare individuals
         * @param comparator the comparator
         */
    public:
        virtual void setComparator(const std::function<bool(const Individual&, const Individual&)> & comparator) {
            this->comparator = comparator;
        }

        /**
         * Sets the diversity measure used in the population
         * @param diversity the diversity measure
         */
        virtual void setDiversityMeasure(DiversityMeasure * diversity_m) {
            diversity = diversity_m;
        }

        /**
         * Clears all statistics
         */
        virtual void clear() = 0;

        /**
         * Logs the start of a new run
         */
        virtual void newRun() = 0;

        /**
         * Closes the current run and commits the statistics to the
         * global record.
         */
        virtual void closeRun() = 0;

        /**
         * Returns the best solution of a given run
         * @param i the index of the run
         * @return the best individual inthe i-th run
         */
        virtual Individual& getBest(int i) = 0;

        /**
         * Returns the best solution of all runs
         * @return the best solution of all runs
         */
        virtual Individual& getBest() = 0;

        /**
         * Returns the data of a certain run in JSON format
         * @param i the run index
         * @return a JSON object with the data of the i-th run
         */
        virtual json toJSON(int i) const = 0;

        /**
         * Returns the data of all runs in JSON format. Any active, non-closed run is not recorded.
         * @return a JSON array with the data of all runs
         */
        virtual json toJSON() const = 0;

        virtual ~Statistics() {
            
        }
    };

}