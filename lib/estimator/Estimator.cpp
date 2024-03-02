#include "Estimator.h"
#include "Integrator.h"
#include "Error.h"
#include "IMU.h"
#include "MathFunctions.h"

#include <ArduinoEigenDense.h>

/*
Estimator.cpp
Description:  Defines all functions for the estimator, including those declared in Estimator.h
Author: Vincent Palmerio
*/

Eigen::VectorXd estimatedStateX(ESTIMATED_STATE_DIMENSION);
Eigen::VectorXd measurementVectorY(MEASUREMENT_DIMENSION);
Eigen::VectorXd initialQuaternion(4);
Eigen::VectorXd initialAcceleration(3);
Eigen::VectorXd earthFrameAcceleration(3);
Eigen::VectorXd velocityBodyFrame(3);
Integrator linearAccelIntegrator;
Integrator bodyFrameVelocityIntegrator;

int initializeEstimator() {
    // //IMU SETUP
    // int errorCode = initializeIMU();
    // while(errorCode != 0) { 
    //     Serial.print("Failed to initialize IMU, error code: ");
    //     Serial.print(errorCode);
    //     Serial.println(". Retrying...");
    //     errorCode = initializeIMU();
    // }

    // for (byte i = 0; i < 100; i++) {
    //     updateIMU();
    // }
    initialAcceleration << linearAccelVector;
    initialQuaternion << qw, qx, qy, qz;

    calculateCBI(initialQuaternion);

    initialAcceleration << CBI * initialAcceleration;

    earthFrameAcceleration << (CBI * linearAccelVector) - initialAcceleration;

    // //TOF SETUP
    // errorCode = setupTOFSensor(TOF_INTERRUPT_PIN, TOF_XSHUT_PIN);
    // while (errorCode != 0) {
    //     Serial.print("Failed to initialize TOF Sensor, error code: ");
    //     Serial.print(errorCode);
    //     Serial.println(". Retrying...");
    //     errorCode = setupTOFSensor(TOF_INTERRUPT_PIN, TOF_XSHUT_PIN);
    // }

    for (unsigned int i = 0; i < ESTIMATED_STATE_DIMENSION; i++) {
        estimatedStateX(i) = 0;
    }
    measurementVectorY.setZero();

    velocityBodyFrame.setZero();

    bodyFrameVelocityIntegrator.integratorSetup(&velocityBodyFrame, 3);
    linearAccelIntegrator.integratorSetup(&earthFrameAcceleration, 3);
    
    return NO_ERROR_CODE;
}

int updateEstimator() {
    updateIMU();
    // TOF_buffer.addData();
    // float* d_m_star = TOF_buffer.getAverage();

    // Eigen::VectorXd TOFDist(1);

    // TOFDist << d_m_star[0];
    // TOFDist << 0;

    // free(d_m_star);
    // d_m_star = NULL;

    Eigen::VectorXd measuredQuaternion(4);
    measuredQuaternion << qw, qx, qy, qz;
    // Eigen::VectorXd currentQuaternion(4);
    // currentQuaternion << quaternionProduct(quaternionConjugate(initialQuaternion), measuredQuaternion);
    // Eigen::VectorXd omega(3);
    // omega << gx, gy, gz;

    // calculateCBI(currentQuaternion);
    // earthFrameAcceleration << CBI*linearAccelVector - initialAcceleration;

    // bodyFrameVelocityIntegrator.integratorUpdate();

    // linearAccelIntegrator.integratorUpdate();

    // velocityBodyFrame << bodyFrameVelocityIntegrator.integratedData(0),
    //                         bodyFrameVelocityIntegrator.integratedData(1),
    //                         bodyFrameVelocityIntegrator.integratedData(2);

    // estimatedStateX(0) = linearAccelIntegrator.integratedData(0);
    // estimatedStateX(1) = linearAccelIntegrator.integratedData(1);
    // estimatedStateX(2) = linearAccelIntegrator.integratedData(2);

    estimatedStateX(0) = measuredQuaternion(1);
    estimatedStateX(1) = measuredQuaternion(2);
    estimatedStateX(2) = measuredQuaternion(3);

    estimatedStateX(3) = gy;
    estimatedStateX(4) = gx;
    estimatedStateX(5) = gz;

    measurementVectorY << linearAccelVector, gyroscopeVector, magnetometerVector;

    return NO_ERROR_CODE;
}