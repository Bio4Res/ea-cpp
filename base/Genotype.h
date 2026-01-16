#pragma once

#include <vector>
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
        std::vector<geno_t> genes;
        /**
         * Creates a genome of the desired length. Each gene is not initialized in C++
         * @param l the genome length
         */
        Genotype(std::size_t l, GeneType t) : type{t}, size{l} , genes(l){
        }

        Genotype(const geno_t* tocopy, std::size_t l, GeneType t) : type{t}, size{l} , genes(l){
            std::memcpy(genes.data(), tocopy, size*sizeof(geno_t));
        }

        // Constructor de copia
        Genotype(const Genotype& other) : type{other.type}, size{other.size}, genes(other.size){
            std::memcpy(genes.data(), other.genes.data(), size * sizeof(geno_t));
        }
        
        // Move constructor
        Genotype(Genotype&& other) noexcept 
            : type(other.type), size(other.size), genes(std::move(other.genes)){}
        
        // Copy assignment
        Genotype& operator=(const Genotype& other){
            if (this != &other) {
                assert(size == other.size && "Cannot assign genotypes of different sizes");
                assert(type == other.type && "Cannot assign genotypes of different types");
                std::memcpy(genes.data(), other.genes.data(), size * sizeof(geno_t));
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

        friend struct std::formatter<ea::Genotype>;
    };

}

template <>
struct std::formatter<ea::Genotype> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const ea::Genotype& g, std::format_context& ctx) const {
        auto out = ctx.out();

        const size_t n = g.length();
        if (g.getType() == ea::GeneType::INT) {
            for (size_t i = 0; i < n; ++i) {
                out = std::format_to(out, " {}", g.genes[i].i);
            }
        } else {
            for (size_t i = 0; i < n; ++i) {
                out = std::format_to(out, " {}", g.genes[i].d);
            }
        }
        return out;
    }
};
