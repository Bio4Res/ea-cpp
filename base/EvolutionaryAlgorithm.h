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
        * Islands in the EA
        */
        std::vector<Island> islands;
        /**
         * The active islands
         */
        std::vector<Island *> active;
        /**
         * The inactive islands
         */
        std::vector<Island *> inactive;
        /**
         * the objective function
         */
        std::unique_ptr<ObjectiveFunction> obj = nullptr;
        /* 
         * diversity measure (when used for statistics)
        */
        std::unique_ptr<DiversityMeasure> diversity_measure  = nullptr;
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
        EvolutionaryAlgorithm(config::EAConfiguration& conf) : stats(islands), configuration(conf) {
            int n = conf.getNumIslands();
            islands.reserve(n);
            active.reserve(n);
            inactive.reserve(n);
            for (int i = 0; i < n; i++) {
                islands.emplace_back(i, conf.getIslandConfiguration(i));
            }
            baseSeed = conf.seed;
            seed = baseSeed;
            numruns = conf.numruns;
            topology = nullptr;
            conf.topology.params.insert(conf.topology.params.begin(), std::to_string(islands.size()));
            stats.clear();
        }

        ~EvolutionaryAlgorithm() {
        }

        /**
         * Sets the objective function used by the EA
         * @param obj the objective function used by the EA
         */
        void setObjectiveFunction(std::unique_ptr<ObjectiveFunction> theobjf) {
            this->obj = std::move(theobjf);
            for (auto& i : islands)
                i.setObjectiveFunction(obj.get());
            stats.setComparator(obj->getComparator());
        }
        /**
         * Sets the diversity measure for all statistics
         */
        void setDiversityMeasure(std::unique_ptr<DiversityMeasure> dm) { 
            diversity_measure = std::move(dm);
            stats.setDiversityMeasure(diversity_measure.get());  // ✅ Pasar raw pointer
        }

        /**
         * Returns the island whose id is given
         * @param id id of an island
         * @return the island whose id is given
         */
        Island & getIslandByID(int id) {
            for (auto & i : islands) {
                if (i.getID() == id)
                    return i;
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
                for (auto & i : islands) {
                    i.resetConnections();
                    auto links = topology->get(i.getID());
                    for (const int& id : links)
                        i.connect(getIslandByID(id));
                }
            }

            active.clear();
            inactive.clear();
            for (auto & i : islands) {
                i.initializeIsland();
                if (i.isActive())
                    active.push_back(&i);
                else
                    inactive.push_back(&i);
            }
        }

        /**
         * Performs an evolutionary iteration on all active islands.
         * If all islands become inactive, statistics are closed for that run
         * @return true if some island(s) remain(s) active
         */
        /*
        bool stepUp() {
            for (size_t k = 0; k < active.size(); ) {
                Island * i = active[k];
                if (!i->stepUp()) {
                    active.erase(active.begin() + k);
                    inactive.push_back(i);
                }
                else
                    k++;
            }

            if (active.empty()) {
                stats.closeRun();
                return false;
            }

            return true;
        }
        */
        bool stepUp() {
            //con stable_partition dejo todos los true a la izquierda de it, y los false a su derecha
            auto it = std::stable_partition(active.begin(), active.end(),
                                            [](Island* i) {
                                                return i->stepUp(); //avance en cada isla
                                            });

            inactive.insert(inactive.end(), it, active.end());
            active.erase(it, active.end());

            if (active.empty()) {
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