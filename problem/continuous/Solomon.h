#pragma once

#include <cmath>
#include <numbers> // PI constant since C++20
#include <fitness/ContinuousObjectiveFunction.h>

namespace ea {

    struct Solomon : public ContinuousObjectiveFunction {
        /**
        * function constant A
        */
    private:
        static constexpr double A = 2.0 * std::numbers::pi;
        /**
         * function constants B
         */
        static constexpr double B = 0.1;
    public:
        Solomon(int numvars, double range) : ContinuousObjectiveFunction(numvars, -range, range) { }

        /**
         * Indicates whether the goal is maximization or minimization
         * @return the optimization sense
         */

        OptimizationSense getOptimizationSense() override {
            return OptimizationSense::MINIMIZATION;
        }
    protected:
        double evaluate_(Individual & i) override {
            Genotype* g = i.getGenome();
            double s = 0.0;
            for (int j=0; j<numvars; j++) {
                double v = std::get<double>(g->genes[j]);
                s += v*v;
            }
            s = std::sqrt(s);
            return 1.0 - std::cos(A*s) + B*s;
        }
  
    };

}