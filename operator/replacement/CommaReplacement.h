#pragma once

#include <operator/replacement/ReplacementOperator.h>

namespace ea {

    struct CommaReplacement : public ReplacementOperator {
        //	/**
        //	 * Creates the operator given the optimization sense
        //	 * @param sense the optimization sense
        //	 */
        //	public CommaReplacement(OptimizationSense sense) {
        //		super(sense);
        //	}

            /**
             * Performs comma replacement. Let mu = |population|. If lambda =
             * |offspring| is larger than mu, it keeps the best mu individuals
             * out of the offspring. Otherwise, it keeps the offspring and the
             * best mu-lambda individuals in the population.
             * @param population parental population
             * @param offspring offspring population
             * @return the resulting population after replacement
             */

        std::unique_ptr<individuals_v> apply(individuals_v & population, individuals_v & offspring) override {
            size_t mu = population.size();
            size_t lambda = offspring.size();
            assert(lambda > 0);
            sort(offspring);
            auto result = std::make_unique<individuals_v>();
            result->reserve(mu);
            auto num = std::min(lambda, mu);
            for (int i = 0; i < num; i++)
                result->push_back(offspring[i]);
            if (lambda < mu) {
                sort(population);
                num = mu - lambda;
                for (int i = 0; i < num; i++) {
                    result->push_back(population[i]);
                }
            }
            return result;

        }

        std::string toString() {
            return "CommaReplacement";
        }

    };

}