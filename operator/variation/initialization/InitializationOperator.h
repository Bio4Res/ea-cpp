#pragma once

#include <operator/variation/VariationOperator.h>

namespace ea {

    struct InitializationOperator : public VariationOperator {
        /**
         * Basic constructor of a mutation operator. Passes the parameters to the parent class
         * @param pars parameters.
         */
        InitializationOperator(const std::vector<std::string>& pars) : VariationOperator(pars) {
        }

        constexpr int getArity() override {
            return 0;
        }


    };
}