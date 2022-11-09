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
            push(stringGeneration(' ', "Введите мощность множества"));
            push(delimiter('-'));
            flush();

            std::tie(power, is_power) = dataInput<InputType>(ActionWithInputValue::LoopIsError);
            constexpr const auto max_limit{ (std::numeric_limits<InputType>::max)() };

            if (!isClamp(power, min_limit, max_limit)) {
                push(stringGeneration(' ', "Ошибка!"));
                push(stringGeneration(' ', "Мощность не попала в диапазон [" + std::to_string(min_limit) + "..." + std::to_string(max_limit) + "]"));
                
                power = clamp(power, min_limit, max_limit);
                
                push(stringGeneration(' ', "Мощность изменена на: " + std::to_string(power)));
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
            push(stringGeneration(' ', "Введите элемент множества:", "ещё " + std::to_string(power - idx + 1)));
            push(delimiter('-'));
            flush();
            auto [elem, is_elem] { dataInput<value_type>(ActionWithInputValue::LoopIsError) };  
            bool is_not_clamp{ !isClamp(elem, 1, power) };
            bool is_unordered_index{ idx != elem };

            if (is_not_clamp) {
                push(delimiter('-'));
                push(stringGeneration(' ', "Введеный элемент не должен превышать мощность множества и быть меньше 1 !"));
                push(delimiter('-'));
                flush();
                continue;
            }
            else if (is_unordered_index) {
                push(delimiter('-'));
                push(stringGeneration(' ', "Ввод данных не соответствует условию!"));
                push(stringGeneration(' ', "[ Ожидается "+std::to_string(idx) + " ]" ));
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

        using value_type = int;


        push(delimiter('='));
        push(stringGeneration(' ', "Ввод пар:"));
        push(delimiter('-'));
        flush();
        auto double_size{ power * power };

        size_t count{ 1 };
        while (pairs_.size() != double_size) {
            push(stringGeneration(' ', "Пара номер: " + std::to_string(count)));
            push(delimiter('-'));
            push(stringGeneration(' ', "Введите 1 элемент пары:"));
            push(delimiter('-'));
            flush();
            auto [first, is_first] { dataInput<value_type>(ActionWithInputValue::LoopIsError) };
            push(delimiter('-'));
            push(stringGeneration(' ', "Введите 2 элемент пары:"));
            push(delimiter('-'));
            flush();
            auto [second, is_second] { dataInput<value_type>(ActionWithInputValue::LoopIsError) };
            
            if (!isClamp(first, value_type{ 1 }, power) || !isClamp(second, value_type{ 1 }, power)) {
                push(delimiter('-'));
                push(stringGeneration(' ', "Ошибка ввода пары! Введите заново!"));
                push(delimiter('-'));
                continue;
            }

            pairs_.emplace_back(std::make_pair(first, second));
            ++count;  

            if (pairs_.size() + 1 != double_size) {
                push(delimiter('-'));
                push(stringGeneration(' ', "Продолжить?", "0 - Нет", "1 - Да"));
                flush();
                auto [answer, is_answer] { dataInput<value_type>(ActionWithInputValue::ErrorReturn) };
                if (!is_answer || answer != 1) break;
            }
        }

        pairs_.shrink_to_fit();
    }

}