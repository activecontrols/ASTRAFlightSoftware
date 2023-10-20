/*
Integrator.h - Integrator Header file
Description: Header file to Integrator.cpp
Author: Vincent Palmerio
Created: 10/20/2023
Last updated: 10/20/2023
*/


#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include <ArduinoEigenDense.h>

Eigen::VectorXd* dataToIntegrate;

elapsedMicros timeBetweenIntegration;

//currently set to arbritrary value
//unit is in microseconds
//TODO: decide if this variable should be a constant or an assigned variable upon the calling of integratorSetup
int maxAllowedTimeBetweenIntegration = 10;

extern Eigen::VectorXd integratedData;

//assigns pointer to dataToIntegrate, initializes integratedData
//returns error code
extern int integratorSetup(Eigen::VectorXd*); 

//updates integratedData with the integration of dataToIntegrate
//returns error code
extern int integratorUpdate(); 

#endif