/*
Controller.h - Controller Header file
Description: Header file to Controller.cpp
Author: Ani Gokhale, Vincent Palmerio
Created: 9/13/2023
Last updated: 10/16/2023
*/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <ArduinoEigenDense.h>
#include "../math/Integrator.h"


#define MODE_ARRAY_LENGTH 12
#define K_ARRAY_LENGTH 10

//dimensions of deltaX matrix
#define X_VECTOR_LENGTH 4

//dimensions of controllerInputU matrix
#define U_ROW_LENGTH 12
#define U_COLUMN_LENGTH 12

//dimensions of kGain matrix
#define K_ROW_LENGTH 12
#define K_COLUMN_LENGTH 12

//Global variables
extern Eigen::Matrix4Xd controllerInputU;
extern double *k;
extern Eigen::MatrixXd kGain;
extern Eigen::VectorXd deltaX;

static Integrator zIntegrationObject;

enum K_GAIN {
    TRACK_K_GAIN = 1,
    STABALIZE_K_GAIN = 1,
    LAND_K_GAIN = 1
};

//malloc's global variables in this header file, 
    //returns error code as int (NO_ERROR_CODE, GENERAL_ERROR_CODE, MEMORY_ALLOCATION_ERROR_CODE)
extern int initializeController();

//control law function that:
    //takes in previous u (output)
    //accesses estimatedStateX (from Estimator.h)
    //accesses k
    //modifies controllerInputU (our output to servos, actuators, etc.)
    //returns error code
int controlLaw(Eigen::Matrix4Xd* uRef);

//set k to an value in the K_GAIN enum depending on the condition deltaX is in
    //returns error code
int controlMode(Eigen::Matrix4Xd* deltaX);

#endif



