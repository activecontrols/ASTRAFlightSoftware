/*
Integrator.cpp - Integrator CPP file
Description: defines functions and variales in Integrator.h
Author: Vincent Palmerio
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
    
    integratedData += (*dataToIntegrate) * changeInTime;

    timeBetweenIntegration = 0;

    if (changeInTime > MAX_ALLOWED_TIME_BETWEEN_INTEGRATION) {
        return MAX_ALLOWED_TIME_BETWEEN_INTEGRATION_EXCEEDED;
    }

    return NO_ERROR_CODE;
}
