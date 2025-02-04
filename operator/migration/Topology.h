#pragma once

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <string>


namespace ea {
    /**
* Interconnection topology among islands
*/
    struct Topology {
    private:
        /**
         * Mapping from island ID to the set of islands ID the former is conected to
         */
        std::unordered_map<int, std::shared_ptr<std::unordered_set<int>> > connections{};
        /**
         * Whether the topology has a random component during creation that
         * might produce a different result if created again.
         */
        bool regenerable;
    public:
        /**
         * Basic constructor for an empty topology
         */
        Topology() {
            //connections = new HashMap<Integer, Set<Integer>>();
            regenerable = false;
        }

        /**
         * basic constructor for an empty topology with n islands
         * @param n the number of islands
         */
        Topology(int n) {
            //this();
            regenerable = false;

            for (int i = 0; i < n; i++) {
                connections[i] = std::make_shared<std::unordered_set<int>>();
            }
        }


        /**
         * Indicates whether the topology is regenerable, i.e.,
         * whether it has a random component during creation that
         * might produce a different result if created again.
         * @return whether the topology is regenerable
         */
        bool isRegenerable() const {
            return regenerable;
        }


        /**
         * Sets the regenerable status of the topology
         * @param regenerable the regenerable status
         */
        void setRegenerable(bool regener) {
            this->regenerable = regener;
        }


        /**
         * Adds a connection to the topology
         * @param origin island sending individuals
         * @param destination island receiving individuals
         */
        void add(int origin, int destination) {
            /*auto links = connections[origin];
            if (links == nullptr) {
                links = new std::unordered_set<int>;
                connections[origin] = links;
            }
            links->insert(destination);
            */
            if (connections.contains(origin))
                connections[origin]->insert(destination);
            else
                connections.emplace(origin, std::make_shared< std::unordered_set<int>>());

        }

        /**
         * Removes a connection to the topology
         * @param origin island sending individuals
         * @param destination island receiving individuals
         */
        void remove(int origin, int destination) {
            /*auto* links = connections[origin];
            if (links == nullptr) {
                links = new std::unordered_set<int>();
                connections[origin] = links;
            }
            links->extract(destination);
            */
            if (connections.contains(origin))
                connections[origin]->extract(destination);
            /*else //JES CHECK: creo que este else no seria necesario
                connections.emplace(origin, std::make_shared< std::unordered_set<int>>());
            */
        }

        /**
         * Gets the connections from an island
         * @param origin the id of the island
         * @return a list with the ids of the islands that origin can send to.
         */
        auto get(int origin) {
            /*std::unordered_set<int>* links = connections[origin];
            if (links == nullptr) {
                links = new std::unordered_set<int>();
                connections[origin] = links;
            }
            return links;
            */
            if (!connections.contains(origin))
                connections.emplace(origin, std::make_shared< std::unordered_set<int>>());
            return connections[origin];
        }

        std::string toString() {
            std::string str;
            str += "topology: ";
            for (auto& [k, v] : connections) {
                str += "<" + std::to_string(k) + ": ";
                for (auto& i : *v)
                    str += std::to_string(i) + " ";
                str += ">";
            }
            return str;
        }


    };

}