/*
Integrator.cpp - Integrator CPP file
Description: defines functions and variales in Integrator.h
Author: Vincent Palmerio
Created: 10/20/2023
Last updated: 10/24/2023
*/

#include "Integrator.h"
#include "../error/Error.h"

int Integrator::integratorSetup(Eigen::VectorXd* pointerToData)
{

    dataToIntegrate = pointerToData;

    if (dataToIntegrate == nullptr) {
        return MEMORY_ALLOCATION_ERROR_CODE;
    }

    int vectorSize = pointerToData->size();

    Eigen::VectorXd v(vectorSize);

    for (int i = 0; i < vectorSize; i++) {
        v(i) = 0;
    }

    integratedData = v;

    previousValue = *dataToIntegrate; // initialize previousValue to the first value of dataToIntegrate

    if (integratedData.size() == 0) {
        return VECTOR_INIT_ZERO_SIZE_ERROR;
    }


    return NO_ERROR_CODE;
}

int Integrator::integratorUpdate()
{
    double changeInTime = timeBetweenIntegration;

    changeInTime = changeInTime / 1000000; //convert to seconds
    
#if defined(ASTRA_INTEGRATOR_DEBUG) or defined(ASTRA_FULL_DEBUG)
    Serial.println("Time between integration seconds: ");
    Serial.println(changeInTime, 60);
#endif
    
    integratedData += (*dataToIntegrate + previousValue) / 2.0 * changeInTime; // trapezoidal integration

    // TODO: maybe use https://en.wikipedia.org/wiki/Simpson%27s_rule if we're able to save 3 values in the future.

    timeBetweenIntegration = 0;

    previousValue = *dataToIntegrate;

    if (changeInTime > MAX_ALLOWED_TIME_BETWEEN_INTEGRATION) {
        return MAX_ALLOWED_TIME_BETWEEN_INTEGRATION_EXCEEDED;
    }

    return NO_ERROR_CODE;
}
