#pragma once

#include <memory>
#include <fitness/ObjectiveFunction.h>
#include <operator/replacement/ReplacementOperator.h>
#include <operator/replacement/ReplacementFactory.h>
#include <operator/selection/SelectionOperator.h>
#include <operator/selection/SelectionFactory.h>
#include <operator/variation/VariationOperator.h>
#include <operator/variation/VariationFactory.h>
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
        std::shared_ptr<ObjectiveFunction> obj{ nullptr };
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
        std::unique_ptr<individuals_v> population;
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
        std::shared_ptr<IslandStatistics> stats = nullptr;
        /**
         * Migration operator
         */
        std::shared_ptr<MigrationOperator> migrate;
        /**
         * Creates an island with a given configuration
         * @param id the island ID
         * @param ic the configuration of the island
         */
        Island(int id, const config::IslandConfiguration& ic) : 
                maxEvals{ ic.maxEvals }, mu{ ic.popSize }, lambda{ ic.numOffspring } {
            this->id = id;
            population = std::make_unique<individuals_v>();
            population->reserve(mu);

            // create operators
            selection = SelectionFactory::create(ic.selection.name, ic.selection.params);
            replace = ReplacementFactory::create(ic.replacement.name, ic.replacement.params);
            size_t numOps = ic.variationOps.size();
            //auto x = variationOps.get_allocator().allocate(numOps);
            variationOps.reserve(numOps);

            initialization = VariationFactory::create(ic.variationOps[0].name, ic.variationOps[0].params);
            poolSize = lambda;
            for (int i = 1; i < numOps; i++) {
                auto op = VariationFactory::create(ic.variationOps[i].name, ic.variationOps[i].params);
                poolSize *= op->getArity();
                variationOps.push_back(std::move(op));
            }
            migrate = MigrationFactory::create(ic.frequency,
                ic.numIndividuals,
                ic.migrationOps[0].name, ic.migrationOps[0].params, //outgoing
                ic.migrationOps[1].name, ic.migrationOps[1].params); //incoming
            stats = std::make_shared<IslandStatistics>();
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
        void setObjectiveFunction(std::shared_ptr<ObjectiveFunction> theobjf) {
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
            return population->size();
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

            population->clear();

            auto placeholder = individuals_v{};
            for (int i = 0; i < mu; i++) {
                std::unique_ptr<Individual> ind = std::move(initialization->apply(placeholder));
                obj->evaluate(*ind);
                population->push_back(std::move(ind));
                
            }
            numEvals = mu;
            stats->takeStats(obj->getEvals(), *population);
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
                population = std::move(migrate->receive(*population));
                // selection ------------------------------------------------------
                auto offspring = selection->apply(*population, poolSize);
                // reproduction ---------------------------------------------------
                for (auto & op : variationOps) {
                    int a = op->getArity();
                    int m = static_cast<int>(offspring->size()) / a;
                    
                    auto stage = std::make_unique<individuals_v>(m,nullptr); //de m elementos
                    auto parents = std::make_unique<individuals_v>(a,nullptr); //de a elementos
                    
                    int offspring_counter = 0;
                    for (int j = 0; j < m; j++) {
                        for (int i = 0; i < a; i++) {
                            (*parents)[i] = (*offspring)[offspring_counter++];
                        }
                        (*stage)[j] = op->apply(*parents);
                    }
                    offspring = std::move(stage);
                }
                // evaluate new individuals ---------------------------------------
                for (const auto & i : *offspring) {
                    if (!i->isEvaluated())
                        numEvals++;
                    obj->evaluate(*i);
                }
                // apply replacement ----------------------------------------------
                population = replace->apply(*population, *offspring);
                // emigration ----------------------------------------------------
                migrate->send(*population);
                // take stats -----------------------------------------------------
                stats->takeStats(obj->getEvals(), *population);
            }
            return isActive();
        }

        friend std::ostream& operator<<(std::ostream& os, const Island& dt);

        /**
         * Returns the island statistics
         * @return the island statistics
         */
        std::shared_ptr<IslandStatistics> getStatistics() {
            return stats;
        }
    };

    std::ostream& operator<<(std::ostream& os, const Island& island){
        os << "{\n\tnumevals: " << island.numEvals << ",\n\tpopulation: [\n" <<
        "\t\t" << island.population->at(0); // .replace("\n", "\n\t\t");
        for (int i = 1; i < island.mu; i++)
            os << ",\n\t\t" << island.population->at(i); // .replace("\n", "\n\t\t");
        os << "\n\t]\n}";
        return os;
    }

}

