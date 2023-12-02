/* 
Estimator.h - Estimator Header file
Description: Header file to Estimator.cpp
Author: Vincent Palmerio
Created: 9/18/2023
Last updated: 10/16/2023
*/

#ifndef ESTIMATOR_H
#define ESTIMATOR_H

#include <ArduinoEigenDense.h>

#define MEASUREMENT_INPUT_LENGTH 9 
#define STATE_DIMENSION 7

//malloc's global variables in this header file
    //returns error code as int (NO_ERROR_CODE, GENERAL_ERROR_CODE, MEMORY_ALLOCATION_ERROR_CODE)
extern int initializeEstimator(); 

//global variable
    //will be accessible to controller.h file
    //variables in the array correspond to ESTIMATOR_VARIABLE_ARRAY_POSITION values
extern Eigen::VectorXd estimatedStateX;

//calculates estimatedStateX
    //updates estimatedStateX
    //accesses measurement data (y) in buffer class
    //accesses controller inputs (u) in Controller.h
    //returns error code as int (NO_ERROR_CODE, GENERAL_ERROR_CODE)
int updateEstimator(); 

float kalmanFilter();

#endif
