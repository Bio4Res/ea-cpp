#pragma once

#include <cmath>
#include <fitness/ContinuousObjectiveFunction.h>

namespace ea {

    struct Griewank : public ContinuousObjectiveFunction {
        /**
        * function constant A
        */
    private:
        static constexpr double A = 1.0/4000.0;

    public:
        Griewank(int numvars, double range) : ContinuousObjectiveFunction(numvars, -range, range) {}

        /**
         * Indicates whether the goal is maximization or minimization
         * @return the optimization sense
         */

        OptimizationSense getOptimizationSense() override {
            return OptimizationSense::MINIMIZATION;
        }
    protected:

        double evaluate_(Individual & i) override {
            Genotype * g = i.getGenome();
            double s = 0.0;
            double p = 1.0;
            for (int j=0; j<numvars; j++) {
                double v = std::get<double>(g->genes[j]);
                s += v*v;
                p *= std::cos(v/std::sqrt(j+1));
            }
            return 1.0 + A*s - p;
        }

    };

}