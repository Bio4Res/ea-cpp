#pragma once

#include <fitness/ContinuousObjectiveFunction.h>
#include <fitness/DiscreteObjectiveFunction.h>

namespace ea {

struct BinaryEncodedContinuousObjectiveFunction: public DiscreteObjectiveFunction {
private:
	/**
	 * the continuous objective function to be optimized
	 */
	std::shared_ptr<ContinuousObjectiveFunction> func;
	/**
	 * number of bits to encode each continuous variable
	 */
	int bits;
	/**
	 * number of continuous variables
	 */
	int numContVars;
	/**
	 * integer value of a string with all bits equal to one
	 */
	long maxval;
	/**
	 * auxiliary individual to make the translation from binary to continuous
	 */
	std::unique_ptr<Individual> aux;

	/**
	 * Creates the shell for a function, indicating the number of bits to be used for encoding each
	 * of the continuous variables
	 * @param bitsPerVar number of bits to be used for encoding each of the continuous variables
	 * @param cof a continuous objective function
	 */
public:
	BinaryEncodedContinuousObjectiveFunction(int bitsPerVar, std::shared_ptr<ContinuousObjectiveFunction> cof)
		: DiscreteObjectiveFunction(bitsPerVar * cof->numvars, 2){
		bits = bitsPerVar;
		func = cof;
		numContVars = cof->numvars;
		std::unique_ptr<Genotype> g = std::make_unique<Genotype>(numContVars);
		aux = std::make_unique<Individual>();
		aux->setGenome(std::move(g));
		maxval = 0;
		for (int i = 0; i < bits; i++)
			maxval = (maxval << 1) + 1;
	}

	OptimizationSense getOptimizationSense() override{
		return func->getOptimizationSense();
	}

protected:
		double evaluate_(Individual & ind) override{
		Genotype * g0 = ind.getGenome();
		Genotype * gaux = aux->getGenome();
		for (int i = 0, j = 0; i < numContVars; i++, j += bits) {
			long val = 0;
			for (int k = 0; k < bits; k++) {
				val <<= 1;
				val += std::get<int>(g0->genes[j + k]);
			}
			gaux->genes[i] = (double)val / (double)maxval * (func->getMaxVal(i) - func->getMinVal(i)) + func->getMinVal(i);
		}
		aux->touch();
		return func->evaluate(*aux);
	}

};

}
