#pragma once

#include <cmath>
#include <fitness/ContinuousObjectiveFunction.h>

namespace ea {

    struct Schaffer2 : public ContinuousObjectiveFunction {
        /**
        * function constant A
        */
    private:
        static constexpr double A = 0.5;
        /**
         * function constants B
         */
        static constexpr double B = 0.001;
        /**
	     * a fixed term in the function that depends on the number of variables
	     */
        double c;
    public:
        Schaffer2(int numvars, double range) : ContinuousObjectiveFunction(numvars, -range, range) {
            c = (double)(numvars-1)*A;
        }

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
            double s = c;
            double v1 = g->genes[0].d;
            double v12 = v1 * v1;
            for (int j=1; j<numvars; j++) {
                double v2 = g->genes[j].d;
                double v22 = v2*v2;
                double d1 = v12 - v22;
                double d2 = v12 + v22;
                double t = std::sin(d1);
                double u = 1 + B*d2;
                s += (t*t-A) / (u*u);
                v12 = v22;
            }
            return s;
        }
        
    };

}