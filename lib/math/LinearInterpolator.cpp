#include "LinearInterpolator.h"
#include "Error.h"

/*
LinearInterpolator.cpp
Description: Defines all functions for the linear interpolator
Author: Vincent Palmerio
*/

Interpolator::Interpolator(int paramYSize) {

    ySize = paramYSize;
    
    slope = (float*) malloc(sizeof(double) * ySize);   
    y1 = (float*) malloc(sizeof(double) * ySize); 

    for (int i = 0; i < ySize; i++) {
        slope[i] = 0;
        y1[i] = 0;
    }

    resultY(ySize); // initialize resultY with size y
    resultY.setZero();

}

int Interpolator::calculatePoint(float x) {

    for (int i = 0; i < ySize; i++) {
        resultY(i) = (slope[i] * (x-x1)) + y1[i];
    }

    return NO_ERROR_CODE;
}


int Interpolator::rebuildLine(float* paramY1, float* y2, float x1, float x2) {

    y1 = paramY1;

    for (int i = 0; i < ySize; i++) {
        slope[i] = ((y2[i] - y1[1]) / (x2-x1));
    }

    return NO_ERROR_CODE;
}