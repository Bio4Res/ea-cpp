#pragma once

#include <memory>
#include <operator/replacement/ReplacementOperator.h>
#include <operator/replacement/CommaReplacement.h>
#include <util/enumutils.h>

namespace ea {
    ENUM(enumReplacementType, int, PLUS, COMMA, UNRELIABLE, STRATIFIED, EDA)

        struct ReplacementFactory {
        /**
         * Returns a replacement operator given its name.
         * If the name does not correspond to any known operator,
         * it returns null.
         * @param name the name of the replacement operator
         * @param pars parameters of the replacement operator
         * @return the replacement operator named
         */
        static std::unique_ptr<ReplacementOperator> create(const std::string& name, const std::vector<std::string>& pars) {
            //JES: FIX ME Implementa todos!!!
            //switch (name.toUpperCase()) {
            switch (enumReplacementType::_from_string_nocase(name.c_str())) {
                /*
                            case enumReplacementType::PLUS:
                                op = new PlusReplacement();
                                break;
                */
            case enumReplacementType::COMMA:
                return std::make_unique<CommaReplacement>();
                break;
                /*            case enumReplacementType::UNRELIABLE:
                                op = new UnreliableReplacer(pars);
                                break;
                            case enumReplacementType::STRATIFIED:
                                op = new StratifiedReplacerHandler(pars);
                                break;
                            case enumReplacementType::EDA:
                                op = new EDAReplacement(pars);
                                break;
                */
            default:
                break;
            }
            return nullptr;
        }
    };

}