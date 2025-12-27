#pragma once

#include <base/Individual.h>
#include <fitness/ObjectiveFunction.h>
#include <functional>
#include <memory>
#include <algorithm>

#define PARALLEL
#include <execution>

namespace ea {

    struct Operator {

        virtual ~Operator() = default;
        /**
         * to compare individuals
         */
        std::function<bool(const Individual&, const Individual&)> comparator;

        /**
         * the objective function (for potential use)
         */
        ObjectiveFunction* obj = nullptr;


        /**
         * Sets the comparator to compare individuals
         * @param comparator the comparator to compare individuals
         */
        void setComparator(const std::function<bool(const Individual&, const Individual&)>& thecomparator) {
            comparator = thecomparator;
        }
        /**
         * Sets the objective function for potential use inside some operator
         * @param obj the objective function
         */
        virtual void setObjectiveFunction(ObjectiveFunction * theobjf) {
            this->obj = theobjf;
            setComparator(obj->getComparator());
        }

        /**
         * Sorts a list of individuals
         * @param pop a list of individuals
         */
        inline void sort(individuals_v & pop) {
        #ifdef EA_PARALLEL
            std::sort(std::execution::par_unseq, pop.begin(), pop.end(), [this](const Individual & a, const Individual & b) { return comparator(a, b); });
        #else
            std::sort(std::execution::seq, pop.begin(), pop.end(), [this](const Individual & a, const Individual & b) { return comparator(a, b); });       
        #endif
        }

        /**
         * Performs whatever actions are required at the beginning of a run
         */
        void newRun() {
        }
    };

}