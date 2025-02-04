#pragma once

#include <fitness/ObjectiveFunction.h>

namespace ea {

	struct ContinuousObjectiveFunction : public ObjectiveFunction {
	protected:
		/**
		 * Minimum value of variables
		 */
		double* minval;
		/**
		 * maximum value of variables
		 */
		double* maxval;

	public:

		/**
		 * Creates the objective function
		 * @param i number of variables
		 * @param minv minimum value of each variable
		 * @param maxv maximum value of each variable
		 */
		ContinuousObjectiveFunction(int i, double minv = 0.0, double maxv = 1.0) : ObjectiveFunction(i) {
			minval = new double[i];
			maxval = new double[i];
			for (int j = 0; j < i; j++) {
				minval[j] = minv;
				maxval[j] = maxv;
			}
		}

		virtual ~ContinuousObjectiveFunction() {
			delete[] minval;
			delete[] maxval;
		}

		/**
		 * Returns the minimum value of the i-th variable
		 * @param i index of the variable
		 * @return the minimum value of the i-th variable
		 */
		inline double getMinVal(int i) const {
			return minval[i];
		}

		/**
		 * Sets the minimum value of the i-th variable
		 * @param i index of the variable
		 * @param v value of the lower limit
		 */
		inline void setMinVal(int i, double v) {
			minval[i] = v;
		}

		/**
		 * Returns the maximum value of the i-th variable
		 * @param i index of the variable
		 * @return the maximum value of the i-th variable
		 */
		inline double getMaxVal(int i) const {
			return maxval[i];
		}

		/**
		 * Sets the maximum value of the i-th variable
		 * @param i index of the variable
		 * @param v value of the upper limit
		 */
		inline void setMaxVal(int i, double v) {
			maxval[i] = v;
		}


	};
}