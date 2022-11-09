#include "Interface.h"
#include <iostream>
#include <bitset>
#include <vector>


const int n{3};
bool s[n];
int a[n][n]{ {1, 0, 1}, {0, 0, 1}, {1, 1, 1} };

void SearchForOne(int u) {
    std::cout << u+1 << " ";
    s[u] = true;
    for (int i{}, ie{ n }; i != ie; ++i) {
        if (a[u][i] > 0 && !s[i]) SearchForOne(i);
    }
}

void SearchForAll() {
    for (auto&& elem : s) elem = false;
    int k = 0;
    for (int i{}, ie{ n }; i != ie; ++i) {
        if (!s[i]) {
            ++k;
            std::cout << k << ") ";
            SearchForOne(i);
            std::cout << "\n";
        }
    }
    
}


//int main()
//{
//    setlocale(LC_ALL, "");
//    std::cout << "Компонент связности:\n";
//    SearchForAll();
//    system("pause");
//    return 0;
//
//}




int main()
{
    KHAS::Interface window{ 100 };
    window.loop();
}

