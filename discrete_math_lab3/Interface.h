#pragma once

#include "CommonInterface.h"
#include <iostream>
#include <unordered_set>
#include <vector>
#include <sstream>

namespace KHAS {

    class Interface final : public CommonInterface
    {
        using Type = size_t;
        std::unordered_set<Type> base_set_;
        std::vector<std::pair<Type, Type>> pairs_;

        std::vector < std::vector<Type>> base_vec_;
        std::vector<Type> vec_bool_;


    private:


        template<typename TType>
        TType inputPowerSet();

        template <typename TPower>
        void inputElemsSet(TPower&& pow);

        template <typename ValueType>
        using PairType = std::pair<ValueType, ValueType>;

        template <typename PowerType>
        void pairsInput(PowerType power);



    private:
        void deletingAPairFromASet();
        void printMatrix();
        void applyPairs();
        void searchForAll(size_t power);
        void searchForOne(size_t elem, size_t power, std::ostream& ss);
    public:
        void loop() override;

        //* params */
        // first:   size_t          table_width_    | not default
        // second:  std::ostream&   out_            | default: std::cout
        Interface(std::size_t table_width, std::ostream& out = std::cout)
            : CommonInterface(table_width, out)
            , base_set_() 
            , pairs_()
            , base_vec_()
            , vec_bool_() {
        
        };
    };

}

#include "Interface.hpp"