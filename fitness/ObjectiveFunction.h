#pragma once

#include <functional>
#include <base/Individual.h>

namespace ea {
    enum class OptimizationSense : int {
        /**
         * maximization
         */
        MAXIMIZATION,
        /**
         * minimization
         */
         MINIMIZATION
    };

    struct ObjectiveFunction {
        /**
         * total number of calls to the objective function
         */
        long evals = 0;
        /**
         * used to store additional cost representing some work
         * done by external problem-aware operators that somehow
         * use the problem information to take fitness-informed
         * decisions or compute fitness partially.
         */
        double extracost = 0.0;
        /**
         * number of variables
         */
        int numvars = 0;

        /**
         * Basic constructor indicating how many variables there are
         * @param i number of variables
         */
        ObjectiveFunction(int i) {
            numvars = i;
        }

        virtual ~ObjectiveFunction(){}
        /**
         * Indicates whether the goal is maximization or minimization
         * @return the optimization sense
         */
        virtual OptimizationSense getOptimizationSense() = 0;
        /**
         * Returns the comparator to use given the optimization sense
         * @return the comparator to use given the optimization sense
         */
        std::function<bool(const Individual&, const Individual&)> getComparator() {
            if (getOptimizationSense() == OptimizationSense::MAXIMIZATION)
                return maxComp;
            else
                return minComp;
        }
        /**
         * Returns the number of evaluations so far
         * @return the number of evaluations so far
         */
        long getEvals() const {
            //return evals + (long)extracost;
            return evals + std::lround(extracost);
        }

        /**
         * Adds additional extra cost
         * @param v the amount of extra cost added
         */
        void addExtraCost(double v) {
            extracost += v;
        }
        /**
         * Resets the number of evaluations
         */
        void reset() {
            evals = 0;
            extracost = 0.0;
        }
        /**
         * Performs whatever actions are required at the start of a run
         * (e.g., when a problem generator is used to create new data for each run).
         * In particular, resets the number of evaluations
         */
        void newRun() {
            reset();
        }
        /**
         * Evaluates an individual (also sets internally its fitness)
         * @param i the individual
         * @return the fitness of the individual
         */
        double evaluate(Individual& i) {
            if (!i.isEvaluated()) {
                evals++;
                double f = evaluate_(i);
                i.setFitness(f);
                return f;
            }
            else
                return i.getFitness();
        }

        /**
         * Internal method to evaluate an individual
         * @param i the individual
         * @return the fitness of the individual
         */
    protected:
        virtual double evaluate_(Individual& i) = 0;

    };

}