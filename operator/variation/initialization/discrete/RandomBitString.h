#pragma once

#include <operator/variation/initialization/InitializationOperator.h>

namespace ea {

    struct RandomBitString : public InitializationOperator {
        /**
         * Generates the operator
         * @param pars parameters (none)
         */
        RandomBitString(const std::vector<std::string>& pars) : InitializationOperator(pars) {
        }

        friend std::ostream& operator<<(std::ostream& os, const RandomBitString& op);
    protected:
        std::unique_ptr<Individual> apply_(const individuals_v & parents) override {
            int l = obj->numvars;
            auto g = std::make_unique<Genotype>(l);
            for (int i = 0; i < l; i++)
                g->genes[i] = EAUtilRandom::instance().random(2);

            std::unique_ptr<Individual> ind = std::make_unique<Individual>();
            ind->setGenome(std::move(g));
            return ind;
        }

    };

    std::ostream& operator<<(std::ostream& os, const RandomBitString & op) {
        os << "RandomBitString";
        return os;
    }
}
