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
        long seed = SEED;
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

        std::string toString() const {
            std::string result;
            result = "numruns: " + std::to_string(numruns) +
                "\nseed: " + std::to_string(seed) +
                "\n#islands: " + std::to_string(getNumIslands()) +
                "\ntopology: " + topology.name + " ";
            std::for_each(topology.params.cbegin(), topology.params.cend(),
                [&result](const auto& elem) { result += elem + " "; });

            if (extendedConfiguration.size() > 0) {
                result += "\nextended:\n";
                for (const OperatorConfiguration& opc : extendedConfiguration) {
                    result += "\tname: " + opc.name + " ( ";
                    std::for_each(opc.params.begin(), opc.params.end(),
                        [&result](const auto& elem) { result += elem + " "; });
                    result += ")\n";
                }
            }

            int k = 1;
            for (const IslandConfiguration& ic : iConf)
                result += "Island configuration #" + std::to_string(k++) + "\n\t" + ic.toString() + "\n";

            return result;
        }
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