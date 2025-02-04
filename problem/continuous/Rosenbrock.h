#pragma once

#include <fitness/ContinuousObjectiveFunction.h>

namespace ea {

    struct Rosenbrock : public ContinuousObjectiveFunction {
    private:
        static constexpr double A = 100.0;
    public:
        Rosenbrock(int numvars, double range) : ContinuousObjectiveFunction(numvars, -range, range) {}

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
            for (int j = 1; j < numvars; j++) {
                double v1 = std::get<double>(g->genes[j - 1]);
                double v2 = std::get<double>(g->genes[j]);
                double t1 = v2 - v1 * v1;
                double t2 = 1 - v1;
                c += A * t1 * t1 + t2 * t2;
            }
            return c;
        }

    };

}