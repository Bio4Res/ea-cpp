#pragma once

#include <operator/variation/mutation/MutationOperator.h>

namespace ea {

    struct BitFlip : public MutationOperator {

        /**
         * Creates the operator.
         * @param pars String representation of the mutation probability
         */
        BitFlip(const std::vector<std::string>& pars) : MutationOperator(pars) {
        }

        friend std::ostream& operator<<(std::ostream& os, const BitFlip & indiv);

    protected:
        std::unique_ptr<Individual> apply_(const individuals_v & parents) override {
            auto ind = std::make_unique<Individual>(*parents[0]);
            auto g = ind->getGenome();
            auto pos = EAUtilRandom::instance().random(g->length());
            g->genes[pos] = 1 - std::get<int>(g->genes[pos]);
            ind->touch();
            return ind;
        }


    };

    std::ostream& operator<<(std::ostream& os, const BitFlip & op) {
        os << "BitFlip(" << op.prob << ")";
        return os;
    }

}