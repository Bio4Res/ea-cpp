#pragma once

#include <iostream>
#include <memory>
#include <base/Genotype.h>
#include <cassert>
#include <deque>

namespace ea {

    struct Individual {
    private:
        /**
         * boolean flag to determine whether the individual has been evaluated
         */
        bool evaluated = false;
    protected:
        /**
         * fitness of the individual
         */
        double fitness = 0.0;

        /**
         * genotype of the individual
         */
        std::unique_ptr<Genotype> genome = nullptr;
    public:
        /**
         * Creates an empty individual
         */
        Individual() {
            touch();
        }

        /**
         * Creates an individual by cloning an existing one
         * @param individual the individual to copy
         */
        Individual(const Individual & individual) noexcept{
            setFitness(individual.fitness);
            setGenome(individual.genome->clone());
            evaluated = individual.evaluated;
        }

        Individual & operator=(const Individual& individual) noexcept{
            setFitness(individual.fitness);
            setGenome(individual.genome->clone());
            evaluated = individual.evaluated;
            return *this;
        }

        Individual(Individual && individual) noexcept{
            setFitness(individual.fitness);
            setGenome(std::move(individual.genome));
            evaluated = individual.evaluated;
        }

        Individual& operator=(Individual&& individual) noexcept{
            setFitness(individual.fitness);
            setGenome(std::move(individual.genome));
            evaluated = individual.evaluated;
            return *this;
        }


        /**
         * Indicates the fitness information of the individual
         * is no longer valid.
         */
        inline void touch() {
            evaluated = false;
        }

        /**
         * Returns whether the individual has been evaluated
         * @return true iff the individual has been evaluated
         */
        bool isEvaluated() const {
            return evaluated;
        }

        /**
         * Returns the genome
         * @return the genome
         */
        Genotype * getGenome() const {
            return genome.get();
        }

        /**
         * Sets the genome
         * @param genome the genome
         */

        void setGenome(std::unique_ptr<Genotype> && thegenome) {
            genome = std::move(thegenome);
            touch();
        }

        /**
         * Returns the fitness of the individual
         * @return the fitness of the individual
         */
        inline double getFitness() const {
            assert(evaluated); // "individual not evaluated";
            return fitness;
        }

        /**
         * Sets the fitness of the individual
         * @param fitness the fitness of the individual
         */
        void setFitness(const double& thefitness) {
            fitness = thefitness;
            evaluated = true;
        }

    };

    using individuals_v = std::vector<Individual>;
    using individuals_dq = std::deque<Individual>;

    auto maxComp = [](const Individual& a, const Individual& b) {
        return(b.getFitness() < a.getFitness());
    };

    /**
     * comparator for minimization
     */

    auto minComp = [](const Individual& a, const Individual& b) {
        return (a.getFitness() < b.getFitness());
    };

}

template <>
struct std::formatter<ea::Individual> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const ea::Individual& indiv, std::format_context& ctx) const {
        auto out = ctx.out();

        out = std::format_to(out, "{{\n\tfitness: ");
        if (indiv.isEvaluated()) {
            out = std::format_to(out, "{}", indiv.getFitness());
        } else {
            out = std::format_to(out, "*");
        }

        out = std::format_to(
            out,
            "\n\tgenome: {}\n}}",
            *indiv.getGenome()
        );

        return out;
    }
};
