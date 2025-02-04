#pragma once

#include <memory>
#include <deque>
#include <base/Individual.h>

namespace ea {

    struct IslandBuffer {
    private:
        /**
         * Internal list of individuals
         */
        std::unique_ptr<std::deque<Individual>> buffer;
    public:
        IslandBuffer() {
            buffer = std::make_unique<std::deque<Individual>>();
        }
        /**
         * Clears the buffer
         */
        void newRun() {
            buffer->clear();
        }
        /**
         * Returns the size of the buffer
         * @return the size of the buffer
         */
        std::size_t size() {
            return buffer->size();
        }

        /**
         * Inserts an individual in the buffer. Returns true iff
         * the individual could be inserted. By default it can be
         * inserted, but a derived class might implement a fixed-size
         * buffer.
         * @param ind the individual to be inserted
         * @return true iff the individual could be inserted
         */
        bool add(Individual& ind) {
            buffer->push_back(Individual{ ind });
            return true;
        }

        /**
         * Inserts a collection of individuals in the buffer. Returns true iff
         * the individuals could be inserted. By default they can be
         * inserted, but a derived class might implement a fixed-size
         * buffer.
         * @param pop the individuals to be inserted
         * @return true iff the individuals could be inserted
         */
        bool add(const std::unique_ptr<individuals_v>& pop) {
            bool ok = true;

            buffer->insert(buffer->end(), pop->begin(), pop->end());
            return ok;
        }
        /**
         * Extracts the oldest individual in the buffer.
         * Subsequently, there will be one individual less
         * in the buffer.
         * @return the oldest individual in the buffer
         */
        Individual get() {
            Individual ind;
            if (buffer->size() > 0) {
                ind = buffer->front();
                buffer->pop_front();
            }
            else {
                assert(false);
            }
            return ind;
        }

        /**
         * Returns all individuals in the buffer. Individuals are extracted and therefore
         * the buffer is subsequently empty afterwards.
         * @return a list of all individuals in the buffer
         */
        std::unique_ptr<individuals_v> getAll() {
            auto migrants = std::make_unique<individuals_v>(buffer->begin(), buffer->end());
            buffer->clear();
            return migrants;
            // 
            //std::deque<Individual*>* migrants = new std::deque<Individual*>(std::move(*buffer));
            //List<Individual> migrants = new LinkedList<Individual>(buffer);
            //return migrants;
        }

        std::string toString() {
            std::string str = "Buffer : ";
            for (auto& elem : *buffer) {
                str += elem.toString();
                str += " ";
            }
            return str;
        }
    };


}