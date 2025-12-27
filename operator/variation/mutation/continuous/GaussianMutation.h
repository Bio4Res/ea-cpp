#pragma once

#include <algorithm>
#include <fitness/ContinuousObjectiveFunction.h>
#include <operator/variation/mutation/MutationOperator.h>

namespace ea {

    struct GaussianMutation : public MutationOperator {
    private:
        double stepsize;
        ContinuousObjectiveFunction * continuous_obj;
    public:
        /**
         * Creates the operator.
         * @param pars String representation of the mutation probability
         */
        GaussianMutation(const std::vector<std::string>& pars) : MutationOperator(pars) {
            if (pars.size() > 1)
                stepsize = std::stod(pars[1]);
            else
                stepsize = 1.0;
        }

        void setObjectiveFunction(ObjectiveFunction* obj) override {
            MutationOperator::setObjectiveFunction(obj);
            continuous_obj = dynamic_cast<ContinuousObjectiveFunction*>(obj);
            if (!continuous_obj) {
                throw std::runtime_error("This operator requires a ContinuousObjectiveFunction");
            }
        }
        
        friend std::ostream& operator<<(std::ostream& os, const GaussianMutation & indiv);

    protected:
        Individual apply_(const individuals_v& parents) override {
            auto ind = Individual(parents[0]);
            auto g = ind.getGenome();
            auto pos = EAUtilRandom::instance().random(g->length());

            double v = g->genes[pos].d;
            double val = v * (1.0 + stepsize * EAUtilRandom::instance().nrandom());

            g->genes[pos].d = std::min(continuous_obj->getMaxVal(pos), std::max(continuous_obj->getMinVal(pos), val));
            ind.touch();
            return ind;
        }

    };

    std::ostream& operator<<(std::ostream& os, const GaussianMutation & op) {
        os << "GaussianMutation(p=" << op.prob << ")";
        return os;
    }

}