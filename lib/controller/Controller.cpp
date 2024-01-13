#include "Controller.h"
//#include "../estimator/Estimator.h"
#include "../error/Error.h"
#include "../estimator/Estimator.h"
#include <Arduino.h>
#include <Servo.h>
/*
Controller.cpp  
Description: Defines all functions for the controller, including those declared in Controller.h
Author: Vincent Palmerio
*/

Eigen::VectorXd controllerInputU(U_ROW_LENGTH);
double* k = (double *)malloc(K_ARRAY_LENGTH * sizeof(double));
Eigen::MatrixXd kGain(K_ROW_LENGTH, K_COLUMN_LENGTH);
Eigen::VectorXd deltaX(X_VECTOR_LENGTH);
Eigen::VectorXd xRef{{0,0,0,0,0,0,0}};

Servo innerGimbal;
Servo outerGimbal;
Servo torqueVane1;
Servo torqueVane2;

int initializeController() {

    zIntegrationObject = *new Integrator();

    zIntegrationObject.integratorSetup(&deltaX);

    //Check if memory allocation was successful
    if (k == NULL) {
        
        return MEMORY_ALLOCATION_ERROR_CODE; //Memory Allocation Error Code
    }

    // for (unsigned int i = 0; i < X_VECTOR_LENGTH; i++) {
    //     deltaX(i) = 0;
    // }
    deltaX.setZero();

    // for (unsigned int i = 0; i < U_ROW_LENGTH; i++) {
    //     controllerInputU(i) = 1;
    // }
    controllerInputU.setConstant(1);

    // for (unsigned int i = 0; i < K_ROW_LENGTH*K_COLUMN_LENGTH; i++) {
    //     kGain(i) = 1;
    // }
    kGain.setConstant(1);

    innerGimbal.attach(INNER_GIMBAL_PIN);
    outerGimbal.attach(OUTER_GIMBAL_PIN);
    torqueVane1.attach(TORQUE_VANE_1_PIN);
    torqueVane2.attach(TORQUE_VANE_2_PIN);

    if (!innerGimbal.attached()) {
        return INNER_GIMBAL_NOT_ATTACHED;
    }

    if (!outerGimbal.attached()) {
        return OUTER_GIMBAL_NOT_ATTACHED;
    }

    if (!torqueVane1.attached()) {
        return TORQUE_VANE_1_NOT_ATTACHED;
    }

    if (!torqueVane2.attached()) {
        return TORQUE_VANE_2_NOT_ATTACHED;
    }

    return NO_ERROR_CODE;
}

int updateController() {
    getDeltaX(&estimatedStateX, &xRef);
    controlLaw();
    saturation();
    controlServos();
    return NO_ERROR_CODE;
}

int controlServos() {

    innerGimbal.write(controllerInputU(1)); //write gamma to inner gimbal
    outerGimbal.write(controllerInputU(0)); //write beta to outer gimbal
    torqueVane1.write(controllerInputU(3)); //write alpha to torque vane 1
    torqueVane2.write(-controllerInputU(3)); //write -alpha to torque vane 2

    return NO_ERROR_CODE;
}

int getDeltaX(Eigen::VectorXd* x, Eigen::VectorXd* xRef) {
    deltaX = (*x)-(*xRef);

    return NO_ERROR_CODE;
}

int controlLaw() {

    controllerInputU = -(kGain * deltaX);

    return NO_ERROR_CODE;
}

int saturation() {

    //beta
    controllerInputU(0) = minMax(controllerInputU(0), BETA_MIN, BETA_MAX);

    //gamma
    controllerInputU(1) = minMax(controllerInputU(1), GAMMA_MIN, GAMMA_MAX);

    //throttle
    controllerInputU(2) = minMax(controllerInputU(2), THROTTLE_MIN, THROTTLE_MAX);

    //alpha
    controllerInputU(3) = minMax(controllerInputU(3), ALPHA_MIN, ALPHA_MAX);

    return NO_ERROR_CODE;
}

double minMax(double value, double min, double max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    } else {
        return value;
    }
}


//UNUSED
//-----------------------------------------------------------------------------------------
Eigen::Quaterniond getConstrainedQuaternion(Eigen::Matrix4Xd* v) {
    //magnitude constraint of quaternion
    //prone to precision float errors
    double constraint = std::sqrt(
        1-
        (*v)(2,0)*(*v)(2,0)-
        (*v)(2,1)*(*v)(2,1)-
        (*v)(2,2)*(*v)(2,2)
    );

    Eigen::Quaterniond e(
        constraint,
        v[2](0),
        v[2](1),
        v[2](0)
    );
    return e;
}

int getDeltaX(Eigen::Matrix4Xd* x, Eigen::Matrix4Xd* xRef) {

    //magnitude of vector in first row
    deltaX(0) = ((*x).row(0) - (*xRef).row(0)).norm();

    //magnitude of vector in second row
    deltaX(1) = ((*x).row(1) - (*xRef).row(1)).norm();

    Eigen::Quaterniond e = getConstrainedQuaternion(x) * getConstrainedQuaternion(xRef);
    //theta (vector of length 1)
    deltaX(2) = 2*std::acos(e.w());

    //magnitude of vector in fourth row
    deltaX(3) = ((*x).row(3) - (*xRef).row(3)).norm();

    return NO_ERROR_CODE;
}

int controlMode(Eigen::Matrix4Xd* deltaX) {
    
    return NO_ERROR_CODE;
}

int controlLaw(Eigen::Matrix4Xd* uRef) {

    //update the integrator now that a new deltaX has been set
    //the integrator is being updated here because this is the closest point
    //between the integration of the data and the usage of the data. 
    //we don't want there to be any gaps in time where the integrator is not updated
    int errorCode = zIntegrationObject.integratorUpdate();

    if (errorCode != 0) {
        return errorCode;
    }
    
    Eigen::Matrix2Xd deltaXIntegratedX;
    
    //deltaX in first row
    for (int i = 0; i < deltaX.size(); i++) {
        deltaXIntegratedX(0, i) = deltaX(i);
    }

    //integrated X in second row
    for (int i = 0; i < zIntegrationObject.integratedData.size(); i++) {
        deltaXIntegratedX(0, i) = zIntegrationObject.integratedData(i);
    }

    controllerInputU = (*uRef) - (kGain * deltaXIntegratedX);

    return NO_ERROR_CODE;
}


