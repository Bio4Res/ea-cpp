#pragma once

#include <memory>
#include <fitness/ObjectiveFunction.h>
#include <operator/replacement/ReplacementOperator.h>
#include <operator/replacement/CommaReplacement.h>
#include <operator/selection/SelectionOperator.h>
#include <operator/selection/TournamentSelection.h>
#include <operator/variation/VariationOperator.h>
#include <operator/variation/mutation/discrete/BitFlip.h>
#include <operator/variation/mutation/continuous/GaussianMutation.h>
#include <operator/variation/recombination/continuous/BLX.h>
#include <operator/variation/recombination/discrete/SinglePointCrossover.h>
#include <operator/variation/recombination/discrete/UniformCrossover.h>
#include <operator/variation/initialization/discrete/RandomBitString.h>
#include <operator/variation/initialization/continuous/RandomVector.h>
#include <statistics/IslandStatistics.h>
#include <operator/migration/MigrationOperator.h>
#include <operator/migration/MigrationFactory.h>
#include <config/IslandConfiguration.h>
#include <iostream>

namespace ea {

    struct Island {
        /**
         * ID of the island
         */
        int id;
        /**
         * population size
         */
        const int mu;
        /**
         * number of offspring
         */
        const int lambda;
        /**
         * The objective function being optimized
         */
        ObjectiveFunction * obj{ nullptr };
        /**
         * number of evaluations so far in the current island
         */
        long numEvals = 0;
        /**
         * maximum number of evaluations
         */
        long maxEvals;
        /**
         * the population
         */
        individuals_v population;
        /**
         * the replacement operator
         */
        std::unique_ptr<ReplacementOperator> replace;
        /**
         * selection operator used
         */
        std::unique_ptr<SelectionOperator> selection;
        /**
         * Initialization operator
         */
        std::unique_ptr<VariationOperator> initialization;
        // technically, it is an InitializationOperator (which is a derived
        // subclass from VariationOperator), but for simplicity it is defined like that.
        /**
         * variation operators used
         */
        std::vector<std::unique_ptr<VariationOperator>> variationOps;
        /**
         * number of individuals to be initially selected to create the offspring
         */
        int poolSize;
        /**
         * statistics
         */
        IslandStatistics stats;
        /**
         * Migration operator
         */
        std::unique_ptr<MigrationOperator> migrate;
        /**
         * Reusable buffers to avoid per-iteration heap allocation in stepUp()
         */
        individuals_v offspringBuffer;
        individuals_v stageBuffer;
        individuals_v parentsBuffer;
        /**
         * Creates an island with a given configuration
         * @param id the island ID
         * @param ic the configuration of the island
         */
        Island(int id, const config::IslandConfiguration& ic) : 
                mu{ ic.popSize }, lambda{ ic.numOffspring }, maxEvals{ ic.maxEvals }{
            this->id = id;
            population.reserve(mu);

            // create operators
            selection = AutoRegistry<SelectionOperator>::create(ic.selection.name, ic.selection.params);
            replace = AutoRegistry<ReplacementOperator>::create(ic.replacement.name, ic.replacement.params);
            size_t numOps = ic.variationOps.size();
            variationOps.reserve(numOps);

            initialization = AutoRegistry<VariationOperator>::create(ic.variationOps[0].name, ic.variationOps[0].params);
            poolSize = lambda;
            for (size_t i = 1; i < numOps; i++) {
                auto op = AutoRegistry<VariationOperator>::create(ic.variationOps[i].name, ic.variationOps[i].params);
                poolSize *= op->getArity();
                variationOps.push_back(std::move(op));
            }
            migrate = MigrationFactory::create(ic.frequency,
                ic.numIndividuals,
                ic.migrationOps[0].name, ic.migrationOps[0].params, //outgoing
                ic.migrationOps[1].name, ic.migrationOps[1].params); //incoming
        }

        /**
         * Returns the island ID
         * @return the island ID
         */
        int getID() {
            return id;
        }

        /**
         * Sets the objective function to be optimized
         * @param obj the objective function to be optimized
         */
        void setObjectiveFunction(ObjectiveFunction * theobjf) {
            obj = theobjf;
            initialization->setObjectiveFunction(obj);
            selection->setObjectiveFunction(obj);
            for (auto& op : variationOps) {
                op->setObjectiveFunction(obj);
            }
            replace->setObjectiveFunction(obj);
            migrate->setObjectiveFunction(obj);
        }

        /**
         * Returns the number of evaluations so far
         * @return the number of evaluations so far
         */
        long getNumEvaluations() const {
            return numEvals;
        }
        /**
        * Returns the population size
        * @return the population size
        */
        std::size_t getPopulationSize() {
            return population.size();
        }

        /**
         * Adds a unidirectional link from self to the island
         * passed as a parameter.
         * @param island the island to connect to
         */
        void connect(Island& island) {
            migrate->connect(island.getID(), island.getMigrationOp());
        }


        /**
         * Removes all connections
         */
        void resetConnections() {
            migrate->resetConnections();
        }

        /**
         * Returns the migration operator
         * @return the migration operator
         */
        MigrationOperator& getMigrationOp() {
            return *migrate;
        }

        /**
         * Creates the initial population of the island
         */
        void initializeIsland() {
            initialization->newRun();
            selection->newRun();
            for (auto & op : variationOps)
                op->newRun();
            replace->newRun();
            migrate->newRun();

            population.clear();

            auto placeholder = individuals_v{};
            for (int i = 0; i < mu; i++) {
                auto ind = initialization->apply(placeholder);
                obj->evaluate(ind);
                population.push_back(std::move(ind));
                
            }
            numEvals = mu;
            stats.takeStats(obj->getEvals(), population);
        }

        /**
         * Returns true iff the island is active, that is,
         * if the number of calls to the objective function is less than maxEvals;
         * @return true iff the island is active
         */
        bool isActive() {
            return (obj->getEvals() < maxEvals);
        }

        /**
         * Performs an evolutionary cycle on the island
         * @return true if the island remains active
         */
       bool stepUp() {

            if (obj->getEvals() < maxEvals) {
                // immigration ----------------------------------------------------
                population = migrate->receive(std::move(population));
                
                // selection ------------------------------------------------------
                selection->apply(population, poolSize, offspringBuffer);
                auto& offspring = offspringBuffer;
                
                // reproduction ---------------------------------------------------
                for (auto & op : variationOps) {
                    int a = op->getArity();
                    int m = static_cast<int>(offspring.size()) / a;

                    stageBuffer.clear();
                    stageBuffer.reserve(m);

                    parentsBuffer.clear();
                    parentsBuffer.reserve(a);

                    for (int j = 0; j < m; j++) {
                        parentsBuffer.clear();

                        for (int i = 0; i < a; i++) {
                            int idx = j * a + i;
                            parentsBuffer.emplace_back(std::move(offspring[idx]));
                        }

                        stageBuffer.emplace_back(op->apply(parentsBuffer));
                    }

                    offspring.swap(stageBuffer);
                    stageBuffer.clear();
                }
                
                // evaluate new individuals ---------------------------------------
                for (auto & i : offspring) {
                    if (!i.isEvaluated())
                        numEvals++;
                    obj->evaluate(i);
                }
                
                // apply replacement ----------------------------------------------
                population = replace->apply(std::move(population), std::move(offspring));
                
                // emigration ----------------------------------------------------
                migrate->send(population);
                
                // take stats -----------------------------------------------------
                stats.takeStats(obj->getEvals(), population);
            }
            return isActive();
        }

        /**
         * Returns the island statistics
         * @return the island statistics
         */
        IslandStatistics & getStatistics() {
            return stats;
        }
    };

}

