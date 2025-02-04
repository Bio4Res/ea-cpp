#pragma once
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace config {
    struct OperatorConfiguration {
        std::string name{ "" };
        std::vector<std::string> params{};
    };

    void from_json(const json & j, OperatorConfiguration& x) {
        j.at("name").get_to(x.name);
        if (j.contains("parameters"))
            j.at("parameters").get_to(x.params);
    }

}