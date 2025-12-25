#pragma once

#include <operator/variation/initialization/InitializationOperator.h>
#include <fitness/ContinuousObjectiveFunction.h>

namespace ea {

    struct RandomVector : public InitializationOperator {
        /**
         * Generates the operator
         * @param pars parameters (none)
         */
        RandomVector(const std::vector<std::string>& pars) : InitializationOperator(pars) {
        }

        friend std::ostream& operator<<(std::ostream& os, const RandomVector& op);
    protected:
        Individual apply_(const individuals_v& parents) override {
            int l = obj->numvars;
            auto f = std::dynamic_pointer_cast<ContinuousObjectiveFunction>(obj);
            auto g = std::make_unique<Genotype>(l, GeneType::DOUBLE);
            for (int i = 0; i < l; i++) {
                double minv = f->getMinVal(i);
                double maxv = f->getMaxVal(i);
                g->genes[i].d = minv + EAUtilRandom::instance().random01() * (maxv - minv);
            }
            Individual ind;
            ind.setGenome(std::move(g));
            return ind;
        }

    };

    std::ostream& operator<<(std::ostream& os, const RandomVector& op) {
        os << "RandomVector";
        return os;
    }
}
