#pragma once

#include <memory>
#include <operator/selection/SelectionOperator.h>
#include <operator/selection/TournamentSelection.h>
#include <util/enumutils.h>

namespace ea {
    ENUM(enumSelectionType, int, TOURNAMENT, BEST, RANDOM, ROULETTE, RANKING, UNRELIABLE)


        struct SelectionFactory {
        /**
         * Returns a selection operator given its name.
         * If the name does not correspond to any known operator,
         * it returns null.
         * @param name the name of the selection operator
         * @param pars parameters of the selection operator
         * @return the selection operator named
         */
        static std::unique_ptr<SelectionOperator> create(const std::string& name, const std::vector<std::string>& pars) {

            //JES FIX ME: completar todos los operadores!            
            //switch (name.toUpperCase()) {
            switch (enumSelectionType::_from_string_nocase(name.c_str())) {

            case enumSelectionType::TOURNAMENT:
                return std::make_unique<TournamentSelection>(pars);
                break;
                /*
                            case "BEST":
                                op = new BestSelection();
                                break;

                            case "RANDOM":
                                op = new RandomSelection();
                                break;

                            case "ROULETTE":
                                op = new RouletteWheelSelection();
                                break;

                            case "RANKING":
                                op = new LinearRanking(pars);
                                break;

                            case "UNRELIABLE":
                                op = new UnreliableSelector(pars);
                                break;
                */
            default:
                break;
            }
            return nullptr;
        }
    };

}