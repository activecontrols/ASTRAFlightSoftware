/* 
Estimator.h - Estimator Header file
Description: Header file to Estimator.cpp
Author: Vincent Palmerio
*/

#ifndef ESTIMATOR_H
#define ESTIMATOR_H

#include <ArduinoEigenDense.h>

#define MEASUREMENT_INPUT_LENGTH 9 
#define STATE_DIMENSION 7

//malloc's global variables in this header file
    //returns error code as int (NO_ERROR_CODE, GENERAL_ERROR_CODE, MEMORY_ALLOCATION_ERROR_CODE)
extern int initializeEstimator(); 

extern Eigen::VectorXd estimatedStateX;

int updateEstimator(); 

float kalmanFilter();

#endif
