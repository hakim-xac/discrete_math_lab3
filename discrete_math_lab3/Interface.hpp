#pragma once

#include <sstream>
#include <iomanip>
#include "Interface.h"

namespace KHAS {

    template <typename TType>
    inline TType Interface::inputPowerSet()
    {
        using InputType = TType;

        InputType min_limit{ 2 };
        InputType power;
        bool is_power;
            push(delimiter('='));
            push(stringGeneration(' ', "������� �������� ���������"));
            push(delimiter('-'));
            flush();

            std::tie(power, is_power) = dataInput<InputType>(ActionWithInputValue::LoopIsError);
            constexpr const auto max_limit{ (std::numeric_limits<InputType>::max)() };

            if (!isClamp(power, min_limit, max_limit)) {
                push(stringGeneration(' ', "������!"));
                push(stringGeneration(' ', "�������� �� ������ � �������� [" + std::to_string(min_limit) + "..." + std::to_string(max_limit) + "]"));
                
                power = clamp(power, min_limit, max_limit);
                
                push(stringGeneration(' ', "�������� �������� ��: " + std::to_string(power)));
                push(delimiter('-'));
            }
        

        return power;

    }


    template <typename TPower>
    inline void Interface::inputElemsSet(TPower&& pow) 
    {

        using value_type = typename decltype(base_set_)::value_type;

        auto power{ std::forward<TPower>(pow) };
        auto count{ power };
        value_type idx{ 1 };
        while(base_set_.size() != power) {
            push(delimiter('-'));
            push(stringGeneration(' ', "������� ������� ���������:", "��� " + std::to_string(power - idx + 1)));
            push(delimiter('-'));
            flush();
            auto [elem, is_elem] { dataInput<value_type>(ActionWithInputValue::LoopIsError) };  
            bool is_not_clamp{ !isClamp(elem, 1, power) };
            bool is_unordered_index{ idx != elem };

            if (is_not_clamp) {
                push(delimiter('-'));
                push(stringGeneration(' ', "�������� ������� �� ������ ��������� �������� ��������� � ���� ������ 1 !"));
                push(delimiter('-'));
                flush();
                continue;
            }
            else if (is_unordered_index) {
                push(delimiter('-'));
                push(stringGeneration(' ', "���� ������ �� ������������� �������!"));
                push(stringGeneration(' ', "[ ��������� "+std::to_string(idx) + " ]" ));
                push(delimiter('-'));
                flush();
                continue;
            }
            ++idx;    

            if (auto mx{ std::max_element(std::begin(base_set_), std::end(base_set_)) };
                base_set_.size() == 0 || (mx != std::end(base_set_) &&  *mx < elem)) {
                base_set_.emplace(elem);
                --count;
            }
            
        }
    }


    template <typename PowerType>
    void Interface::pairsInput(PowerType power)
    {

        using value_type = PowerType;


        push(delimiter('='));
        push(stringGeneration(' ', "���� ���:"));
        push(delimiter('-'));
        flush();
        auto double_size{ power * power };

        size_t count{ 1 };
        while (pairs_.size() != double_size) {
            push(stringGeneration(' ', "���� �����: " + std::to_string(count)));
            push(delimiter('-'));
            push(stringGeneration(' ', "������� 1 ������� ����:"));
            push(delimiter('-'));
            flush();
            auto [first, is_first] { dataInput<value_type>(ActionWithInputValue::LoopIsError) };
            push(delimiter('-'));
            push(stringGeneration(' ', "������� 2 ������� ����:"));
            push(delimiter('-'));
            flush();
            auto [second, is_second] { dataInput<value_type>(ActionWithInputValue::LoopIsError) };
            
            if (!isClamp(first, value_type{ 1 }, power) || !isClamp(second, value_type{ 1 }, power)) {
                push(delimiter('-'));
                push(stringGeneration(' ', "������ ����� ����! ������� ������!"));
                push(delimiter('-'));
                continue;
            }

            pairs_.emplace_back(std::make_pair(first, second));
            ++count;  

            if (pairs_.size() + 1 != double_size) {
                push(delimiter('-'));
                push(stringGeneration(' ', "����������?", "0 - ���", "1 - ��"));
                flush();
                auto [answer, is_answer] { dataInput<value_type>(ActionWithInputValue::ErrorReturn) };
                if (!is_answer || answer != 1) break;
            }
        }

        pairs_.shrink_to_fit();
    }

    template <typename TType>
    void Interface::printMatrix(TType lenght_col)
    {
        using value_type = decltype(base_vec_)::value_type;

        std::stringstream ss;

        push(delimiter('='));
        push(stringGeneration(' ', "����� ������� ���������"));
        push(delimiter('-'));

        for (value_type i{}, ie{ lenght_col }; i != ie; ++i) ss << std::setw(4) << "|" << std::setw(4) << "#" << i + 1;
        push(stringGeneration(' ', ss.str()));
        ss.str("");
        for (value_type i{}, ie{ lenght_col }; i != ie; ++i) ss << std::setw(4) << "-" << std::setw(4) << "-" << "-";
        push(stringGeneration(' ', ss.str()));
        ss.str("");

        for (value_type i{}, ie{ base_vec_.size() }; i != ie; ++i) {
            ss << std::setw(4) << "|" << std::setw(4) << base_vec_[i] << " ";
            if ((i + 1) % lenght_col == 0) {
                push(stringGeneration(' ', ss.str()));
                ss.str("");
            }
        }
        flush();

    }

    template <typename TType>
    void Interface::applyPairs(TType lenght_col) {

        for (auto&& pair : pairs_) {
            auto row{ pair.first - 1 };
            auto col{ pair.second - 1 };

            auto pos{ row * lenght_col + col };
            auto reverse_pos{ col * lenght_col + row };

            assert(base_vec_.size() > pos);
            assert(base_vec_.size() > reverse_pos);
            ++base_vec_[pos];
            ++base_vec_[reverse_pos];
        }
    }

    template<typename TType>
    void Interface::deletingAPairFromASet(TType lenght_col) {

        using value_type = typename decltype(base_vec_)::value_type;

        push(delimiter('='));
        push(stringGeneration(' ', "�������� ����:"));
        push(delimiter('-'));

        value_type first{}, second{};
        bool is_first{}, is_second{};
        do {
            push(stringGeneration(' ', "������� 1 ������� ����:"));
            push(delimiter('-'));
            flush();
            std::tie(first, is_first) = dataInput<value_type>(ActionWithInputValue::LoopIsError);
            push(delimiter('-'));
            push(stringGeneration(' ', "������� 2 ������� ����:"));
            push(delimiter('-'));
            flush();
            std::tie(second, is_second) = dataInput<value_type>(ActionWithInputValue::LoopIsError);
            if (!(is_first && is_second)) {
                push(delimiter('-'));
                push(stringGeneration(' ', "������! ��������� ����!"));
                push(delimiter('-'));
                flush();
            }
        } while (!(is_first && is_second));

        auto fn{ std::find(std::begin(pairs_), std::end(pairs_), std::make_pair(first, second)) };
        if (fn == std::end(pairs_)) {
            push(delimiter('-'));
            push(stringGeneration(' ', "������! ������ ���� �����������!"));
            push(delimiter('-'));
            flush();
            return;
        }

        auto row{ first - 1 };
        auto col{ second - 1 };

        auto pos{ row * lenght_col + col };
        auto reverse_pos{ col * lenght_col + row };

        assert(base_vec_.size() > pos);
        assert(base_vec_.size() > reverse_pos);

        if(base_vec_[pos])          --base_vec_[pos];
        if(base_vec_[reverse_pos])  --base_vec_[reverse_pos];
        if(base_vec_[pos] == 0)     pairs_.erase(fn, std::end(pairs_));

        push(delimiter('-'));
        push(stringGeneration(' ', "������ ���� (" + std::to_string(first) + ", " + std::to_string(second) + ") ������� �������! "));
        push(delimiter('-'));
        flush();
    }
}