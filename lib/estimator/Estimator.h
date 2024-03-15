/* 
Estimator.h - Estimator Header file
Description: Header file to Estimator.cpp
Author: Vincent Palmerio
*/

#ifndef ESTIMATOR_H
#define ESTIMATOR_H

#include <ArduinoEigen.h>

#define ESTIMATED_STATE_DIMENSION (6)
#define MEASUREMENT_DIMENSION (9)

extern int initializeEstimator(); 

extern Eigen::VectorXd estimatedStateX;
extern Eigen::VectorXd measurementVectorY;
extern Eigen::VectorXd initialQuaternion;
extern Eigen::VectorXd initialAcceleration;
extern Eigen::VectorXd earthFrameAcceleration;
extern Eigen::VectorXd velocityBodyFrame;

int updateEstimator(); 

float kalmanFilter();

#endif
