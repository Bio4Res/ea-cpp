#pragma once

#include <config/OperatorConfiguration.h>

#define NUMISLANDS 1
#define POPSIZE 100
#define MAXEVALS 10000
#define INITIALIZATION "BITSTRING"
#define SELECTION  "TOURNAMENT"
#define SELECTION_PARAMS "2"
#define REPLACEMENT "COMMA"
#define NUMOFFSPRING (POPSIZE - 1)
#define RECOMBINATION "SPX"
#define RECOMBINATION_PARAMS "0.9"
#define MUTATION "BITFLIP"
#define MUTATION_PARAMS "0.01"
#define FREQUENCY 10
#define INDIVIDUALS 0

using json = nlohmann::json;

namespace config{

    struct IslandConfiguration {
        /**
         * number of islands with this configuration
         */
        int numIslands = NUMISLANDS;
        /**
         * population size
         */
        int popSize = POPSIZE;
        /**
         * maximum number of evaluations
         */
        long maxEvals = MAXEVALS;
        /**
         * number of offspring generated in each iteration
         */
        int numOffspring = NUMOFFSPRING;
        /**
         * selection operator used
         */
        OperatorConfiguration selection{};
        /**
         * replacement operator used
         */
        OperatorConfiguration replacement{};
        /**
         * List of variation operators and their parameters
         */
        std::vector<OperatorConfiguration> variationOps{};
        /**
         * frequency among migrations
         */
        int frequency = FREQUENCY;
        /**
         * number of individuals to migrate
         */
        int numIndividuals = INDIVIDUALS;
        /**
         * In/out configuration of migration
         */
        std::vector<OperatorConfiguration> migrationOps{ {}, {} }; //JES: two default elements. Should it be an array?

        /**
         * List of additional non-standard configuration settings used in specific island models
         */
        std::vector<OperatorConfiguration> extendedConfiguration{};

        std::string toString() const {
            std::string str = "islands: " + std::to_string(numIslands) + "\n" +
                "popsize: " + std::to_string(popSize) + "\n" +
                "offspring: " + std::to_string(numOffspring) + "\n" +
                "maxevals: " + std::to_string(maxEvals) + "\n" +
                "initialization: " + variationOps[0].name;
            if (variationOps[0].params.size() > 0) {
                str += " (";
                std::for_each(variationOps[0].params.begin(), variationOps[0].params.end(),
                    [&str](const auto& elem) { str += elem + " "; });
                str += ")";
            }
            str += "\nselection: " + selection.name;
            if (selection.params.size() > 0) {
                str += " (";
                std::for_each(selection.params.begin(), selection.params.end(),
                    [&str](const auto& elem) { str += elem + " "; });
                str += ")";

            }

            str += "\n";
            size_t n = variationOps.size();
            for (size_t i = 1; i < n; i++) {
                str += "variation: " + variationOps[i].name + " (";
                std::for_each(variationOps[i].params.begin(), variationOps[i].params.end(),
                    [&str](const auto& elem) { str += elem + " "; });
                str += ")\n";
            }
            str += "replacement: " + replacement.name;
            if (replacement.params.size() > 0) {
                str += " (";
                std::for_each(replacement.params.begin(), replacement.params.end(),
                    [&str](const auto& elem) { str += elem + " "; });
                str += ")";
            }
            str += "\n";
            if ((migrationOps[0].name != "") || (migrationOps[1].name != "")) {
                str += "migration : ";
                if (migrationOps[0].name != "") {
                    str += migrationOps[0].name;
                    if (migrationOps[0].params.size() > 0) {
                        str += " (";
                        std::for_each(migrationOps[0].params.begin(), migrationOps[0].params.end(),
                            [&str](const auto& elem) { str += elem + " "; });
                        str += ")";
                    }
                    str += " [freq=" + std::to_string(frequency) + ", num=" + std::to_string(numIndividuals) + "] + ";
                }
                else
                    str += "null + ";
                if (migrationOps[1].name != "") {
                    str += migrationOps[1].name;
                    if (migrationOps[1].params.size() > 0) {
                        str += " (";
                        std::for_each(migrationOps[1].params.begin(), migrationOps[1].params.end(),
                            [&str](const auto& elem) { str += elem + " "; });
                        str += ")";
                    }
                }
                else
                    str += "null";
            }
            return str;
        }
    };


    void from_json(const json& j, IslandConfiguration& c) {
        j.at("numislands").get_to(c.numIslands);
        j.at("popsize").get_to(c.popSize);
        j.at("offspring").get_to(c.numOffspring);
        j.at("maxevals").get_to(c.maxEvals);


        if (j.contains("selection")) {
            j.at("selection").get_to(c.selection);
        }
        else {
            c.selection.name = SELECTION;
            c.selection.params.push_back(SELECTION_PARAMS);
        }

        if (j.contains("replacement")) {
            j.at("replacement").get_to(c.replacement);
        }
        else {
            c.replacement.name = REPLACEMENT;
        }

        if (j.contains("initialization")) {
            c.variationOps.push_back(j.at("initialization").get<OperatorConfiguration>());
        }
        else {
            c.variationOps.push_back({ INITIALIZATION,{} });
        }

        if (j.contains("variation")) {
            std::vector<OperatorConfiguration> opv = j.at("variation").get<std::vector<OperatorConfiguration>>();
            size_t opvsize = opv.size();
            for (size_t i = 0; i < opvsize; ++i)
                c.variationOps.push_back(std::move(opv[i]));
        }
        else {
            c.variationOps.push_back({ RECOMBINATION,{RECOMBINATION_PARAMS} });
            c.variationOps.push_back({ MUTATION,{MUTATION_PARAMS} });
        }

        if (j.contains("extended")) {
            j.at("extended").get_to<std::vector<OperatorConfiguration>>(c.extendedConfiguration);
        }

        if (j.contains("migration")) {
            json migrat = j.at("migration");
            if (migrat.contains("frequency"))
                migrat.at("frequency").get_to(c.frequency);
            if (migrat.contains("individuals"))
                migrat.at("individuals").get_to(c.numIndividuals);
            if (migrat.contains("send"))
                migrat.at("send").get_to(c.migrationOps[0]);
            if (migrat.contains("receive"))
                migrat.at("receive").get_to(c.migrationOps[1]);
        }
    }

}