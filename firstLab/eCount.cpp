#include <windows.h>
#include <iostream>
#include <iomanip>
#include "cmath"
//
// Created by meteo on 16.11.2023.
//
extern "C" {
__declspec(dllexport) float countE_1realize(int x) {
    float result = pow(1.f / x + 1.f, x);
    return result;
}


__declspec(dllexport) float countE_2realize(int x) {
    float res = 0.f;
    for (int iteration = 1; iteration <= x; ++iteration) {
        float prev = res;
        res = prev + prev / (float) iteration;
    }
    return res;
}
}