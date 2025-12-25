#pragma once

#include <fitness/ContinuousObjectiveFunction.h>

namespace ea {

    struct Sphere : public ContinuousObjectiveFunction {
    public:
        Sphere(int numvars, double range) : ContinuousObjectiveFunction(numvars, -range, range) {}

        /**
         * Indicates whether the goal is maximization or minimization
         * @return the optimization sense
         */

        OptimizationSense getOptimizationSense() override {
            return OptimizationSense::MINIMIZATION;
        }
    protected:

        double evaluate_(Individual& i) override {
            double c = 0.0;
            Genotype* g = i.getGenome();
            for (int j = 0; j < numvars; j++) {
                const double v = g->genes[j].d;
                c += v * v ;
            }
            return c;
        }
    };

}