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

//currently set to arbritrary value
//unit is in microseconds
//TODO: decide if this variable should be a constant or an assigned variable upon the calling of integratorSetup
#define MAX_ALLOWED_TIME_BETWEEN_INTEGRATION 10

//TODO: I get a redefinition error if the two static variables below are uncommented
    //I have not figured out why - Vincent
//static Eigen::VectorXd* dataToIntegrate;

static elapsedMicros timeBetweenIntegration;


//result of integration
extern Eigen::VectorXd integratedData;

//assigns pointer to dataToIntegrate, initializes integratedData
//returns error code
extern int integratorSetup(Eigen::VectorXd*); 

//updates integratedData with the integration of dataToIntegrate
//returns error code
extern int integratorUpdate(); 

#endif