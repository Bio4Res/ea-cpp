#pragma once

#include <config/IslandConfiguration.h>

#define SEED    1
#define NUMRUNS 1
#define TOPOLOGY    "EMPTY"

namespace config {

    struct EAConfiguration {
    private:
        int numIslands = 0;
        std::vector<IslandConfiguration> iConf{};
    public:
        uint32_t seed = SEED;
        int numruns = NUMRUNS;
        std::vector<OperatorConfiguration> extendedConfiguration{};
        OperatorConfiguration topology;

        void addIslandConfiguration(IslandConfiguration ic) {
            numIslands += ic.numIslands;
            iConf.push_back(ic);
        }
        void refreshNumIslands() {
            numIslands = 0;
            for (const IslandConfiguration& ic : iConf) {
                numIslands += ic.numIslands;
            }
        }
        int getNumIslands() const {
            return numIslands;
        }

        IslandConfiguration getIslandConfiguration(int iID) {
            IslandConfiguration ic{};
            if ((iID >= 0) && (iID < numIslands)) {
                int c = 0;
                do {
                    ic = iConf[c];
                    iID -= ic.numIslands;
                    c++;
                } while (iID >= 0);
            }
            return ic;
        }

        auto getExtendedConfigurationValue(std::string name) {
            //String key = name.toLowerCase();
            for (auto& opc : extendedConfiguration) {
                if (opc.name == name)
                    return opc.params;
            }
            return std::vector<std::string>{};
        }

        friend struct std::formatter<config::EAConfiguration>;
    };


    void from_json(const json& j, EAConfiguration& c) {
        j.at("seed").get_to(c.seed);
        j.at("numruns").get_to(c.numruns);

        if (j.contains("topology"))
            j.at("topology").get_to(c.topology);
        else
            c.topology.name = "empty";
        if (j.contains("extended")) {
            j.at("extended").get_to<std::vector<OperatorConfiguration>>(c.extendedConfiguration);
        }

        if (j.contains("islands")) {
            auto islandconfv = j.at("islands").get<std::vector<IslandConfiguration>>();
            for (auto& islandconf : islandconfv)
                c.addIslandConfiguration(std::move(islandconf));
        }
        else
            c.addIslandConfiguration(IslandConfiguration{});

    }
}

template <>
struct std::formatter<config::EAConfiguration> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const config::EAConfiguration& c, std::format_context& ctx) const {
        auto out = ctx.out();

        out = std::format_to(
            out,
            "numruns: {}\nseed: {}\n#islands: {}\ntopology: {} ",
            c.numruns,
            c.seed,
            c.getNumIslands(),
            c.topology.name
        );

        for (const auto& p : c.topology.params) {
            out = std::format_to(out, "{} ", p);
        }

        if (!c.extendedConfiguration.empty()) {
            out = std::format_to(out, "\nextended:\n");
            for (const auto& opc : c.extendedConfiguration) {
                out = std::format_to(out, "\tname: {} ( ", opc.name);
                for (const auto& p : opc.params) {
                    out = std::format_to(out, "{} ", p);
                }
                out = std::format_to(out, ")\n");
            }
        }

        int k = 1;
        for (const auto& ic : c.iConf) {
            out = std::format_to(
                out,
                "Island configuration #{}\n\t{}\n",
                k++,
                ic
            );
        }

        return out;
    }
};
