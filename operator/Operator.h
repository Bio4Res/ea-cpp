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
        std::shared_ptr<ObjectiveFunction> obj = nullptr;


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
        void setObjectiveFunction(const std::shared_ptr<ObjectiveFunction>& obj) {
            this->obj = obj;
            setComparator(obj->getComparator());
        }

        /**
         * Sorts a list of individuals
         * @param pop a list of individuals
         */
        void sort(individuals_v & pop) {
            std::sort(std::execution::par, pop.begin(), pop.end(), [this](const std::shared_ptr<Individual> & a, const std::shared_ptr<Individual> & b) { /*return(comparator(a, b) < 0);*/ return comparator(*a, *b); });
        }
        /**
         * Performs whatever actions are required at the beginning of a run
         */
        void newRun() {
        }
    };

}