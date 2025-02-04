#pragma once

#include <operator/Operator.h>

namespace ea {
    struct SelectionOperator : public Operator {

        /**
         * Applies the selection operator on a parental population.
         * Returns the number of individuals requested.
         * @param population parental population
         * @param num the number of individuals to be selected
         * @return the populations of selected individuals
         */
        virtual std::unique_ptr<individuals_v> apply(individuals_v & population, const int& num) = 0;


    };

}