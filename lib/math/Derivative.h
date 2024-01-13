/*
Derivative.h - Derivative Header file
Description: Header file to Derivative.cpp
Author: Vincent Palmerio
*/

#ifndef DERIVATIVE_H
#define DERIVATIVE_H

#include <ArduinoEigenDense.h>


class Derivative {

public:
    Eigen::VectorXd derivative;

    //assings pointer to newestData, initializes previousData to specified vector, initializes derivative
    //returns error code
    int derivativeSetup(Eigen::VectorXd*, Eigen::VectorXd);

    //updates the derivative variable with the derivative of between newestData and previousData
    int derivativeUpdate(); 

private:
    Eigen::VectorXd* newestData;
    Eigen::VectorXd previousData;
    elapsedMicros timeStepDerivative;

};

#endif

