#pragma once

#include <operator/variation/VariationOperator.h>
namespace ea {

    struct RecombinationOperator : public VariationOperator {
        /**
         * Basic constructor of a recombination operator. Passes the parameters to the parent class
         * @param pars parameters (application probability)
         */
        RecombinationOperator(const std::vector<std::string>& pars) : VariationOperator(pars) {
        }

    };

}