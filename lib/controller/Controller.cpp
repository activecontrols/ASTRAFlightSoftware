#include "Controller.h"
#include "Error.h"
#include "../estimator/Estimator.h"
#include "../trajectory/SDcard.h"
#include "../math/LinearInterpolator.h"
#include "../timer/timer.h"
#include <Arduino.h>
#include <Servo.h>

/*
Controller.cpp  
Description: Defines all functions for the controller, including those declared in Controller.h
Author: Vincent Palmerio
*/

Eigen::VectorXd controllerInputU(U_ROW_LENGTH);
Eigen::MatrixXd kGain(U_ROW_LENGTH, X_VECTOR_LENGTH + ERROR_VECTOR_LENGTH);
Eigen::MatrixXd qsGain(U_ROW_LENGTH, X_VECTOR_LENGTH);

Eigen::VectorXd deltaX(X_VECTOR_LENGTH);
Eigen::VectorXd xRef{X_VECTOR_LENGTH};
Eigen::VectorXd uRef{U_ROW_LENGTH};

Eigen::VectorXd xSnap{X_VECTOR_LENGTH};
Eigen::VectorXd deltaXSnap(X_VECTOR_LENGTH);

Integrator zIntegrationObject;
Integrator zSnapIntegrationObject;

Interpolator xTrajInterpolator = *new Interpolator(X_VECTOR_LENGTH + ERROR_VECTOR_LENGTH);
Interpolator uTrajInterpolator = *new Interpolator(U_ROW_LENGTH);

Servo innerGimbal;
Servo outerGimbal;
Servo torqueVaneLeft;
Servo torqueVaneRight;

elapsedMicros xSnapTimer;

//time offset after switching out of stability control mode
//Must be in seconds!
double timeOffset = 0;

/* Trajectory */

//Represents the index of the time that is the first point being linearly interpolated (x1)
//x2 is traj::t[currentTimeIndex + 1]
int currentTimeIndex = 0;


int initializeController() {

    zIntegrationObject = *new Integrator();

    zIntegrationObject.integratorSetup(&deltaX);

    deltaX.setZero();
    xRef.setZero();
    xSnap.setZero();
    deltaX.setZero();
    controllerInputU.setZero();
    kGain.setConstant(1);

    if (traj::m != U_ROW_LENGTH) {
        return ROW_KGAIN_MISMATCH;
    }

    if ((traj::n + traj::N) != X_VECTOR_LENGTH + ERROR_VECTOR_LENGTH) {
        return COLUMN_KGAIN_MISMATCH;
    }

    if (traj::n != X_VECTOR_LENGTH) {
        return COLUMN_QSM_MISMATCH;
    }

    float (*kGainMatrix)[traj::m][traj::n + traj::N] = (float (*)[traj::m][traj::n + traj::N]) traj::vgainM;
    float (*quickStabilizationMatrix)[traj::m][traj::n] = (float (*)[traj::m][traj::n]) traj::vqsm;

    for (int i = 0; i < U_ROW_LENGTH; i++) {
        for (int j = 0; j < X_VECTOR_LENGTH + ERROR_VECTOR_LENGTH; j++) {
            kGain(i, j) = *kGainMatrix[i][j];
        }
    }

    for (int i = 0; i < U_ROW_LENGTH; i++) {
        for (int j = 0; j < X_VECTOR_LENGTH; j++) {
            qsGain(i, j) = *quickStabilizationMatrix[i][j];
        }
    }

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
    loadTrajectoryPoint();
    controlLaw();
    saturation();
    controlServos();
    return NO_ERROR_CODE;
}

/*
 * Loads trajectory point the rocket should be at based current mission time
 * x is loaded into xRef and u is loaded into uRef
 */
int loadTrajectoryPoint() {
    float currentTime = getMissionTimeSeconds() - timeOffset;

    if (currentTimeIndex == (traj::k-1)) {
        //set mode to Regulation and Landing
        //return error or other code??
    }

    //The linear interpolation happens within a range: the time at currentTimeIndex and time at currentTimeIndex + 1
    //This variable represents the end of this range
    float endTimePeriod = traj::t[currentTimeIndex + 1];

    //if our time is greater than the time at currentTimeIndex + 1, find rhe next time range and linearly interpolate
    if (currentTime > endTimePeriod) {
        while (currentTime > endTimePeriod) {
            currentTimeIndex++;
            endTimePeriod = traj::t[currentTimeIndex + 1];
        }

        float (*x)[traj::n] = (float (*)[traj::n]) traj::vx;
        float* y1 = x[currentTimeIndex];
        float* y2 = x[currentTimeIndex + 1];

        xTrajInterpolator.rebuildLine(y1, y2, traj::t[currentTimeIndex], endTimePeriod);

        float (*u)[traj::m] = (float (*)[traj::m]) traj::vu;
        float* u1 = u[currentTimeIndex];
        float* u2 = u[currentTimeIndex + 1];

        uTrajInterpolator.rebuildLine(u1, u2, traj::t[currentTimeIndex], endTimePeriod);

    }

    xTrajInterpolator.calculatePoint(currentTime);
    xRef = xTrajInterpolator.resultY;

    uTrajInterpolator.calculatePoint(currentTime);
    uRef = uTrajInterpolator.resultY;
    
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
    //controlMode(&estimatedStateX, &xRef);
    //}
    controlLawRegulate();
    return NO_ERROR_CODE;

}

int switchControlStability() {
    //Get snapshot of estimatedStateX with just position (everything else zero) (position not implemented yet)
    //set xSnap to that snapshot
    //initialize xSnap integrator and set it to zSnapIntegrationObject
    // zSnapIntegrationObject = *new Integrator();
    // zSnapIntegrationObject.integratorSetup(&);
    //start a new timer and set it to xSnapTimer
    xSnapTimer = 0;
}

int switchControlTraj() {
    //if previous mode was Stability, set timeOffset to the timer xSnapTimer
        //make sure to convert xSnapTimer to seconds!
}

int controlMode() {
    
    //estimatedStateX and xRef

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

int controlLawTrack() {

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

    controllerInputU = (uRef) - (kGain * deltaXIntegratedX);

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

