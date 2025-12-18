#pragma once

#include <cmath>
#include <fitness/ContinuousObjectiveFunction.h>

namespace ea {

    struct XinSheYang2 : public ContinuousObjectiveFunction {
    public:
        XinSheYang2(int numvars, double range) : ContinuousObjectiveFunction(numvars, -range, range) { }

        /**
         * Indicates whether the goal is maximization or minimization
         * @return the optimization sense
         */

        OptimizationSense getOptimizationSense() override {
            return OptimizationSense::MINIMIZATION;
        }
    protected:
        double evaluate_(Individual & i) override{
            Genotype* g = i.getGenome();
            double s1 = 0.0;
            double s2 = 0.0;
            for (int j=0; j<numvars; j++) {
                double v = std::get<double>(g->genes[j]);
                s1 += std::abs(v);
                s2 += std::sin(v*v);
            }
            return s1 / std::exp(s2);
        }
  
    };

}