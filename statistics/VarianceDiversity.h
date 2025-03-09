#pragma once

#include <statistics/DiversityMeasure.h>

namespace ea {
	struct VarianceDiversity : public DiversityMeasure {

		double apply(const individuals_v & pop) override{
			size_t n = pop.at(0)->getGenome()->length();
			int mu = static_cast<int>(pop.size());

			double sigma = 0;
			for (size_t i = 0; i < n; i++) {
				double mean = 0.0;
				double mean2 = 0.0;
				double K = std::get<double>(pop.at(0)->getGenome()->genes[i]);

				for (const auto & ind : pop) {
					mean += std::get<double>(ind->getGenome()->genes[i]) - K;
					mean2 += std::pow(std::get<double>(ind->getGenome()->genes[i]) - K, 2);
				}
				sigma += std::sqrt((mean2 - (mean * mean) / mu) / mu);
			}
			return sigma / n;
		}

	};

}