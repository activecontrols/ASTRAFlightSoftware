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

//TODO: I get a redefinition error if the two static variables below are uncommented
    //I have not figured out why - Vincent
static Eigen::VectorXd* newestData;
static Eigen::VectorXd previousData;
static elapsedMicros timeBetweenIntegration;

extern Eigen::VectorXd derivative;

//assings pointer to newestData, initializes previousData and derivative
//returns error code
extern int derivativeSetup(Eigen::VectorXd*);

//updates the derivative variable with the derivative of between newestData and previousData
extern int derivativeUpdate(); 

#endif



