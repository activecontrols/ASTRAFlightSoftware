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

namespace estimator {
    int initializeEstimator(); 

    extern Eigen::VectorXd estimatedStateX;

    int updateEstimator(); 

    float kalmanFilter();
}

#endif
