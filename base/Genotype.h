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
    private:    
        const GeneType type;    
        const size_t size;
    public:
        /**
         * The genes in the genome
         */
        //std::unique_ptr<geno_t[]> genes;
        std::vector<geno_t> genes;
        /**
         * Creates a genome of the desired length. Each gene is not initialized in C++
         * @param l the genome length
         */
        Genotype(std::size_t l, GeneType t) : type{t}, size{l}, genes(l){
            //genes = std::make_unique<geno_t[]>(l);
        }

        Genotype(const geno_t* tocopy, std::size_t l, GeneType t) : type{t}, size{l}, genes(l){
            //genes = std::make_unique<geno_t[]>(size);
            std::memcpy(genes.data(), tocopy, size*sizeof(geno_t));
        }

        // Constructor de copia
        Genotype(const Genotype& other) : type{other.type}, size{other.size}, genes(other.size) {
            //genes = std::make_unique<Gene[]>(size);
            std::memcpy(genes.data(), other.genes.data(), size * sizeof(Gene));
        }
        
        // Move constructor
        Genotype(Genotype&& other) noexcept 
            : type(other.type), size(other.size), genes(std::move(other.genes)){}
        
        // Copy assignment
        Genotype& operator=(const Genotype& other){
            if (this != &other) {
                assert(size == other.size && "Cannot assign genotypes of different sizes");
                assert(type == other.type && "Cannot assign genotypes of different types");
                std::memcpy(genes.data(), other.genes.data(), size * sizeof(Gene));
            }
            return *this;
        }
        
        // Move assignment
        Genotype& operator=(Genotype&& other) noexcept {
            if (this != &other) {
                assert(size == other.size && "Cannot assign genotypes of different sizes");
                assert(type == other.type && "Cannot assign genotypes of different types");
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
            return std::make_unique<Genotype>(genes.data(), size, type);
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
    };

}