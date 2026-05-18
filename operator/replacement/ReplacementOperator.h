#pragma once

#include <operator/Operator.h>

namespace ea {

    struct ReplacementOperator : public Operator {
        virtual individuals_v apply(individuals_v&& population, individuals_v&& offspring) = 0;
    };

    template<typename T>
    concept ReplacementOp = requires(T& op, individuals_v pop, individuals_v offspring) {
        { op.apply(std::move(pop), std::move(offspring)) } -> std::same_as<individuals_v>;
    };

}