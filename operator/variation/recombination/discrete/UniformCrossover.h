#pragma once

#include <memory>
#include <operator/variation/recombination/RecombinationOperator.h>

namespace ea {
    struct UniformCrossover : public RecombinationOperator {

        /**
         * Generates the operator
         * @param pars probability of application
         */
        UniformCrossover(const std::vector<std::string>& pars) : RecombinationOperator(pars) {

        }
/*
        std::string toString() {
            return { "UX(" + std::to_string(prob) + ")" };
        }
*/
        constexpr int getArity() override {
            return 2;
        }

    protected:
        Individual apply_(const individuals_v & parents) override {
            const auto father = parents[0].getGenome();
            const auto mother = parents[1].getGenome();
            size_t l = father->length();
            std::unique_ptr<Genotype> offspring = std::make_unique<Genotype>(l, GeneType::INT);

            for (size_t i = 0; i < l; ++i) {
                if (EAUtilRandom::instance().random(2) > 0)
                    offspring->genes[i] = father->genes[i];
                else
                    offspring->genes[i] = mother->genes[i];
            }
            Individual ind;
            ind.setGenome(std::move(offspring));
            return ind;
        }


    };

}