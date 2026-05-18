#pragma once

#include <operator/Operator.h>

namespace ea {

    struct SelectionOperator : public Operator {
        virtual void apply(individuals_v& population, int num, individuals_v& out) = 0;
    };

    template<typename T>
    concept SelectionOp = requires(T& op, individuals_v& pop, int n, individuals_v& out) {
        { op.apply(pop, n, out) } -> std::same_as<void>;
    };

}