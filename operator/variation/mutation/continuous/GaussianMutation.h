#pragma once

#include <algorithm>
#include <fitness/ContinuousObjectiveFunction.h>
#include <operator/variation/mutation/MutationOperator.h>

namespace ea {

    struct GaussianMutation : public MutationOperator {
    private:
        double stepsize;
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
        
        friend std::ostream& operator<<(std::ostream& os, const GaussianMutation & indiv);

    protected:
        Individual apply_(const individuals_v& parents) override {
            auto p = std::dynamic_pointer_cast<ContinuousObjectiveFunction>(obj);
            auto ind = Individual(parents[0]);
            auto g = ind.getGenome();
            auto pos = EAUtilRandom::instance().random(g->length());

            double v = g->genes[pos].d;
            double val = v * (1.0 + stepsize * EAUtilRandom::instance().nrandom());

            g->genes[pos].d = std::min(p->getMaxVal(pos), std::max(p->getMinVal(pos), val));
            ind.touch();
            return ind;
        }

    };

    std::ostream& operator<<(std::ostream& os, const GaussianMutation & op) {
        os << "GaussianMutation(p=" << op.prob << ")";
        return os;
    }

}