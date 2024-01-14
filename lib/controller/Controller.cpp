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

Eigen::VectorXd xSnap{X_VECTOR_LENGTH};
Eigen::VectorXd deltaXSnap(X_VECTOR_LENGTH);

Integrator zIntegrationObject;
Integrator zSnapIntegrationObject;

Servo innerGimbal;
Servo outerGimbal;
Servo torqueVaneLeft;
Servo torqueVaneRight;

elapsedMicros xSnapTimer;

double offset = 0;

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
    torqueVaneLeft.attach(LEFT_TORQUE_VANE_PIN);
    torqueVaneRight.attach(RIGHT_TORQUE_VANE_PIN);

    if (!innerGimbal.attached()) {
        return INNER_GIMBAL_NOT_ATTACHED;
    }

    if (!outerGimbal.attached()) {
        return OUTER_GIMBAL_NOT_ATTACHED;
    }

    if (!torqueVaneLeft.attached()) {
        return LEFT_TORQUE_VANE_NOT_ATTACHED;
    }

    if (!torqueVaneRight.attached()) {
        return RIGHT_TORQUE_VANE_NOT_ATTACHED;
    }

    innerGimbal.write(INNER_GIMBAL_INITIAL_SETTING);
    outerGimbal.write(OUTER_GIMBAL_INITIAL_SETTING);
    torqueVaneLeft.write(LEFT_TORQUE_VANE_INITIAL_SETTING);
    torqueVaneRight.write(RIGHT_TORQUE_VANE_INITIAL_SETTING);

    return NO_ERROR_CODE;
}

int updateController() {
    //getDeltaX(&estimatedStateX, &xRef);
    controlLaw();
    saturation();
    controlServos();
    return NO_ERROR_CODE;
}

int controlServos() {

    innerGimbal.write(controllerInputU(1) + INNER_GIMBAL_INITIAL_SETTING); //write gamma to inner gimbal
    outerGimbal.write(controllerInputU(0) + OUTER_GIMBAL_INITIAL_SETTING); //write beta to outer gimbal
    torqueVaneLeft.write(controllerInputU(3) + LEFT_TORQUE_VANE_INITIAL_SETTING); //write alpha to left torque vane
    torqueVaneRight.write(-controllerInputU(3) + RIGHT_TORQUE_VANE_INITIAL_SETTING); //write -alpha to right torque vane

    return NO_ERROR_CODE;
}

int getDeltaX(Eigen::VectorXd* x, Eigen::VectorXd* xRef) {
    deltaX = (*x)-(*xRef);

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

//determines which control function to use based on priority:
//1. comms commands
//   - Calls controlLaw()
//2. analysis of estimated state
//   - Calls controlMode()
int controlLaw() {

    //if comms
    //else {
    controlMode(&estimatedStateX, &xRef);
    //}

}

int controlMode(Eigen::VectorXd* x, Eigen::VectorXd* xRef) {
    
    //if deltaX == some condition for controlLawStability
    //Get snapshot of estimatedStateX with just position (everything else zero) (position not implemented yet)
    //set xSnap to that snapshot
    //initialize xSnap integrator and set it to zSnapIntegrationObject
    //start a new timer and set it to xSnapTimer

    //if deltaX == some condition for controlLawTrack
    //if previously in controlLawStability,
        //end timer, set timer value to offset

    return NO_ERROR_CODE;
}

int controlLawRegulate() {

    controllerInputU = -(kGain * estimatedStateX);

    return NO_ERROR_CODE;
}

int controlLawTrack(Eigen::Matrix4Xd* uRef) {

    getDeltaX(&estimatedStateX, &xRef);

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

int controlLawStability() {

    getDeltaX(&estimatedStateX, &xSnap);

    //update the integrator now that a new deltaX has been set
    //the integrator is being updated here because this is the closest point
    //between the integration of the data and the usage of the data. 
    //we don't want there to be any gaps in time where the integrator is not updated
    int errorCode = zSnapIntegrationObject.integratorUpdate();

    if (errorCode != 0) {
        return errorCode;
    }
    
    Eigen::Matrix2Xd deltaXIntegratedX;
    
    //deltaX in first row
    for (int i = 0; i < deltaX.size(); i++) {
        deltaXIntegratedX(0, i) = deltaX(i);
    }

    //integrated X in second row
    for (int i = 0; i < zSnapIntegrationObject.integratedData.size(); i++) {
        deltaXIntegratedX(0, i) = zSnapIntegrationObject.integratedData(i);
    }

    controllerInputU = - (kGain * deltaXIntegratedX);

    return NO_ERROR_CODE;
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

