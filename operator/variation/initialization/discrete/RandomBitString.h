#pragma once

#include <operator/variation/initialization/InitializationOperator.h>
#include <util/OperatorRegistry.h>

namespace ea {

    struct RandomBitString
        : public RegisteredExtension<VariationOperator, RandomBitString, "BITSTRING", InitializationOperator> {
        /**
         * Generates the operator
         * @param pars parameters (none)
         */
        RandomBitString(const std::vector<std::string>& pars) : RegisteredExtension(pars) {
        }

    protected:
        Individual apply_(const individuals_v & parents) override {
            int l = obj->numvars;
            auto g = std::make_unique<Genotype>(l, GeneType::INT);
            for (int i = 0; i < l; i++)
                g->genes[i].i = EAUtilRandom::instance().random(2);

            Individual ind;
            ind.setGenome(std::move(g));
            return ind;
        }

    };

}
