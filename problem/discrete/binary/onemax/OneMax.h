#pragma once

#include <fitness/DiscreteObjectiveFunction.h>

namespace ea {

    struct OneMax : public DiscreteObjectiveFunction {
        /**
         * Basic constructor
         * @param i the number of bits
         */
        OneMax(int i) : DiscreteObjectiveFunction(i, 2) {

        }

        /**
         * Indicates whether the goal is maximization or minimization
         * @return the optimization sense
         */

        OptimizationSense getOptimizationSense() override {
            return OptimizationSense::MAXIMIZATION;
        }
    protected:

        double evaluate_(Individual& i) override {
            long c = 0;
            Genotype * g = i.getGenome();
            for (int j = 0; j < numvars; j++) {
                c += g->genes[j].i;
            }
            return (double)c;
        }

    };


}