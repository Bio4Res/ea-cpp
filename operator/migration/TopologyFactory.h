#pragma once

#include <util/enumutils.h>
#include <memory>
#include <operator/migration/Topology.h>

namespace ea {

    ENUM(enumTopologyType, int, RING, COMPLETE, EMPTY)

        struct TopologyFactory {

        /**
         * Creates a topology given its name and parameters. The topology is expressed
         * as a mapping from integers (the id of an island) to sets of integers (the
         * ids of islands it is connected to).
         * @param name name of the topology
         * @param parameters parameters of the topology. There is at least one parameter (the 1st one): the number of islands
         * @return the topology created
         */
        static std::unique_ptr<Topology> create(std::string name, std::vector<std::string> parameters) {
            int n = std::stoi(parameters[0]);
            std::unique_ptr<Topology> topology = std::make_unique<Topology>(n);
            //JES FILTRO:
            if (name == "") {
                topology->setRegenerable(false);
                return topology;
            }

            switch (enumTopologyType::_from_string_nocase(name.c_str())) {
            case enumTopologyType::RING: // bidirectional ring
                for (int i = 0; i < n; i++) {
                    topology->add(i, (i + 1) % n);
                    topology->add((i + 1) % n, i);
                }
                topology->setRegenerable(false);
                break;
            case enumTopologyType::COMPLETE: // complete graph
                for (int i = 0; i < n; i++)
                    for (int j = i + 1; j < n; j++) {
                        topology->add(i, j);
                        topology->add(j, i);
                    }
                topology->setRegenerable(false);
                break;

            case enumTopologyType::EMPTY: // default topology: no connections
            default:
                topology->setRegenerable(false);
                break;
            }
            return topology;
        }


    };

}