/*
Derivative.h - Derivative Header file
Description: Header file to Derivative.cpp
Author: Vincent Palmerio
Created: 10/20/2023
Last updated: 10/20/2023
*/

#ifndef DERIVATIVE_H
#define DERIVATIVE_H

#include <ArduinoEigenDense.h>


static Eigen::VectorXd* newestData;
static Eigen::VectorXd previousData;
static elapsedMicros timeStepDerivative;

extern Eigen::VectorXd derivative;

//assings pointer to newestData, initializes previousData to specified vector, initializes derivative
//returns error code
extern int derivativeSetup(Eigen::VectorXd*, Eigen::VectorXd);

//updates the derivative variable with the derivative of between newestData and previousData
extern int derivativeUpdate(); 

#endif



