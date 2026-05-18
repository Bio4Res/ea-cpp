#pragma once

#include <operator/Operator.h>

namespace ea {

    struct SelectionOperator : public Operator {
        virtual individuals_v apply(individuals_v& population, const int& num) = 0;
    };

    template<typename T>
    concept SelectionOp = requires(T& op, individuals_v& pop, int n) {
        { op.apply(pop, n) } -> std::same_as<individuals_v>;
    };

}