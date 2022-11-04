#pragma once

#include "CommonInterface.h"
#include <iostream>
#include <unordered_set>
#include <sstream>

namespace KHAS {

    class Interface final : public CommonInterface
    {
        using Type = size_t;
        std::unordered_set<Type> base_set_;
        std::vector<std::pair<Type, Type>> pairs_;
        std::vector<Type> base_vec_;

    private:

        template<typename TType>
        void deletingAPairFromASet(TType lenght_col);

        template<typename TType>
        void removingAPairFromASet(TType lenght_col);

        template<typename TType>
        TType inputPowerSet();

        template <typename TPower>
        void inputElemsSet(TPower&& pow);

        template <typename ValueType>
        using PairType = std::pair<ValueType, ValueType>;

        template <typename PowerType>
        void pairsInput(PowerType power);

        template <typename TType>
        void printMatrix(TType lenght_col);

        template <typename TType>
        void applyPairs(TType lenght_col);

        template <typename TType>
        void printProperties(TType lenght_col);

        template <typename TType>
        void printIsReflexive(TType lenght_col);

        template <typename TType>
        void printIsAntiReflexive(TType lenght_col);

        template <typename TType>
        void printIsSymmetry(TType lenght_col);

        template <typename TType>
        void printIsAntiSymmetry(TType lenght_col);

        template <typename TType>
        void printIsTransitivity(TType lenght_col);

        template <typename TType>
        bool isReflexive(TType lenght_col);

        template <typename TType>
        bool isAntiReflexive(TType lenght_col);

        template <typename TType>
        bool isSymmetry(TType lenght_col);

        template <typename TType>
        bool isAntiSymmetry(TType lenght_col);

        template <typename TType>
        bool isTransitivity(TType lenght_col);




    public:
        void loop() override;

        //* params */
        // first:   size_t          table_width_    | not default
        // second:  std::ostream&   out_            | default: std::cout
        Interface(std::size_t table_width, std::ostream& out = std::cout)
            : CommonInterface(table_width, out)
            , base_set_() 
            , pairs_()
            , base_vec_() {};
    };

}

#include "Interface.hpp"