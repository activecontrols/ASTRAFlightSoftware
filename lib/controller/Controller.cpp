#include "Controller.h"
#include "Error.h"
#include "settings.h"
#include "Estimator.h"
#include "Encoder.h"
#include "Integrator.h"
#include "timer.h"
#if !REGULATE_ONLY
    #include "SDcard.h"
    #include "LinearInterpolator.h"
#endif

#include <Arduino.h>
#include <Servo.h>


/*
Controller.cpp  
Description: Defines all functions for the controller, including those declared in Controller.h
Author: Vincent Palmerio
*/

namespace controller {

    Eigen::VectorXd controllerInputU(U_ROW_LENGTH);
    Eigen::MatrixXd trajectoryGain(U_ROW_LENGTH, X_VECTOR_LENGTH + ERROR_VECTOR_LENGTH);
    Eigen::MatrixXd qsGain(U_ROW_LENGTH, X_VECTOR_LENGTH);

    Eigen::VectorXd deltaX(X_VECTOR_LENGTH);
    Eigen::VectorXd xRef(X_VECTOR_LENGTH);
    Eigen::VectorXd uRef(U_ROW_LENGTH);

    Eigen::VectorXd xSnap{X_VECTOR_LENGTH};
    Eigen::VectorXd deltaXSnap(X_VECTOR_LENGTH);

    Integrator zIntegrationObject;
    Integrator zSnapIntegrationObject;

#if !REGULATE_ONLY  
    Interpolator xTrajInterpolator = *new Interpolator(X_VECTOR_LENGTH + ERROR_VECTOR_LENGTH);
    Interpolator uTrajInterpolator = *new Interpolator(U_ROW_LENGTH);
#endif

    Servo innerGimbal;
    Servo outerGimbal;
    Servo torqueVaneLeft;
    Servo torqueVaneRight;

    elapsedMillis xNonTrajTimer;

    int controlModeIndicator = 0;

    double timeOffset = 0; /* time offset after switching out of stability control mode, unit is in seconds!! */

    /*
     * Represents the index of the time that is the first point being linearly interpolated (x1)
     * x2 is traj::t[currentTimeIndex + 1] 
     */
    int currentTimeIndex = 0;

    double offset = 0;

    int initializeController() {

        zIntegrationObject = *new Integrator();

        zIntegrationObject.integratorSetup(&deltaX, X_VECTOR_LENGTH);

        deltaX.setZero();
        
        controllerInputU.setZero();
        
        qsGain << 0.010175896789397, -0.999948223723789, -0.000031563385498, 0.010270474752111, -1.009237389930116, -0.000033700327955, 
                -0.006293635967089, -0.000032482311507, -0.999980194349455, -0.006350485144209, -0.000034619292106, -1.009010144715387, 
                0.000000000000000, 0.000000000000001, -0.000000000000003, 0.000000000000000, -0.000000000000000, 0.000000000000004, 
                99.992841807351638, 1.017589389543643, -0.629364064139043, 99.992889040319923, 1.017583962544098, -0.629360802065534;

#if !REGULATE_ONLY
        uRef.setZero();
        xRef.setZero();
        trajectoryGain.setZero();
        if (traj::m != U_ROW_LENGTH) {
            return ROW_KGAIN_MISMATCH;
        }

        if ((traj::n + traj::N) != X_VECTOR_LENGTH + ERROR_VECTOR_LENGTH) {
            return COLUMN_KGAIN_MISMATCH;
        }

        if (traj::n != X_VECTOR_LENGTH) {
            return COLUMN_QSM_MISMATCH;
        }

        float (*trajectoryGainMatrix)[traj::m][traj::n + traj::N] = (float (*)[traj::m][traj::n + traj::N]) traj::vgainM;
        float (*quickStabilizationMatrix)[traj::m][traj::n] = (float (*)[traj::m][traj::n]) traj::vqsm;

        for (int i = 0; i < U_ROW_LENGTH; i++) {
            for (int j = 0; j < X_VECTOR_LENGTH + ERROR_VECTOR_LENGTH; j++) {
                trajectoryGain(i, j) = *trajectoryGainMatrix[i][j];
            }
        }

        for (int i = 0; i < U_ROW_LENGTH; i++) {
            for (int j = 0; j < X_VECTOR_LENGTH; j++) {
                qsGain(i, j) = *quickStabilizationMatrix[i][j];
            }
        }
#endif
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

        //ENCODER SETUP
#if USE_ENCODER
        while (!encoder::encoderSetup()) {
            Serial.println("Connecting to encoder...");
        }
#endif

        innerGimbal.write(INNER_GIMBAL_INITIAL_SETTING);
        outerGimbal.write(OUTER_GIMBAL_INITIAL_SETTING);
        torqueVaneLeft.write(LEFT_TORQUE_VANE_INITIAL_SETTING);
        torqueVaneRight.write(RIGHT_TORQUE_VANE_INITIAL_SETTING);

        return NO_ERROR_CODE;
    }

    int updateController() {
#if !REGULATE_ONLY
        getDeltaX(&estimator::estimatedStateX, &xRef);
#endif
        controlLaw();
        saturation();
        controlServos();
        return NO_ERROR_CODE;
    }

    int controlServos() {

        innerGimbal.write(controllerInputU(0) + INNER_GIMBAL_INITIAL_SETTING); //write gamma to inner gimbal
        outerGimbal.write(controllerInputU(1) + OUTER_GIMBAL_INITIAL_SETTING); //write beta to outer gimbal
        
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
        //controlMode(&estimator::estimatedStateX, &xRef);
        //}

#if !REGULATE_ONLY
        switch (controlModeIndicator) {
            case TRACK_MODE:
                loadTrajectoryPoint();
                controlLawTrack();
                break;
            case STABILIZE_MODE:
                controlLawStability();
                break;
            case LAND_MODE:
                controlLawLand();
                break;
            //case FINAL_APPROACH_MODE:
                // controlLawFinalApproach();
                // break;
            default:
                //something went really bad.. try to land safely
                controlLawRegulate();
                break;
        }
#else
        controlLawRegulate();
#endif

        float currentTime = timer::getMissionTimeSeconds();
        if (currentTime < 1.5) {
            controllerInputU(2) = 100;
        } else if (currentTime < 100) {
            //Do nothing
        } else if (currentTime < 101) {
            controllerInputU(2) = 90;
        } else {
            controllerInputU(2) = 0;
        }

        float beta_dot = encoder::magEncoder1.getAngularSpeed();
        float gamma_dot = encoder::magEncoder2.getAngularSpeed();
        
        controllerInputU(1) = controllerInputU(1) + BETA_DAMPENING_CONSTANT*beta_dot;
        controllerInputU(0) = controllerInputU(0) + GAMMA_DAMPENING_CONSTANT*gamma_dot;

        return NO_ERROR_CODE;

    }

    int controlLawRegulate() {

        controllerInputU = -(qsGain * estimator::estimatedStateX);
        Serial.print("Controller Multiplication: ");
        for (byte i = 0; i < ESTIMATED_STATE_DIMENSION; i++) {
            Serial.print( -(qsGain(0, i) * estimator::estimatedStateX(i)), 5);
            Serial.print(", ");
        }
        
        Serial.println();
        Serial.print(qsGain(0, 2));
        Serial.print(estimator::estimatedStateX(2));
        Serial.println();

        controllerInputU(0) = 180.0*controllerInputU(0)/PI;
        Serial.print("Controller Gamma: ");
        Serial.println(controllerInputU(0), 3);

        controllerInputU(1) = 180.0*controllerInputU(1)/PI;
        Serial.print("Controller Beta: ");
        Serial.println(controllerInputU(1), 3);

        return NO_ERROR_CODE;
    }

#if !REGULATE_ONLY
  
  /*
   * Loads trajectory point the rocket should be at based current mission time
   * x is loaded into xRef and u is loaded into uRef
   */
  int loadTrajectoryPoint() {
      float currentTime = timer::getMissionTimeSeconds() - timeOffset;

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

  
    int switchControlStability() {
        
        //if deltaX == some condition for controlLawStability
        //Get snapshot of estimatedStateX with just position (everything else zero) (position not implemented yet)
        //set xSnap to that snapshot
        xSnap.setZero();
        
        //reset timer to determine how much time in this control mode
        xNonTrajTimer = 0;
        controlModeIndicator = STABILIZE_MODE;

        return NO_ERROR_CODE;
    }

    int switchControlTraj() {

        if ((controlModeIndicator == STABILIZE_MODE) || (controlModeIndicator == LAND_MODE)) {
            //convert xSnapTimer to seconds!
            timeOffset = xNonTrajTimer / 1000;
            zIntegrationObject = *new Integrator();
            zIntegrationObject.integratorSetup(&deltaX, X_VECTOR_LENGTH);
        }

        controlModeIndicator = TRACK_MODE;
        return NO_ERROR_CODE;
    }

    int switchControlReg() {

        //reset timer to determine how much time in this control mode
        xNonTrajTimer = 0;
        controlModeIndicator = LAND_MODE;

        return NO_ERROR_CODE;
    }

    int controlMode() {
        
        //estimatedStateX and xRef

        //if deltaX == some condition for controlLawStability
            //switchControlStability()

        //if deltaX == some condition for controlLawTrack
        //if previously in controlLawStability,
            //end timer, set timer value to offset

        return NO_ERROR_CODE;
    }

    int controlLawTrack() {

        getDeltaX(&estimator::estimatedStateX, &xRef);

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

        controllerInputU = (uRef) - (qsGain * deltaXIntegratedX);

        return NO_ERROR_CODE;
    }

    int controlLawStability() {

        getDeltaX(&estimator::estimatedStateX, &xSnap);

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

        controllerInputU = - (qsGain * deltaXIntegratedX);

        return NO_ERROR_CODE;
    }
#endif

    int controlLawLand() {
        return NO_ERROR_CODE;
    }
  
    Eigen::VectorXd getControlInputs() {
        return controllerInputU;
    }

}


//UNUSED
//-----------------------------------------------------------------------------------------
// Eigen::Quaterniond getConstrainedQuaternion(Eigen::Matrix4Xd* v) {
//     //magnitude constraint of quaternion
//     //prone to precision float errors
//     double constraint = std::sqrt(
//         1-
//         (*v)(2,0)*(*v)(2,0)-
//         (*v)(2,1)*(*v)(2,1)-
//         (*v)(2,2)*(*v)(2,2)
//     );

//     Eigen::Quaterniond e(
//         constraint,
//         v[2](0),
//         v[2](1),
//         v[2](0)
//     );
//     return e;
// }

// int getDeltaX(Eigen::Matrix4Xd* x, Eigen::Matrix4Xd* xRef) {

//     //magnitude of vector in first row
//     deltaX(0) = ((*x).row(0) - (*xRef).row(0)).norm();

//     //magnitude of vector in second row
//     deltaX(1) = ((*x).row(1) - (*xRef).row(1)).norm();

//     Eigen::Quaterniond e = getConstrainedQuaternion(x) * getConstrainedQuaternion(xRef);
//     //theta (vector of length 1)
//     deltaX(2) = 2*std::acos(e.w());

//     //magnitude of vector in fourth row
//     deltaX(3) = ((*x).row(3) - (*xRef).row(3)).norm();

//     return NO_ERROR_CODE;
// }
