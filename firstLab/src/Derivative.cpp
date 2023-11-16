#include "cmath"
//
// Created by meteo on 16.11.2023.
//
float Derivative_1realize(float A, float deltaX){
    return (float)(cos(A + deltaX) - cos(A))/deltaX;
}


float Derivative_2realize(float A, float deltaX){
    return (float)(cos(A + deltaX) - cos(A - deltaX))/(2.f*deltaX);
}