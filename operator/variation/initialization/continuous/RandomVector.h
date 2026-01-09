#pragma once

#include <operator/variation/initialization/InitializationOperator.h>
#include <fitness/ContinuousObjectiveFunction.h>

namespace ea {

    struct RandomVector : public InitializationOperator {
    private:
        ContinuousObjectiveFunction * continuous_obj;
    public:
        RandomVector(const std::vector<std::string>& pars) : InitializationOperator(pars) {}
        
        void setObjectiveFunction(ObjectiveFunction* obj) override {
            InitializationOperator::setObjectiveFunction(obj);
            continuous_obj = dynamic_cast<ContinuousObjectiveFunction*>(obj);
            if (!continuous_obj) {
                throw std::runtime_error("This operator requires a ContinuousObjectiveFunction");
            }
        }

    protected:
        Individual apply_(const individuals_v& parents) override {
            int l = obj->numvars;
            auto g = std::make_unique<Genotype>(l, GeneType::DOUBLE);
            for (int i = 0; i < l; i++) {
                double minv = continuous_obj->getMinVal(i);
                double maxv = continuous_obj->getMaxVal(i);
                g->genes[i].d = minv + EAUtilRandom::instance().random01() * (maxv - minv);
            }
            Individual ind;
            ind.setGenome(std::move(g));
            return ind;
        }

    };

}
