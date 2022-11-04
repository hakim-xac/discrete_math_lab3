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

    private:

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