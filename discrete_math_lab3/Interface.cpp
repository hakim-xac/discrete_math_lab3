#include "Interface.h"
#include <unordered_set>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>

namespace KHAS {


    void Interface::deletingAPairFromASet() {

        using value_type = size_t;

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

        auto row{ first - 1 };
        auto col{ second - 1 };

        if (base_vec_[row][col])         --base_vec_[row][col];
        if (base_vec_[row][col] == 0)    pairs_.erase(fn, std::end(pairs_));

        push(delimiter('-'));
        push(stringGeneration(' ', "Данная пара (" + std::to_string(first) + ", " + std::to_string(second) + ") успешно удалена! "));
        push(delimiter('-'));
        flush();
    }

    void Interface::printMatrix()
    {
        using value_type = size_t;

        std::stringstream ss;

        push(delimiter('='));
        push(stringGeneration(' ', "Вывод матрицы смежности"));
        push(delimiter('-'));

        for (size_t i{}, ie{ base_vec_.size() }; i != ie; ++i) ss << std::setw(4) << "|" << std::setw(4) << "#" << i + 1;
        push(stringGeneration(' ', ss.str()));
        ss.str("");
        for (size_t i{}, ie{ base_vec_.size() }; i != ie; ++i) ss << std::setw(4) << "-" << std::setw(4) << "-" << "-";
        push(stringGeneration(' ', ss.str()));
        ss.str("");


        for (auto&& vecs : base_vec_) {
            for (auto&& elem : vecs) {
                ss << std::setw(4) << "|" << std::setw(4) << elem << " ";
            }
            push(stringGeneration(' ', ss.str()));
            ss.str("");
        }

        flush();

    }

    void Interface::applyPairs() {

        for (auto&& pair : pairs_) {
            auto row{ pair.first - 1 };
            auto col{ pair.second - 1 };
            ++base_vec_[row][col];
        }
    }

    void Interface::searchForAll(size_t power) {

        vec_bool_.resize(power, 0);
        base_vec_.resize(power, std::vector<Type>(power));
        push(delimiter('='));
        push(stringGeneration(' ', "Компоненты связности"));
        int k{};
        std::stringstream ss, ss_count;
        for (size_t i{}, ie{ power }; i != ie; ++i) {
            if (vec_bool_[i] == 0) {
                ++k;
                ss_count << std::setw(8) << k << ")";
                searchForOne(i, power, ss);
                push(delimiter('-'));
                push(stringGeneration(' ', ss_count.str(), "", ss.str()));
                ss_count.str("");
                ss.str("");
            }
        }
        flush();

    }

    void Interface::searchForOne(size_t elem, size_t power, std::ostream& ss) {

        ss << elem + 1 << " ";
        vec_bool_[elem] = 1;
        for (size_t i{}, ie{ power }; i != ie; ++i) {
            if (base_vec_[elem][i] > 0 && !vec_bool_[i]) searchForOne(i, power, ss);
        }
    }

    void Interface::loop()
    {
        setlocale(LC_ALL, "Russian");

        auto activeCommand{ Commands::None };
        system("cls");
        printHeader();
        auto power{ inputPowerSet<Type>() };

        while (activeCommand != Commands::Exit) {

            switch (activeCommand)
            {
            case KHAS::Commands::AddingAPair:
            {
                base_vec_.clear();
                base_vec_.resize(power, std::vector<Type>(power));
                pairsInput(power);
                applyPairs();
                printMatrix();
            }
                break;
            case KHAS::Commands::DeletingAPair:     deletingAPairFromASet(); break;
            case KHAS::Commands::Unknown:           printUnknown(); break;
            case KHAS::Commands::SetNewSET:         power = inputPowerSet<Type>();
            case KHAS::Commands::None:
            {
                base_set_.clear();
                base_vec_.clear();
                pairs_.clear();

                base_set_.reserve(power);
                pairs_.reserve(power * power);
                base_vec_.resize(power, std::vector<Type>(power));
                pairsInput(power);
                applyPairs(); 
            }
                break;
            }

            printMatrix();
            searchForAll(power);
            printMenu();

            activeCommand = selectCommand();

            system("cls");
            printHeader();
        }

        push(delimiter('-'));
        push(stringGeneration(' ', "Выход из программы выполнен!"));
        push(delimiter('-'));
        flush();
    }
}