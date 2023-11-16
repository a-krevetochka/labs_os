#include "iostream"
#include "Derivative.h"
#include <windows.h>
#include <fstream>

//
// Created by meteo on 16.11.2023.
//
typedef float (*countE_1realize)(int);

typedef float (*countE_2realize)(int);

using namespace std;

int main() {
    float derative_arg1, derative_arg2;
    int x;

    cout << "do y wanna test me? " << endl;
    char is_test = getchar();

    if (is_test == 'y'){
        ifstream file;
        file.open("test.txt");
        if (!file.is_open()){
            cerr << "File want open" << endl;
            return -1;
        }
        file >> derative_arg1 >> derative_arg2 >> x;
        file.close();
    }

    else{
        cout << "1" << endl;
        cin >> derative_arg1 >> derative_arg2;
    }

    float res1 = Derivative_1realize(derative_arg1, derative_arg2);
    float res2 = Derivative_2realize(derative_arg1, derative_arg2);

    std::cout << "first realization answer is: " << res1 << " ,second realization answer is:  " << res2 << ", difference is: " << abs(res1 - res2) << std::endl;

    HINSTANCE hGetProcIDDLL = LoadLibrary("libMyLibrary.dll");

    if (!hGetProcIDDLL) {
        std::cout << "could not load the dynamic library" << std::endl;
        return EXIT_FAILURE;
    }

    auto func_countE_1realize = (countE_1realize) GetProcAddress(hGetProcIDDLL, "countE_1realize");

    auto func_countE_2realize = (countE_2realize) GetProcAddress(hGetProcIDDLL, "countE_1realize");

    if (!func_countE_1realize || !func_countE_2realize) {
        std::cerr << "could not locate the function" << std::endl;
        return -1;
    }
    if (is_test != 'y'){
        cout << "2" << endl;
        cin >> x;
    }

    float result1 = func_countE_1realize(x);
    std::cout << "1 realization returned: " << result1;

    float result2 = func_countE_2realize(x);
    std::cout << ", 2 realization returned: " << result2 << std::endl;

    FreeLibrary(hGetProcIDDLL);
    return 0;

}