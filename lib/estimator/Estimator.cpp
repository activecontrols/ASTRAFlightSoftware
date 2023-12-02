#include "Estimator.h"
#include "../math/Integrator.h"
#include "../error/Error.h"
#include "../drivers/ASTRA/IMU/src/IMU.h"
#include <ArduinoEigenDense.h>

Eigen::VectorXd estimatedStateX(STATE_DIMENSION);
Integrator linearAccelIntegrator;

int initializeEstimator() {

    for (unsigned int i = 0; i < STATE_DIMENSION; i++) {
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