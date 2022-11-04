#include "Interface.h"
#include <unordered_set>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>

namespace KHAS {

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
                base_vec_.resize(power * power, Type{});
                pairsInput(power);
                applyPairs(power);
                printMatrix(power);
            }
                break;
            case KHAS::Commands::DeletingAPair:     deletingAPairFromASet(power); break;
            case KHAS::Commands::Unknown:           printUnknown(); break;
            case KHAS::Commands::SetNewSET:         power = inputPowerSet<Type>();
            case KHAS::Commands::None:
            {
                base_set_.clear();
                base_vec_.clear();
                pairs_.clear();

                base_set_.reserve(power);
                pairs_.reserve(power * power);
                base_vec_.resize(power * power, Type{});
                pairsInput(power);
                applyPairs(power); 
            }
                break;
            }            

            printMatrix(power);
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