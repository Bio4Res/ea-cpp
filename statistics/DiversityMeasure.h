#pragma once

#include <base/Individual.h>

namespace ea {
    struct DiversityMeasure {
        /**
         * Computes a measure of diversity in the population
         * @param pop the population
         * @return a numerical value that measures diversity
         */
        virtual double apply(const individuals_v & pop) = 0;
        virtual ~DiversityMeasure(){}
    };

}
