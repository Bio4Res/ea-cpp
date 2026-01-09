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
        individuals_dq buffer;
    public:
        IslandBuffer() = default;

        /**
         * Clears the buffer
         */
        void newRun() {
            buffer.clear();
        }
        /**
         * Returns the size of the buffer
         * @return the size of the buffer
         */
        std::size_t size() const {
            return buffer.size();
        }

        /**
         * Checks if buffer is empty
         */
        bool empty() const {
            return buffer.empty();
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
            buffer.push_back(ind);
            return true;
        }
        
        bool add(Individual&& ind) {
            buffer.push_back(std::move(ind));
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
        bool add(individuals_v && pop) {            
            for (auto& ind : pop) {
                buffer.push_back(std::move(ind));
            }
            return true;
        }
        /**
         * Extracts the oldest individual in the buffer.
         * Subsequently, there will be one individual less
         * in the buffer.
         * @return the oldest individual in the buffer
         */
        Individual get() {
            assert(buffer.size() > 0);
            Individual ind = std::move(buffer.front());
            buffer.pop_front();
            return ind;
        }

        /**
         * Returns all individuals in the buffer. Individuals are extracted and therefore
         * the buffer is subsequently empty afterwards.
         * @return a list of all individuals in the buffer
         */
        std::unique_ptr<individuals_v> getAll() {
            auto migrants = std::make_unique<individuals_v>();
            migrants->reserve(buffer.size());
            
            for (auto & ind : buffer) {
                migrants->push_back(std::move(ind));
            }
            
            buffer.clear();
            return migrants;
        }

    };

}