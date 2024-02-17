/*
Integrator.cpp - Integrator CPP file
Description: defines functions and variales in Integrator.h
Author: Ishan Goel, Vincent Palmerio
Created: 10/20/2023
Last updated: 01/10/2024 by Ishan Goel <goeli@purdue.edu>
*/

#include "Integrator.h"
#include "Error.h"


// This function needs testing
int Integrator::reset(Eigen::VectorXd* vector)
{
    ready_for_simpson = false;
    prevValue = *vector;
    integratedData = *vector;
    time = 0;

    return NO_ERROR_CODE;
}

int Integrator::integratorSetup(Eigen::VectorXd* pointerToData)
{
    dataToIntegrate = pointerToData;

    if (dataToIntegrate == nullptr) {
        return MEMORY_ALLOCATION_ERROR_CODE;
    }

    int vectorSize = pointerToData->size();

    Eigen::VectorXd v(vectorSize);
    // for (int i = 0; i < vectorSize; i++) {
    //     v(i) = 0;
    // }
    v.setZero();

    integratedData = v;

    prevValue = *dataToIntegrate; // initialize previousValue to the first value of dataToIntegrate

    if (integratedData.size() == 0) {
        return VECTOR_INIT_ZERO_SIZE_ERROR;
    }

    return NO_ERROR_CODE;
}

int Integrator::integratorUpdate()
{
    static bool ready_for_simpson = false;
    double changeInTime = time;
    changeInTime = changeInTime / 1000000; // convert to seconds

    dtPrev = dt;
    dt = changeInTime;
    
#if defined(ASTRA_INTEGRATOR_DEBUG) or defined(ASTRA_FULL_DEBUG)
    Serial.println("Time between integration seconds: ");
    Serial.println(changeInTime, 60);
#endif

    if (!ready_for_simpson) { // not enough data to integrate with simpson's rule
        integratedData += (*dataToIntegrate + prevValue) / 2.0 * changeInTime; // trapezoidal integration
        ready_for_simpson = true;
    } else {
        integratedData = simpson_nonuniform_cumulative(prev2Value, prevValue, *dataToIntegrate, dtPrev, dt);
    }

    prev2Value = prevValue;
    prevValue = *dataToIntegrate;
    time = 0;

    if (changeInTime > MAX_ALLOWED_TIME_BETWEEN_INTEGRATION) {
        return MAX_ALLOWED_TIME_BETWEEN_INTEGRATION_EXCEEDED;
    }

    return NO_ERROR_CODE;
}

Eigen::VectorXd Integrator::simpson_nonuniform_cumulative(
    Eigen::VectorXd prev2_f, Eigen::VectorXd prev_f, Eigen::VectorXd new_f,
    double dtPrev, double dt) const {

    static Eigen::VectorXd integ = Eigen::VectorXd(integratedData.size()).setConstant(1.0);
    static Eigen::VectorXd last_correction = Eigen::VectorXd(integratedData.size()).setConstant(1.0);
    static Eigen::VectorXd zero = Eigen::VectorXd(integratedData.size()).setZero();

    double h0 = dtPrev, h1 = dt;

    // implementation is a result of https://en.wikipedia.org/wiki/Simpson%27s_rule

    if (!last_correction.isZero()) {
        integ -= last_correction;
        last_correction = zero;
        double hph = h1 + h0, hdh = h1 / h0, hmh = h1 * h0;
        integ += (hph / 6) * ((2 - hdh) * prev2_f + (hph * hph / hmh) * prev_f + (2 - 1 / hdh) * new_f);
    } else {
        last_correction += new_f * (2 * h1 * h1 + 3 * h0 * h1) / (6 * (h0 + h1));
        last_correction += prev_f * (h1 * h1 + 3 * h1 * h0) / (6 * h0);
        last_correction -= prev2_f * h1 * h1 * h1 / (6 * h0 * (h0 + h1));
        integ += last_correction;
    }
    return integ;
}
