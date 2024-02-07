#include "Estimator.h"
#include "Integrator.h"
#include "Error.h"
#include "IMU.h"

#include <ArduinoEigenDense.h>

/*
Estimator.cpp
Description:  Defines all functions for the estimator, including those declared in Estimator.h
Author: Vincent Palmerio
*/

Eigen::VectorXd estimatedStateX(ESTIMATED_STATE_DIMENSION);
Integrator linearAccelIntegrator;

/*
 * Initializes estimation library, including malloc'ing global variables
 * returns error code as int (NO_ERROR_CODE, GENERAL_ERROR_CODE, MEMORY_ALLOCATION_ERROR_CODE)
 */
int initializeEstimator() {

    for (unsigned int i = 0; i < ESTIMATED_STATE_DIMENSION; i++) {
        estimatedStateX[i] = 0;
    }

    linearAccelIntegrator.integratorSetup(&linearAccelVector);
    
    return NO_ERROR_CODE;
}

int updateEstimator() {
    updateIMU();
    linearAccelIntegrator.integratorUpdate();

    estimatedStateX(0) = qx;
    estimatedStateX(1) = qy;
    estimatedStateX(2) = qz;

    estimatedStateX(3) = gx;
    estimatedStateX(4) = gy;
    estimatedStateX(5) = gz;

    estimatedStateX(6) = linearAccelIntegrator.integratedData(0);


    return NO_ERROR_CODE;
}