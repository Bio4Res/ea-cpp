#pragma once

#include <operator/Operator.h>
namespace ea {


    struct ReplacementOperator : public Operator {

        /**
         * Applies the replacement operator on a parental population
         * and an offspring population. Returns the result of the replacement.
         * @param population parental population
         * @param offspring offspring population
         * @return the resulting population after replacement
         */
        virtual individuals_v apply(individuals_v && population, individuals_v && offspring) = 0;
    };

}