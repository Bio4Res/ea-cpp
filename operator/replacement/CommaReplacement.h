#pragma once

#include <operator/replacement/ReplacementOperator.h>

namespace ea {

    struct CommaReplacement : public ReplacementOperator {
            /**
             * Performs comma replacement. Let mu = |population|. If lambda =
             * |offspring| is larger than mu, it keeps the best mu individuals
             * out of the offspring. Otherwise, it keeps the offspring and the
             * best mu-lambda individuals in the population.
             * @param population parental population
             * @param offspring offspring population
             * @return the resulting population after replacement
             */

        individuals_v apply(individuals_v && population, individuals_v && offspring) override {
            size_t mu = population.size();
            size_t lambda = offspring.size();
            assert(lambda > 0);
            sort(offspring);
            individuals_v result;
            result.reserve(mu);
            auto num = std::min(lambda, mu);
            for (size_t i = 0; i < num; i++)
                result.push_back(std::move(offspring[i]));
            if (lambda < mu) {
                num = mu - lambda;
                sort(population);
                for (size_t i = 0; i < num; i++) {
                    result.push_back(std::move(population[i]));
                }
            }
            return result;

        }

        std::string toString() {
            return "CommaReplacement";
        }

    };

}