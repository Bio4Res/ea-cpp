
#pragma once

#include <cmath>
#include <fitness/ContinuousObjectiveFunction.h>

namespace ea {

    struct Schwefel : public ContinuousObjectiveFunction {
        /**
        * function constant A
        */
    private:
        static constexpr double A = 418.9829;
        /**
	     * a fixed term in the function that depends on the number of variables
	     */
        double c;
    public:
        Schwefel(int numvars, double range) : ContinuousObjectiveFunction(numvars, -range, range) {
            c = (double)numvars*A;
        }

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
            double s = 0.0;
            for (int j=0; j<numvars; j++) {
                double v = std::get<double>(g->genes[j]);
                s += v*std::sin(std::sqrt(std::abs(v)));
            }
            return c - s;
        }
  
    };

}