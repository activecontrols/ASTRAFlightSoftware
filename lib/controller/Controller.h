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

#define U_ARRAY_LENGTH 12
#define MODE_ARRAY_LENGTH 12
#define K_ARRAY_LENGTH 10

//size of x
#define X_COLUMN_LENGTH 3
#define X_ROW_LENGTH 4


//Global variables
extern Eigen::VectorXd controllerInputU;
extern double *mode;
extern double *k;
extern Eigen::Matrix4Xd deltaX;

extern Eigen::VectorXd xRef;

//malloc's global variables in this header file, 
    //returns error code as int (NO_ERROR_CODE, GENERAL_ERROR_CODE, MEMORY_ALLOCATION_ERROR_CODE)
extern int initializeController();

//control law function that:
    //takes in estimatedStateX (from Estimator.h)
    //takes in k
    //takes in mode
    //modifies controllerInputU
    //returns error code
int updateController();

//calculates mode
    //takes in estimatedStateX
    //modifies mode
    //returns error code
int getControlMode();

#endif



