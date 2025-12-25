#pragma once

#include <vector>
#include <any>
#include <memory>
#include <string>
#include <cassert>

namespace ea {
    enum class GeneType : uint8_t {
        INT,
        DOUBLE
    };

    union Gene {
        int i;
        double d;
        
        Gene() : d(0.0) {}
        explicit Gene(int val) : i(val) {}
        explicit Gene(double val) : d(val) {}
    };

    struct Genotype { 

        using geno_t = Gene;
        /**
         * The genes in the genome
         */
        
        std::unique_ptr<geno_t[]> genes;
        /**
         * Creates a genome of the desired length. Each gene is not initialized in C++
         * @param l the genome length
         */
        Genotype(std::size_t l, GeneType t) : size{l}, type{t}{
            genes = std::make_unique<geno_t[]>(l);
        }

        Genotype(const geno_t* tocopy, std::size_t l, GeneType t) : size{l}, type{t}{
            genes = std::make_unique<geno_t[]>(size);
            std::memcpy(genes.get(), tocopy, size*sizeof(geno_t));
        }

        // Constructor de copia
        Genotype(const Genotype& other) : size{other.size}, type{other.type} {
            genes = std::make_unique<Gene[]>(size);
            std::memcpy(genes.get(), other.genes.get(), size * sizeof(Gene));
        }
        
        // Move constructor
        Genotype(Genotype&& other) noexcept 
            : genes(std::move(other.genes)), size(other.size), type(other.type) {}
        
        // Copy assignment
        Genotype& operator=(const Genotype& other) {
            if (this != &other) {
                assert(size == other.size && "Cannot assign genotypes of different sizes");
                assert(type == other.type && "Cannot assign genotypes of different types");
                std::memcpy(genes.get(), other.genes.get(), size * sizeof(Gene));
            }
            return *this;
        }
        
        // Move assignment
        Genotype& operator=(Genotype&& other) noexcept {
            if (this != &other) {
                genes = std::move(other.genes);
            }
            return *this;
        }
        /**
         * Returns the length of the genotype
         * @return the length of the genotype
         */
        std::size_t length() const { return size; }

        GeneType getType() const { return type; }

        std::unique_ptr<Genotype> clone() {
            return std::make_unique<Genotype>(genes.get(), size, type);
        }

        std::string toString() const {
            std::string str;
            str.reserve(size * 12);
            
            if (type == GeneType::INT) {
                for (size_t i = 0; i < size; ++i) {
                    str += " ";
                    str += std::to_string(genes[i].i);
                }
            } else {
                for (size_t i = 0; i < size; ++i) {
                    str += " ";
                    str += std::to_string(genes[i].d);
                }
            }
            return str;
        }
    private:
        const size_t size;
        const GeneType type;
    };

}