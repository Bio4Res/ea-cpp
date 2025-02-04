#pragma once

#include <memory>
#include <operator/migration/MigrationOperator.h>
#include <operator/selection/SelectionFactory.h>
#include <operator/replacement/ReplacementFactory.h>

namespace ea {
    struct MigrationFactory {

        /**
         * Creates a migration operator given its parameters
         * @param freq frequency of migration (iterations)
         * @param num number of individuals to migrate
         * @param outgoing name of the selection procedure for selecting migrants to send
         * @param outPars parameters (if any) of the selection method
         * @param incoming name of the replacement procedure to receive migrants
         * @param inPars parameters of the replacement method
         * @return the so-configured migration operator
         */
        static std::unique_ptr<MigrationOperator> create(int freq, int num, const std::string& outgoing, const std::vector<std::string>& outPars, const std::string& incoming, const std::vector<std::string>& inPars) {
            std::shared_ptr<SelectionOperator> sel = (outgoing != "") ? SelectionFactory::create(outgoing, outPars) : nullptr;
            std::shared_ptr<ReplacementOperator> rep = (incoming != "") ? ReplacementFactory::create(incoming, inPars) : nullptr;

            return std::make_unique<MigrationOperator>(sel, rep, freq, num);
        }

    };

}