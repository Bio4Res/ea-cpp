#pragma once

#include <unordered_map>
#include <memory>
#include <statistics/DiversityMeasure.h>
#include <base/Individual.h>

namespace ea {

    struct EntropyDiversity : public DiversityMeasure {

        double apply(const individuals_v & pop) override {
            size_t n = pop[0]->getGenome()->length();
            std::vector<std::unordered_map<int, int>> freq(n);
            for (size_t i = 0; i < n; i++)
                freq[i] = std::unordered_map<int, int>();
            for (const auto& ind : pop) {
                auto g = ind->getGenome();
                for (size_t i = 0; i < n; i++) {
                    int v = std::get<int>(g->genes[i]);
                    auto& map = freq[i];
                    if (map.contains(v)) {
                        map[v] = (map[v] + 1);
                    }
                    else {
                        map[v] = 1;
                    }
                }
            }
            double h = 0.0;
            for (size_t i = 0; i < n; i++) {
                double lh = 0.0;
                auto& map = freq[i];
                if (map.size() > 1) {
                    for (const auto& [key, value] : map) {
                        double p = (double)value / (double)pop.size();
                        lh -= p * std::log(p);
                    }
                }
                h += lh;
            }
            return h;
        }


    };



}