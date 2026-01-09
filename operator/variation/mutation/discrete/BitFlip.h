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

    protected:
        Individual apply_(const individuals_v & parents) override {
            auto ind = Individual(parents[0]);
            auto g = ind.getGenome();
            auto pos = EAUtilRandom::instance().random(g->length());
            g->genes[pos].i = 1 - g->genes[pos].i;
            ind.touch();
            return ind;
        }


    };

}