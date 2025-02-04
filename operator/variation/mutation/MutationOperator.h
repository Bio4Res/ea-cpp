#pragma once

#include <operator/variation/VariationOperator.h>

namespace ea {

    struct MutationOperator : VariationOperator {
        /**
         * Basic constructor of a mutation operator. Passes the parameters to the parent class
         * @param pars parameters (application probability)
         */
        MutationOperator(const std::vector<std::string>& pars) : VariationOperator(pars) {
        }

        constexpr int getArity() override {
            return 1;
        }
    protected:
        virtual std::unique_ptr<Individual> apply_(const individuals_v & parents) override = 0;

    };

}