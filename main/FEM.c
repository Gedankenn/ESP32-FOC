#include "FEM.h"
extern double pi;

double FEM_A(double theta){
    //double pi = 3.141592;
    theta = fmod(theta,(2*pi));
    return sin(theta);
}

double FEM_B(double theta){
    theta = fmod(theta,(2*pi));
    return sin(theta + 2*pi/3.0);
}

double FEM_C(double theta){
    theta = fmod(theta,(2*pi));
    return sin(theta + 4*pi/3.0);
}