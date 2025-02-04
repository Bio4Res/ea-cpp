#pragma once

#include <cmath>
#include <numbers> // PI constant since C++20
#include <fitness/ContinuousObjectiveFunction.h>

namespace ea {

    struct Ackley : public ContinuousObjectiveFunction {
        /**
        * function constant A
        */
    private:
        static constexpr double A = 20.0;
        /**
         * function constants B
         */
        static constexpr double B = 0.2;
        /**
         * function constant C
         */
        static constexpr double C = 2.0 * std::numbers::pi;
    public:
        Ackley(int numvars, double range) : ContinuousObjectiveFunction(numvars, -range, range) {}

        /**
         * Indicates whether the goal is maximization or minimization
         * @return the optimization sense
         */

        OptimizationSense getOptimizationSense() override {
            return OptimizationSense::MINIMIZATION;
        }
    protected:

        double evaluate_(Individual& i) override {
            double s1 = 0.0;
            double s2 = 0.0;
            Genotype* g = i.getGenome();
            for (int j = 0; j < numvars; j++) {
                double v = std::get<double>(g->genes[j]);
                s1 += v * v;
                s2 += std::cos(C * v);
            }
            return -A * std::exp(-B * std::sqrt(s1 / numvars)) - std::exp(s2 / numvars) + A + std::numbers::e;
        } 
    };

}