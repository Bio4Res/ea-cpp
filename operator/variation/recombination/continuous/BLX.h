#pragma once

#include <algorithm>
#include <operator/variation/recombination/RecombinationOperator.h>
#include <fitness/ContinuousObjectiveFunction.h>

namespace ea {

    struct BLX : public RecombinationOperator {
    private:
        double alpha;
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

        friend std::ostream& operator<<(std::ostream& os, const BLX& op);
    protected:
        std::unique_ptr<Individual> apply_(const individuals_v& parents) override {
            const auto father = parents[0]->getGenome();
            const auto mother = parents[1]->getGenome();
            auto p = std::dynamic_pointer_cast<ContinuousObjectiveFunction>(obj);
            size_t l = father->length();
            std::unique_ptr<Genotype> offspring = std::make_unique<Genotype>(l);

            for (int i = 0; i < l; ++i) {
                double fg = std::get<double>(father->genes[i]);
                double mg = std::get<double>(mother->genes[i]);
                double minval = std::min(fg, mg);
                double maxval = std::max(fg, mg);
                double interval = maxval - minval;
                double explore = alpha * interval;
                double val = minval - explore + EAUtilRandom::instance().random01() * (interval + 2.0 * explore);
                offspring->genes[i] = std::min(p->getMaxVal(i), std::max(p->getMinVal(i), val));
            }
            std::unique_ptr<Individual> ind = std::make_unique<Individual>();
            ind->setGenome(std::move(offspring));
            return ind;
        }

    };

    std::ostream& operator<<(std::ostream& os, const BLX& op) {
        os << "BLX(p=" << op.prob << ", alpha=" << op.alpha << ")";
        return os;
    }
}
