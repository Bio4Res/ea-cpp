#pragma once

#include <vector>
#include <any>
#include <memory>
#include <string>
#include <variant>
#include <cassert>

namespace ea {

    struct Genotype { 

        using geno_t = std::variant<int, double>;
        /**
         * The genes in the genome
         */
        
        std::unique_ptr<geno_t[]> genes;
        /**
         * Creates a genome of the desired length. Each gene is not initialized in C++
         * @param l the genome length
         */
        Genotype(std::size_t l) : size{ l } {
            genes = std::make_unique_for_overwrite<geno_t[]>(l);
        }

        Genotype(const geno_t* tocopy, std::size_t l) : size{ l } {
            genes = std::make_unique_for_overwrite<geno_t[]>(size);
            std::memcpy(genes.get(), tocopy, size*sizeof(geno_t));
        }

        /**
         * Returns the length of the genotype
         * @return the length of the genotype
         */
        std::size_t length() const {
            return size;
        }

        std::unique_ptr<Genotype> clone() {
            auto copy = std::make_unique<Genotype>(genes.get(), size);
            //copy->genes = genes;
            return copy;
        }

        std::string toString() {
            std::string str = "";
            for (size_t i = 0; i < size; ++i)
                std::visit([&str](const auto& x) { str += " "; str += std::to_string(x); }, genes[i]);
            return str;
        }
    private:
        const size_t size;
    };

}