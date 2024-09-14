#include "Estimator.h"
#include "Integrator.h"
#include "Error.h"
#include "IMU.h"
#include "MathFunctions.h"
#include "TOF.h"
#include "settings.h"

#include <ArduinoEigenDense.h>

/*
Estimator.cpp
Description:  Defines all functions for the estimator, including those declared in Estimator.h
Author: Vincent Palmerio
*/

namespace estimator {
    Eigen::VectorXd estimatedStateX(ESTIMATED_STATE_DIMENSION);

    Eigen::VectorXd measurementVectorY(MEASUREMENT_DIMENSION);
    Eigen::VectorXd initialQuaternion(4);
    Eigen::VectorXd initialAcceleration(3);
    Eigen::VectorXd earthFrameAcceleration(3);
    Eigen::VectorXd velocityBodyFrame(3);

    Integrator linearAccelIntegrator;
    Integrator bodyFrameVelocityIntegrator;

    Eigen::Vector4d rotationConjugate(4);

    /*
    * Initializes estimation library, including malloc'ing global variables
    * returns error code as int (NO_ERROR_CODE, GENERAL_ERROR_CODE, MEMORY_ALLOCATION_ERROR_CODE)
    */
    int initializeEstimator() {

        //IMU SETUP
        int errorCode = initializeIMU();
        while(errorCode != 0) {
            Serial.print("Failed to initialize IMU, error code: ");
            Serial.print(errorCode);
            Serial.println(". Retrying...");
            errorCode = initializeIMU();
        }
        //- --

        estimatedStateX.setZero();
        
        for (byte i = 0; i < 10000; i++) {
            //TODO: Add a delay here to allow the kalman filter to calibrate to loop time
            updateIMU();
        }
    
        initialAcceleration << linearAccelVector;
        initialQuaternion << qw, qx, qy, qz;

        rotationConjugate = math::quaternionConjugate(initialQuaternion);

        math::calculateCBI(initialQuaternion);

        initialAcceleration << math::CBI * initialAcceleration;

        earthFrameAcceleration << (math::CBI * linearAccelVector) - initialAcceleration;

        //TOF SETUP
    #if (USE_TOF_SENSOR)
        int tofErrorCode = tof::setupTOFSensor(TOF_INTERRUPT_PIN, TOF_XSHUT_PIN);
        while (tofErrorCode != 0) {
            Serial.print("Failed to initialize TOF Sensor, error code: ");
            Serial.print(tofErrorCode);
            Serial.println(". Retrying...");
            tofErrorCode = tof::setupTOFSensor(TOF_INTERRUPT_PIN, TOF_XSHUT_PIN);
        }
    #endif

        for (unsigned int i = 0; i < ESTIMATED_STATE_DIMENSION; i++) {
            estimatedStateX(i) = 0;
        }
        measurementVectorY.setZero();

        velocityBodyFrame.setZero();

        bodyFrameVelocityIntegrator.integratorSetup(&velocityBodyFrame, velocityBodyFrame.size());
        linearAccelIntegrator.integratorSetup(&earthFrameAcceleration, earthFrameAcceleration.size());
        
        return NO_ERROR_CODE;
    }

    int updateEstimator() {
        updateIMU();

    #if USE_TOF_SENSOR
        tof::TOF_buffer.addData();
        float* d_m_star = tof::TOF_buffer.getAverage();

        Eigen::VectorXd TOFDist(1);

        TOFDist << d_m_star[0];
        TOFDist << 0;

        free(d_m_star);
        d_m_star = NULL;
    #endif


        Eigen::Vector4d measuredQuaternion(4);
        measuredQuaternion << qw, qx, qy, qz;

        Eigen::VectorXd currentQuaternion(4);
        currentQuaternion << math::quaternionProduct(math::quaternionConjugate(initialQuaternion), measuredQuaternion);
        Eigen::VectorXd omega(3);
        omega << gx, gy, gz;

        math::calculateCBI(currentQuaternion);
        earthFrameAcceleration << math::CBI*linearAccelVector - initialAcceleration;

        bodyFrameVelocityIntegrator.integratorUpdate();

        linearAccelIntegrator.integratorUpdate();

        velocityBodyFrame << bodyFrameVelocityIntegrator.integratedData(0),
                                bodyFrameVelocityIntegrator.integratedData(1),
                                bodyFrameVelocityIntegrator.integratedData(2);


        Eigen::Vector4d realQuaternion(4);
        realQuaternion = math::quaternionProduct(measuredQuaternion, rotationConjugate);
        

        Serial.println("Measured Quaternion: ");
        Serial.print(measuredQuaternion(1), 5);
        Serial.print(", ");
        Serial.print(measuredQuaternion(2), 5);
        Serial.print(", ");
        Serial.print(measuredQuaternion(3), 5);
        Serial.println();

        estimatedStateX(0) = measuredQuaternion(1);
        estimatedStateX(1) = measuredQuaternion(2);
        estimatedStateX(2) = measuredQuaternion(3);

        estimatedStateX(3) = gy;
        estimatedStateX(4) = gx;
        estimatedStateX(5) = gz;

        measurementVectorY << linearAccelVector, gyroscopeVector, magnetometerVector;

        return NO_ERROR_CODE;
    }
}