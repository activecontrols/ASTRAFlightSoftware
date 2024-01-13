/*
Integrator.h - Integrator Header file
Description: Header file to Integrator.cpp
Author: Ishan Goel, Vincent Palmerio
Created: 10/20/2023
Last updated: 01/10/2024 by Ishan Goel <goeli@purdue.edu>
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
// We now use Simpson's rule for integration. Here's a comparison of its accuracy against other common techniques:
// https://cplayground.com/?p=salmon-cat-turkey. (Simpson's is orders of magnitude better)
//
// Simpson's also has the advantage that it integrates quadratics _exactly_ (because it does a quadratic fit on each
// step), and since much real world data is quadratic in nature (eg. falling, forces being applied, etc.) Simpson's rule
// gives way better results than other techniques.
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
