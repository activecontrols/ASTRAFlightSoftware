/* 
Estimator.h - Estimator Header file
Description: Header file to Estimator.cpp
Author: Vincent Palmerio
*/

#ifndef ESTIMATOR_H
#define ESTIMATOR_H

#include <ArduinoEigenDense.h>

#define ESTIMATED_STATE_DIMENSION (7)


extern int initializeEstimator(); 

extern Eigen::VectorXd estimatedStateX;

int updateEstimator(); 

float kalmanFilter();

#endif
