#pragma once

#include <statistics/DiversityMeasure.h>

namespace ea {
	struct VarianceDiversity : public DiversityMeasure {

		double apply(const individuals_v & pop) override{
			size_t n = pop.at(0).getGenome()->length();
			int mu = static_cast<int>(pop.size());

			double sigma = 0;
			for (size_t i = 0; i < n; i++) {
				double mean = 0.0;
				double mean2 = 0.0;
				double K = pop.at(0).getGenome()->genes[i].d;

				for (const auto & ind : pop) {
					mean += ind.getGenome()->genes[i].d - K;
					mean2 += std::pow(ind.getGenome()->genes[i].d - K, 2);
				}
				sigma += std::sqrt((mean2 - (mean * mean) / mu) / mu);
			}
			return sigma / n;
		}

	};

}