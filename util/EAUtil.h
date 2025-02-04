#pragma once

#include <random>

namespace ea {

    class EAUtilRandom final{
    private:
        EAUtilRandom() = default;
        int seed = 1;
        std::mt19937 generator;
        std::uniform_int_distribution<int> distribution_i;
        std::uniform_real_distribution<double> distribution_r;
        std::normal_distribution<double> distribution_n;
    public:
        void setSeed(long n) {
            seed = n;
            generator.seed(n); 
        }
        int getSeed() const {
            return seed;
        }
        int random(const int bound) {
            using param_type = std::uniform_int_distribution<int>::param_type;
            return distribution_i(generator, param_type{ 0, bound - 1 });
        }

        int random(const size_t bound) {
            using param_type = std::uniform_int_distribution<int>::param_type;
            return distribution_i(generator, param_type{ 0, static_cast<int>(bound) - 1 });
        }



        /**
        * Returns a double number between 0.0 (inc) and 1.0 (exc)
        * @return a double number between 0.0 (inc) and 1.0 (exc)
        */
        double random01() {
            return distribution_r(generator); 
        }

        /**
        * Returns a normally distributed random number (mean = 0, var = 1)
        * @return a normally distributed random number
        */
        double nrandom() {
            return distribution_n(generator);
        }

        static EAUtilRandom & instance(){
            static EAUtilRandom the_instance; //Since C++11. Scott Meyer, you are a god among humans!
            return the_instance;
        }

    };

}