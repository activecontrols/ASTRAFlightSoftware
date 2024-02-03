/*
Derivative.cpp - Derivative CPP file
Description: defines the functions and variales in Derivative.h
Author: Vincent Palmerio
*/

#include "Derivative.h"
#include "Error.h"


Eigen::VectorXd derivative(0);

int Derivative::derivativeSetup(Eigen::VectorXd* pointerToData, Eigen::VectorXd startingData)
{
    newestData = pointerToData;

    if (newestData == nullptr) {
        return MEMORY_ALLOCATION_ERROR_CODE;
    }

    int vectorSize = newestData->size();

    if (startingData.size() != vectorSize) {
        return VECTOR_SIZE_MISMATCH;
    }

    previousData = startingData;

    Eigen::VectorXd v(vectorSize);

    //initialize vector to all 0 values
    for (int i = 0; i < vectorSize; i++) {
        v(i) = 0;
    }

    derivative = v;

    if (previousData.size() == 0 || derivative.size() == 0) {
        return VECTOR_INIT_ZERO_SIZE_ERROR;
    }

    return NO_ERROR_CODE;
}

int Derivative::derivativeUpdate()
{
    Serial.println("Time Step Derivative (microseconds)");
    Serial.println(timeStepDerivative);


    Eigen::VectorXd newData = (*newestData);

    Serial.println("Newest Data Vector");
    for (int i = 0; i < newData.size(); i++) {
        Serial.println(newData(i));
    }

    Serial.println("Previous Data Vector");
    for (int i = 0; i < previousData.size(); i++) {
        Serial.println(previousData(i));
    }

    derivative = ((*newestData) - previousData)/timeStepDerivative;

    timeStepDerivative = 0;

    previousData = (*newestData);

    return NO_ERROR_CODE;
}

