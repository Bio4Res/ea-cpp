#pragma once

#include <algorithm>
#include <operator/variation/recombination/RecombinationOperator.h>
#include <fitness/ContinuousObjectiveFunction.h>

namespace ea {

    struct BLX : public RecombinationOperator {
    private:
        double alpha;
        ContinuousObjectiveFunction * continuous_obj;
    public:
        /**
         * Generates the operator
         * @param pars parameters
         */
        BLX(const std::vector<std::string>& pars) : RecombinationOperator(pars) {
            alpha = std::stod(pars[1]);
        }

        constexpr int getArity() override{
            return 2;
        }

        void setObjectiveFunction(ObjectiveFunction* obj) override {
            RecombinationOperator::setObjectiveFunction(obj);
            continuous_obj = dynamic_cast<ContinuousObjectiveFunction*>(obj);
            if (!continuous_obj) {
                throw std::runtime_error("This operator requires a ContinuousObjectiveFunction");
            }
        }

    protected:
        Individual apply_(const individuals_v& parents) override {
            const auto father = parents[0].getGenome();
            const auto mother = parents[1].getGenome();
            size_t l = father->length();
            std::unique_ptr<Genotype> offspring = std::make_unique<Genotype>(l, GeneType::DOUBLE);

            for (size_t i = 0; i < l; ++i) {
                double fg = father->genes[i].d;
                double mg = mother->genes[i].d;
                double minval = std::min(fg, mg);
                double maxval = std::max(fg, mg);
                double interval = maxval - minval;
                double explore = alpha * interval;
                double val = minval - explore + EAUtilRandom::instance().random01() * (interval + 2.0 * explore);
                offspring->genes[i].d = std::min(continuous_obj->getMaxVal(i), std::max(continuous_obj->getMinVal(i), val));
            }
            Individual ind;
            ind.setGenome(std::move(offspring));
            return ind;
        }
    

    };

}
