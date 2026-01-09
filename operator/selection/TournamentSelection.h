#pragma once

#include <operator/selection/SelectionOperator.h>
#include <util/EAUtil.h>

namespace ea {

    struct TournamentSelection : public SelectionOperator {
        /**
         * binary tournament by default
         */
        static const int TOURNAMENT_SIZE = 2;
    private:
        /**
         * size of the tournament
         */
        int tournamentSize = TOURNAMENT_SIZE;
    public:
        /**
         * Creates the tournament selection operator with the default tournament size
         */
        TournamentSelection() {
            setTournamentSize(TOURNAMENT_SIZE);
        }

        /**
         * Creates the tournament selection operator for the given tournament size
         * @param tsize tournament size
         */
        TournamentSelection(int tsize) {
            setTournamentSize(tsize);
        }

        /**
         * Creates the operator given its parameter list
         * @param pars tournament size
         */
        TournamentSelection(const std::vector<std::string>& pars) {
            setTournamentSize(std::stoi(pars[0]));
        }

        /**
         * Returns the tournament size
         * @return the tournament size
         */
        int getTournamentSize() const {
            return tournamentSize;
        }

        /**
         * Sets the tournament size
         * @param tournamentSize the tournament size
         */
        void setTournamentSize(int thetournamentSize) {
            tournamentSize = thetournamentSize;
        }

        individuals_v apply(individuals_v & population, const int& num) override {
            individuals_v result;
            result.reserve(num);
            auto mu = population.size();
            for (int i = 0; i < num; i++) {
               int best = EAUtilRandom::instance().random(mu);
               for (int j = 1; j < tournamentSize; j++) {
                    int cand = EAUtilRandom::instance().random(mu);
                    if (comparator(population[cand], population[best]))
                       best = cand;
               }
               result.push_back(population[best]);
            }
            return result;
        }
/*
        std::string toString() {
            return { "Tournament(" + std::to_string(tournamentSize) + ")" };
        }
*/
    };


}