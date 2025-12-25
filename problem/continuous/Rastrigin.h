#pragma once

#include <cmath>
#include <numbers>
#include <fitness/ContinuousObjectiveFunction.h>

namespace ea {

    struct Rastrigin : public ContinuousObjectiveFunction {
    private:
        static constexpr double A = 10.0;
    public:
        Rastrigin(int numvars, double range) : ContinuousObjectiveFunction(numvars, -range, range) {}

        /**
         * Indicates whether the goal is maximization or minimization
         * @return the optimization sense
         */

        OptimizationSense getOptimizationSense() override {
            return OptimizationSense::MINIMIZATION;
        }
    protected:

        double evaluate_(Individual& i) override {
            double c = A * numvars;
            Genotype* g = i.getGenome();
            for (int j = 0; j < numvars; j++) {
                const double v = g->genes[j].d;
                c += v * v - A * std::cos(2.0 * std::numbers::pi * v);
            }
            return c;
        }

    };

}