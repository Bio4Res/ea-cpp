#pragma once

#include <operator/Operator.h>
#include <util/EAUtil.h>

namespace ea {

    struct VariationOperator : public Operator {
        /**
         * operator application probability
         */
        double prob;

        /**
         * Creates the operator.
         * @param pars String representation of the application probability. If the
         * list provided is empty, it is assumed that the probability is 1.0
         */
        VariationOperator(const std::vector<std::string>& pars) {
            prob = pars.size() > 0 ? std::strtod(pars.front().c_str(), nullptr) : 1.0;
        }

        /**
         * Returns the application probability
         * @return the application probability
         */
        double getProbability() {
            return prob;
        }

        /**
         * Sets the application probability
         * @param prob the application probability
         */
        void setProbability(double prob) {
            this->prob = prob;
        }

        /**
         * Returns the arity of the operator (how many individual it expects as an input).
         * @return the arity of the operator
         */
        constexpr virtual int getArity() = 0;

        /**
         * Applies the operator to a collection of parents.
         * If the number of parents does not match the arity of the operator,
         * null is returned.
         * @param parents the parents
         * @return an individual
         */
        std::unique_ptr<Individual> apply(const individuals_v & parents) {
            std::unique_ptr<Individual> child;
            if (parents.size() == getArity()) {
                if (EAUtilRandom::instance().random01() < prob) {
                    child = apply_(parents);
                }
                else
                    child = std::make_unique<Individual>(*parents.front());
            }
            return child;
        }

        /**
         * Internal method for applying the operator. Receives a list of individuals,
         * and returns a full-fledged individual whose fitness information might be
         * potentially valid (that would depend on the operator used; for example, if
         * local search is used, the individual will be already typically evaluated).
         * @param parents a list of individuals
         * @return an individual created from those parents
         */
    protected:
        virtual std::unique_ptr<Individual> apply_(const individuals_v & parents) = 0;

    };

}
