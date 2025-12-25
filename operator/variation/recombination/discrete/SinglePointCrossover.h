#pragma once

#include <operator/variation/recombination/RecombinationOperator.h>


/**
 * Single-Point Crossover (SPX)
 */
namespace ea {
    struct SinglePointCrossover : public RecombinationOperator {

        /**
         * Generates the operator
         * @param pars probability of application
         */
        SinglePointCrossover(const std::vector<std::string>& pars) : RecombinationOperator(pars) {

        }

        std::string toString() {
            return { "SPX(" + std::to_string(prob) + ")" };
        }

        constexpr int getArity() override {
            return 2;
        }

    protected:
        Individual apply_(const individuals_v& parents) override {
            const auto father = parents[0].getGenome();
            const auto mother = parents[1].getGenome();
            size_t l = father->length();
            std::unique_ptr<Genotype> offspring = std::make_unique<Genotype>(l, GeneType::INT);

            int c = EAUtilRandom::instance().random(l - 1);

            for (int i = 0; i <= c; i++)
                offspring->genes[i] = father->genes[i];
            for (size_t i = c + 1; i < l; i++)
                offspring->genes[i] = mother->genes[i];

            Individual ind;
            ind.setGenome(std::move(offspring));
            return ind;
        }

    };

}