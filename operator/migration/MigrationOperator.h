#pragma once

#include <operator/Operator.h>
#include <operator/selection/SelectionOperator.h>
#include <operator/replacement/ReplacementOperator.h>
#include <operator/migration/IslandBuffer.h>

namespace ea {

    struct MigrationOperator : public Operator {
        /**
         * frequency among migrations
         */
        int frequency;
        /**
         * iterations remaining for nest migration
         */
        int counter;
        /**
         * number of individuals to migrate
         */
        int numIndividuals;
        /**
         * operator to select which individuals to migrate
         */
        std::shared_ptr<SelectionOperator> emigrate;
        /**
         * operator to select how to deal with incoming individuals
         */
        std::shared_ptr<ReplacementOperator> immigrate;
        /**
         * input buffer of the current island
         */
        std::shared_ptr<IslandBuffer> buffer;
        /**
         * list of input buffers from other islands
         */
        std::unordered_map<int, std::shared_ptr<IslandBuffer>> connections;

        /**
         * Creates the operator given its parameters
         * @param out selection operator to pick individuals to migrate
         * @param in replacement operator to determine how to accept incoming individuals
         * @param freq frequency of migration
         * @param num number of individuals to migrate
         */
        MigrationOperator(std::shared_ptr<SelectionOperator> out, std::shared_ptr<ReplacementOperator> in, int freq, int num) : Operator() {
            emigrate = out;
            immigrate = in;
            frequency = freq;
            numIndividuals = num;
            buffer = std::make_shared<IslandBuffer>();
            newRun();
        }

        /**
         * Sets the objective function for potential use inside some operator
         * @param obj the objective function
         */
        void setObjectiveFunction(std::shared_ptr<ObjectiveFunction> obj) {
            Operator::setObjectiveFunction(obj);
            if (emigrate != nullptr)
                emigrate->setObjectiveFunction(obj);
            if (immigrate != nullptr)
                immigrate->setObjectiveFunction(obj);
        }

        /**
         * Adds a unidirectional link from self to the island whose
         * migration operator is passed as a parameter.
         * @param id the id of the island to connect to
         * @param op the migration operator of another island
         */
        void connect(int id, MigrationOperator& op) {
            connections[id] = op.getBuffer();
        }

        /**
         * Removes a connection
         * @param id the id of the island whose connection to is removed
         */
        void removeConnection(int id) {
            connections.extract(id);
        }

        /**
         * Removes all connections
         */
        void resetConnections() {
            connections.clear();
        }

        /**
         * Returns the buffer of the island
         * @return the buffer of the island
         */
        std::shared_ptr<IslandBuffer> getBuffer() {
            return buffer;
        }

        /**
         * Resets the counter to determine when the new migration will take place
         * and empties the island buffer
         */
        void newRun() {
            Operator::newRun();
            buffer->newRun();
            counter = frequency;
        }

        /**
         * Checks whether migration has to be performed. If so, individuals are selected
         * and sent to the islands connected, and true is returned. Otherwise (the time has
         * not come or no migrant selection mechanism was specified when creating the
         * migration operator), it does nothing and returns false.
         * @param population the list of individuals from which migrants will be selected
         * @return true iff migration is performed
         */
        bool send(individuals_v & population) {
            if (emigrate != nullptr) {
                counter--;
                if (counter == 0) {
                    counter = frequency;
                    for (const auto& c : connections) {
                        c.second->add(emigrate->apply(population, numIndividuals));
                    }
                    return true;
                }
            }
            return false;
        }

        /**
         * Checks whether there are individuals in the input buffer, and
         * if so, inserts them into the population. If no insertion mechanism was
         * specified when creating the migration operator, the input population is
         * returned.
         * @param population the list of individuals to which migrants will be inserted
         * @return the updated population
         */
        std::unique_ptr<individuals_v> receive(individuals_v & population) {
            if (immigrate != nullptr) {
                auto all = buffer->getAll();
                return immigrate->apply(population, *all);
            }

            return make_unique<individuals_v>(population);
        }
/*
        std::string toString() override {
            std::string str = "Migration(";
            str += emigrate->toString();
            str += "[" + std::to_string(frequency) + ", " + std::to_string(numIndividuals) + "], ";
            str += immigrate->toString() + ")";
            return str;
        }
*/
        friend std::ostream& operator<<(std::ostream& os, const MigrationOperator& op);

    };

    std::ostream& operator<<(std::ostream& os, const MigrationOperator& op){
        os << "Migration(";
        os <<  op.emigrate;
        os << "[" << op.frequency << ", " << op.numIndividuals << "], ";
        os << op.immigrate << ")";
        return os;
    }
            


}