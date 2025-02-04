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
        double fitness;

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
            setGenome(std::move(individual.genome->clone()));
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

        /**
         * Returns a copy of the individual
         * @return a copy of the individual
         */

         /*
         std::unique_ptr<Individual> clone() const {
             auto result = std::make_unique<Individual>(*this);
             //result->setFitness(this->fitness);
             //result->setGenome(std::move(this->genome->clone()));
             return result;
         }
         */
        
        friend std::ostream& operator<<(std::ostream& os, const Individual & indiv);

        /*
        std::string toString() {
            return "{\n\tfitness: " + (evaluated ? std::to_string(fitness) : "*") + "\n\tgenome: " + genome->toString() + "\n}";
        }
        */

    };

    std::ostream& operator<<(std::ostream& os, const Individual& indiv){
        os << "{\n\tfitness: ";
        if (indiv.evaluated)
            os << indiv.fitness;
        else
            os << "*";
        os << "\n\tgenome: " << indiv.genome->toString() << "\n}";
        return os;
    }

    using individuals_v = std::vector<std::shared_ptr<Individual>>;
    using individuals_dq = std::deque<std::shared_ptr<Individual>>;

    /**
     * comparator for maximization
     */
     /*
     auto maxComp = [](const std::shared_ptr<Individual> & a, const std::shared_ptr<Individual> & b)->int {
         if (std::abs(a->getFitness() - b->getFitness()) < 1e-9) {
             return 0;
         }
         else if (b->getFitness() < a->getFitness())
             return -1;
         return 1;
     };
     */

    auto maxComp = [](const Individual& a, const Individual& b) {
        return(b.getFitness() < a.getFitness());
    };

    /**
     * comparator for minimization
     */

     /*
     auto minComp = [](const std::shared_ptr<Individual> & a, const std::shared_ptr<Individual> & b)->int {
         if (std::abs(a->getFitness() - b->getFitness()) < 1e-9) {
             return 0;
         }
         else if (a->getFitness() < b->getFitness())
             return -1;
         return 1;
     };
     */

    auto minComp = [](const Individual& a, const Individual& b) {
        return (a.getFitness() < b.getFitness());
    };





}