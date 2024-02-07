/*
Controller.h - Controller Header file
Description: Header file to Controller.cpp
Author: Vincent Palmerio
*/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <ArduinoEigenDense.h>

#include "Integrator.h"

#define MODE_ARRAY_LENGTH (12)
#define K_ARRAY_LENGTH (10)
#define X_VECTOR_LENGTH (7)
#define ERROR_VECTOR_LENGTH (X_VECTOR_LENGTH) /* integrated X_VECTOR that tells us our error */
#define U_ROW_LENGTH (4) /* dimensions of controllerInputU (vector for controlling servos and torque) */
#define K_ROW_LENGTH (12) /* row dimension of kGain (tracking) matrix */
#define K_COLUMN_LENGTH (12) /* dimensions of kGain (tracking) matrix */
#define BETA_MAX (8) /* outer gimbal max*/
#define BETA_MIN (-BETA_MAX) /* outer gimbal min */
#define GAMMA_MAX (8) /* inner gimbal max */
#define GAMMA_MIN (-GAMMA_MAX) /* inner gimbal min */
#define THROTTLE_MIN (0)
#define THROTTLE_MAX (1)
#define ALPHA_MAX (8) /* left and right torque vane max */
#define ALPHA_MIN (-ALPHA_MAX) /* left and right torque vane min */
#define INNER_GIMBAL_PIN (0)
#define OUTER_GIMBAL_PIN (0)
#define LEFT_TORQUE_VANE_PIN (0)
#define RIGHT_TORQUE_VANE_PIN (0)
#define INNER_GIMBAL_INITIAL_SETTING (100)
#define OUTER_GIMBAL_INITIAL_SETTING (100)
#define LEFT_TORQUE_VANE_INITIAL_SETTING (145)
#define RIGHT_TORQUE_VANE_INITIAL_SETTING (140)

/* Global variables */

extern Eigen::VectorXd controllerInputU;
extern double *k;
extern Eigen::MatrixXd kGain;
extern Eigen::VectorXd deltaX;
extern int controlModeIndicator;

enum CONTROL_MODE {
    TRACK_MODE = 1,
    STABILIZE_MODE = 2,
    LAND_MODE = 3,
    FINAL_APPROACH_MODE = 4
};

extern Eigen::VectorXd xRef;

extern int initializeController();

extern int updateController();

int getDeltaX(Eigen::VectorXd*, Eigen::VectorXd*);
int controlLaw();
int controlLawRegulate();
int controlLawTrack();
int controlLawStability();
int controlLawLand();
int saturation();
int controlServos();
int loadTrajectoryPoint();
double minMax(double value, double min, double max);

int controlMode(Eigen::VectorXd* x, Eigen::VectorXd* xRef);
int controlModeUpdate(int controlModeIndicator);

#endif



