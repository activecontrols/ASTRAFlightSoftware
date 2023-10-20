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


Eigen::VectorXd* newestData;

Eigen::VectorXd previousData;

extern Eigen::VectorXd derivative;

//assings pointer to newestData, initializes previousData and derivative
//returns error code
extern int derivativeSetup(Eigen::VectorXd*);

//updates the derivative variable with the derivative of between newestData and previousData
extern int derivativeUpdate(); 

#endif



