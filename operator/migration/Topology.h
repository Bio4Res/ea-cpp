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
        std::unordered_map<int, std::unordered_set<int> > connections{};
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
                connections[i] = std::unordered_set<int>();
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
            connections[origin].insert(destination);
        }

        /**
         * Removes a connection to the topology
         * @param origin island sending individuals
         * @param destination island receiving individuals
         */
        void remove(int origin, int destination) {
            if (connections.contains(origin))
                connections[origin].erase(destination);
        }

        /**
         * Gets the connections from an island
         * @param origin the id of the island
         * @return a set with the ids of the islands that origin can send to.
         */
        const std::unordered_set<int>& get(int origin) {
             if (!connections.contains(origin))
                connections[origin] = std::unordered_set<int>();
            return connections[origin];
        }

        friend struct std::formatter<ea::Topology>;
    };

}

template <>
struct std::formatter<ea::Topology> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const ea::Topology& t, std::format_context& ctx) const {
        auto out = ctx.out();

        out = std::format_to(out, "topology: ");

        for (const auto& [k, v] : t.connections) {
            out = std::format_to(out, "<{}: ", k);
            for (const auto& i : v) {
                out = std::format_to(out, "{} ", i);
            }
            out = std::format_to(out, ">");
        }

        return out;
    }
};
