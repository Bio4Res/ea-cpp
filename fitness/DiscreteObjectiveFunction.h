#pragma once

#include <fitness/ObjectiveFunction.h>

namespace ea {
    /**
* Abstract class for objective functions on discrete variables
* @author ccottap
* @version 1.0
*/
    struct DiscreteObjectiveFunction : public ObjectiveFunction {
        /**
         * alphabet size by default
         */
        static const int NUMVAL = 2;

        /**
         * Number of values of variables (alphabet size for each variable)
         */
    protected:
        int* numval;

    public:
        /**
         * Creates the objective function
         * @param i number of variables
         */
        DiscreteObjectiveFunction(int n, int size = NUMVAL) : ObjectiveFunction(n) {
            numval = new int[n];
            for (int j = 0; j < n; j++) {
                setAlphabetSize(j, size);
            }
        }

        virtual ~DiscreteObjectiveFunction() {
            delete[] numval;
        }

        /**
         * Returns the alphabet size of the i-th variable
         * @param i index of the variable
         * @return the alphabet size of the i-th variable
         */
        int getAlphabetSize(int i) {
            return numval[i];
        }

        /**
         * Sets the alphabet size of the i-th variable
         * @param i index of the variable
         * @param v value of the alphabet size
         */
        void setAlphabetSize(int i, int v) {
            numval[i] = v;
        }

    };





}