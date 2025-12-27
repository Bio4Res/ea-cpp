#pragma once

#include <vector>
#include <base/Island.h>
#include <statistics/EAStatistics.h>
#include <operator/migration/Topology.h>
#include <operator/migration/TopologyFactory.h>
#include <config/EAConfiguration.h>

namespace ea {

    struct EvolutionaryAlgorithm {
        /**
        * islands in the EA
        */
        std::vector<std::shared_ptr<Island>> islands;
        /**
         * List of active islands
         */
        std::deque<std::shared_ptr<Island>> active;
        /**
         * List of inactive islands
         */
        std::deque<std::shared_ptr<Island>> inactive;
        /**
         * the objective function
         */
        std::unique_ptr<ObjectiveFunction> obj{nullptr};
        /**
         * current seed for the RNG
         */
        long seed;
        /**
         * base seed for each batch of runs
         */
        long baseSeed;
        /**
         * statistics of the EA
         */
        EAStatistics stats;
        /**
         * island interconnection topology
         */
        std::unique_ptr<Topology> topology;
        /**
         * number of runs
         */
        int numruns;
        /**
         * configuration used in the EA
         */
        config::EAConfiguration& configuration;
        /**
         * Creates an EA given a certain configuration
         * @param conf configuration of the EA
         */
        EvolutionaryAlgorithm(config::EAConfiguration& conf) : islands(conf.getNumIslands()), stats(islands), configuration(conf) {
            obj = nullptr;
            int n = conf.getNumIslands();
            //islands.reserve(n);
            for (int i = 0; i < n; i++) {
                islands[i] = std::make_shared<Island>(i, conf.getIslandConfiguration(i));
            }
            baseSeed = conf.seed;
            seed = baseSeed;
            numruns = conf.numruns;
            topology = nullptr;
            conf.topology.params.insert(conf.topology.params.begin(), std::to_string(islands.size()));
            stats.clear();
        }

        ~EvolutionaryAlgorithm() {
            //JES CHECK ME: topology y stats se borran por ser unique_ptr 
            //std::for_each(islands.begin(), islands.end(), [](auto* elem) { delete elem; });
        }

        /**
         * Sets the objective function used by the EA
         * @param obj the objective function used by the EA
         */
        void setObjectiveFunction(std::unique_ptr<ObjectiveFunction> theobjf) {
            this->obj = std::move(theobjf);
            for (auto& i : islands)
                i->setObjectiveFunction(obj.get());
            stats.setComparator(obj->getComparator());
        }

        /**
         * Returns the island whose id is given
         * @param id id of an island
         * @return the island whose id is given
         */
        Island & getIslandByID(int id) {
            for (auto & i : islands) {
                if (i->getID() == id)
                    return *i;
            }
            //assert(false);
            throw std::runtime_error("No Island with provided id");
        }

        /**
         * Initializes all islands and starts statistical recording
         */
        void initEA() {
            EAUtilRandom::instance().setSeed(seed++);
            stats.newRun();
            obj->newRun();

            if ((topology == nullptr) || (topology->isRegenerable())) {
                topology = TopologyFactory::create(configuration.topology.name, configuration.topology.params);
                for (const auto& i : islands) {
                    i->resetConnections();
                    auto links = topology->get(i->getID());
                    for (const int& id : *links)
                        i->connect(getIslandByID(id));
                }
            }


            for (const auto& i : islands) {
                i->initializeIsland();
                if (i->isActive())
                    active.push_back(i);
                else
                    inactive.push_back(i);
            }
        }

        /**
         * Performs an evolutionary iteration on all active islands.
         * If all islands become inactive, statistics are closed for that run
         * @return true if some island(s) remain(s) active
         */
        bool stepUp() {
            for (int k = 0; k < (int)active.size(); ) {
                const auto& i = active[k];
                if (!i->stepUp()) {
                    active.erase(active.begin() + k);
                    inactive.push_back(i);
                }
                else
                    k++;
            }

            if (active.size() == 0) {
                stats.closeRun();
                return false;
            }

            return true;
        }
        /**
         * Runs the EA
         */
        void run() {
            initEA();
            while (stepUp());
        }

        /**
         * Performs all runs of the EA indicated in the configuration,
         * starting with the seed indicated in such configuration.
         */
        void runAll() {
            seed = baseSeed;
            for (int i = 0; i < numruns; i++)
                run();
        }
    };

}