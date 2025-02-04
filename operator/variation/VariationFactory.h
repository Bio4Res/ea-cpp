#pragma once

#include <memory>
#include <util/enumutils.h>
#include <operator/variation/VariationOperator.h>
#include <operator/variation/recombination/discrete/UniformCrossover.h>
#include <operator/variation/mutation/discrete/BitFlip.h>
#include <operator/variation/initialization/discrete/RandomBitString.h>

#include <operator/variation/initialization/continuous/RandomVector.h>
#include <operator/variation/recombination/continuous/BLX.h>
#include <operator/variation/recombination/discrete/SinglePointCrossover.h>
#include <operator/variation/mutation/continuous/GaussianMutation.h>



namespace ea {

    ENUM(enumVariationType, int, SPX, UX, BLX, OX, UCX, BINOMIAL, BITFLIP, GAUSSIAN, SWAP, REVERSE, FLIP, ESMUTATION, BITSTRING, RANDOMVECTOR, PERMUTATION, STEEPEST_IMPROVEMENT, UNRELIABLE)

        struct VariationFactory {
        /**
         * Returns a variation operator given its name.
         * If the name does not correspond to any known operator,
         * it returns null.
         * @param name the name of the variation operator
         * @param pars the parameters of the variation operator
         * @return the variation operator named
         */
        static std::unique_ptr<VariationOperator> create(const std::string& name, const std::vector<std::string>& pars) {
            //JES FIX ME: hacer todos los metodos!
            //switch (name.toUpperCase()) {
            switch (enumVariationType::_from_string_nocase(name.c_str())) {
              // Recombination
                case enumVariationType::SPX:
                    return std::make_unique<SinglePointCrossover>(pars);
                    break;
                case enumVariationType::UX:
                    return std::make_unique<UniformCrossover>(pars);
                    break;
                case enumVariationType::BLX:
                    return std::make_unique<BLX>(pars);
                    break;
    /*            case enumVariationType::OX:
                    op = new OrderCrossover(pars);
                    break;
                case enumVariationType::UCX:
                    op = new UniformCycleCrossover(pars);
                    break;

                // Mutation
                case enumVariationType::BINOMIAL:
                    op = new BinomialMutation(pars);
                    break;
    */            case enumVariationType::BITFLIP:
                      return std::make_unique<BitFlip>(pars);
                      break;
                  case enumVariationType::GAUSSIAN:
                      return std::make_unique<GaussianMutation>(pars);
                      break;
        /*            case enumVariationType::SWAP:
                        op = new Swap(pars);
                        break;
                    case enumVariationType::REVERSE:
                        op = new Reverse(pars);
                        break;
                    case enumVariationType::FLIP:
                        op = new Flip(pars);
                        break;

                        // Evolution strategies
                    case enumVariationType::ESMUTATION:
                        op = new ESMutation(pars);
                        break;
        */
                    // Initialization
                    case enumVariationType::BITSTRING:
                        return std::make_unique<RandomBitString>(pars);
                        break;
                    case enumVariationType::RANDOMVECTOR:
                        return std::make_unique<RandomVector>(pars);
                        break;
        /*            case enumVariationType::PERMUTATION:
                        op = new RandomPermutation(pars);
                        break;

                        // Local search
                    case enumVariationType::STEEPEST_IMPROVEMENT:
                        op = new SteepestImprovement(pars);
                        break;

                        // Unreliable
                    case enumVariationType::UNRELIABLE:
                        op = new UnreliableVariationOperator(pars);
                        break;
*/
                default:
                    break;
            }
            return { nullptr };
        }
    };

}