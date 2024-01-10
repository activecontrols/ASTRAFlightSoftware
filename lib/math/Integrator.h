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
//unit is in seconds
//TODO: decide if this variable should be a constant or an assigned variable upon the calling of integratorSetup
#define MAX_ALLOWED_TIME_BETWEEN_INTEGRATION 10

class Integrator {

public:

    //result of integration
    Eigen::VectorXd integratedData;

    //assigns pointer to dataToIntegrate, initializes integratedData
    //returns error code
    //we aren't using a constructor because constructors can't return error codes
    int integratorSetup(Eigen::VectorXd*); 

    //updates integratedData with the integration of dataToIntegrate
    //returns error code
    int integratorUpdate(); 

private: 
    Eigen::VectorXd* dataToIntegrate;
    Eigen::VectorXd prevValue;
    Eigen::VectorXd prev2Value;
    elapsedMicros time;
    double dt;
    double dtPrev;

    Eigen::VectorXd simpson_nonuniform_cumulative(
        Eigen::VectorXd prev2_f, Eigen::VectorXd prev_f, Eigen::VectorXd new_f,
        double dt, double dtPrev) const;

};

#endif