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

        using value_type = PowerType;


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

    template <typename TType>
    void Interface::printMatrix(TType lenght_col)
    {
        using value_type = decltype(base_vec_)::value_type;

        std::stringstream ss;

        push(delimiter('='));
        push(stringGeneration(' ', "Вывод матрицы смежности"));
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


    template <typename TType>
    void Interface::printProperties(TType lenght_col) {

        push(delimiter('='));
        push(stringGeneration(' ', "Свойства"));
        push(delimiter('-'));
        printIsReflexive       (lenght_col);
        printIsAntiReflexive   (lenght_col);
        printIsSymmetry        (lenght_col);
        printIsAntiSymmetry    (lenght_col);
        printIsTransitivity    (lenght_col);

        flush();
    }

    template <typename TType>
    void Interface::printIsReflexive(TType lenght_col) {
        
        push(stringGeneration(' ', "Рефлексивность:", (isReflexive(lenght_col) ? "Рефлексивно" : "НЕ Рефлексивно")));
        push(delimiter('-'));
    }

    template <typename TType>
    void Interface::printIsAntiReflexive(TType lenght_col) {

        push(stringGeneration(' ', "Антирефлексивность:", (isAntiReflexive(lenght_col) ? "Антирефлексивно" : "НЕ Антирефлексивно")));
        push(delimiter('-'));
    }

    template <typename TType>
    void Interface::printIsSymmetry(TType lenght_col) {

        push(stringGeneration(' ', "Симметричность:", (isSymmetry(lenght_col) ? "Симметрично" : "НЕ Симметрично")));
        push(delimiter('-'));
    }

    template <typename TType>
    void Interface::printIsAntiSymmetry(TType lenght_col) {

        push(stringGeneration(' ', "Антисимметричность:", (isAntiSymmetry(lenght_col) ? "Антисимметрично" : "НЕ Антисимметрично")));
        push(delimiter('-'));
    }

    template <typename TType>
    void Interface::printIsTransitivity(TType lenght_col) {

        push(stringGeneration(' ', "Транзитивность:", (isTransitivity(lenght_col) ? "Транзитивно" : "НЕ Транзитивно")));
        push(delimiter('-'));
    }

    template <typename TType>
    bool Interface::isReflexive(TType lenght_col) {
        
        for (size_t ie{}, ite{ base_vec_.size() }; ie < ite; ie += (lenght_col + 1))
            if (!base_vec_[ie]) return false;
        return true;
    }

    template <typename TType>
    bool Interface::isAntiReflexive(TType lenght_col) {

        for (size_t it{}, ite{ base_vec_.size() }; it < ite; it += (lenght_col + 1))
            if (base_vec_[it]) return false;
        return true;
    }

    template <typename TType>
    bool Interface::isSymmetry(TType lenght_col) {

    for (size_t it{ 1 }, ite{ lenght_col }; it < ite; ++it) {
            auto left{ lenght_col * it + it - 1 };
            auto right{ (it - 1) * lenght_col + it };
            
            if (base_vec_[left] != base_vec_[right]) return false;
        }
        return true;
    }

    template <typename TType>
    bool Interface::isAntiSymmetry(TType lenght_col) {

        return !isSymmetry(lenght_col);
    }

    template <typename TType>
    bool Interface::isTransitivity(TType lenght_col) {

        for (const auto& elem : pairs_) {

            auto fnd{ std::find_if(std::begin(pairs_)
                , std::end(pairs_)
                , [&](const auto& pair ) {

                auto pos { (elem.first - 1) * lenght_col + pair.second - 1 };
                return elem.second == pair.first && base_vec_[pos] == 1;

                }) };
            if (fnd == pairs_.end()) return false;
        }
        return true;
    }


    template<typename TType>
    void Interface::deletingAPairFromASet(TType lenght_col) {

        using value_type = typename decltype(base_vec_)::value_type;

        push(delimiter('='));
        push(stringGeneration(' ', "Удаление пары:"));
        push(delimiter('-'));

        value_type first{}, second{};
        bool is_first{}, is_second{};
        do {
            push(stringGeneration(' ', "Введите 1 элемент пары:"));
            push(delimiter('-'));
            flush();
            std::tie(first, is_first) = dataInput<value_type>(ActionWithInputValue::LoopIsError);
            push(delimiter('-'));
            push(stringGeneration(' ', "Введите 2 элемент пары:"));
            push(delimiter('-'));
            flush();
            std::tie(second, is_second) = dataInput<value_type>(ActionWithInputValue::LoopIsError);
            if (!(is_first && is_second)) {
                push(delimiter('-'));
                push(stringGeneration(' ', "Ошибка! Повторите ввод!"));
                push(delimiter('-'));
                flush();
            }
        } while (!(is_first && is_second));

        auto fn{ std::find(std::begin(pairs_), std::end(pairs_), std::make_pair(first, second)) };
        if (fn == std::end(pairs_)) {
            push(delimiter('-'));
            push(stringGeneration(' ', "Ошибка! Данная пара отсутствует!"));
            push(delimiter('-'));
            flush();
            return;
        }

        pairs_.erase(fn, std::end(pairs_));
        auto row{ first - 1 };
        auto col{ second - 1 };

        auto pos{ row * lenght_col + col };
        assert(base_vec_.size() > pos);
        base_vec_[pos] = 0;

        push(delimiter('-'));
        push(stringGeneration(' ', "Данная пара (" + std::to_string(first) + ", " + std::to_string(second) + ") успешно удалена! "));
        push(delimiter('-'));
        flush();
    }


    template<typename TType>
    void Interface::removingAPairFromASet(TType lenght_col) {

        using value_type = typename decltype(base_vec_)::value_type;

        push(delimiter('='));
        push(stringGeneration(' ', "Изменение пары:"));
        push(delimiter('-'));

        value_type first{}, second{};
        bool is_first{}, is_second{};

        push(stringGeneration(' ', "Введите 1 элемент пары:"));
        push(delimiter('-'));
        flush();
        std::tie(first, is_first) = dataInput<value_type>(ActionWithInputValue::LoopIsError);
        push(delimiter('-'));
        push(stringGeneration(' ', "Введите 2 элемент пары:"));
        push(delimiter('-'));
        flush();
        std::tie(second, is_second) = dataInput<value_type>(ActionWithInputValue::LoopIsError);
        if (!(is_first && is_second)) {
            push(delimiter('-'));
            push(stringGeneration(' ', "Данной пары не существует!"));
            push(delimiter('-'));
            flush();
        }
        auto fn{ std::find(std::begin(pairs_), std::end(pairs_), std::make_pair(first, second)) };

        if (fn == std::end(pairs_)) {

            auto [min, max]{ std::minmax_element(std::begin(base_vec_), std::end(base_vec_)) };

            if (!isClamp(first, *min, *max) || !isClamp(second, *min, *max)) {
                push(delimiter('-'));
                push(stringGeneration(' ', "Данной пары не существует!"));
                push(delimiter('-'));
                flush();
                return;
            }

            pairs_.emplace_back(std::make_pair(first, second));
        }
        else pairs_.erase(fn, std::end(pairs_));

        auto row{ first - 1 };
        auto col{ second - 1 };
        auto pos{ row * lenght_col + col };

        assert(base_vec_.size() > pos);
        auto old_value{ base_vec_[pos] };
        auto new_value{ old_value ? 0 : 1 };
        base_vec_[pos] = new_value;

        push(delimiter('-'));
        push(stringGeneration(' ', "Пара успешно изменена!"));
        push(delimiter('-'));
        push(stringGeneration(' ', "Старое значение: " + std::to_string(old_value)));
        push(stringGeneration(' ', "Новое значение: " + std::to_string(new_value)));
        push(delimiter('-'));
        flush();
    }

}