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

        std::string toString() {
            return { "UX(" + std::to_string(prob) + ")" };
        }

        constexpr int getArity() override {
            return 2;
        }

    protected:
        std::unique_ptr<Individual> apply_(const individuals_v & parents) override {
            const auto father = parents[0]->getGenome();
            const auto mother = parents[1]->getGenome();
            size_t l = father->length();
            std::unique_ptr<Genotype> offspring = std::make_unique<Genotype>(l);

            for (int i = 0; i < l; ++i) {
                if (EAUtilRandom::instance().random(2) > 0)
                    offspring->genes[i] = father->genes[i];
                else
                    offspring->genes[i] = mother->genes[i];
            }
            std::unique_ptr<Individual> ind = std::make_unique<Individual>();
            ind->setGenome(std::move(offspring));
            return ind;
        }


    };

}