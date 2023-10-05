/*
Controller.h - Controller Header file
Description: Header file to Controller.cpp
Author: Ani Gokhale, Vincent Palmerio
Created: 9/13/2023
Last updated: 9/13/2023
*/

#ifndef CONTROLLER_H
#define CONTROLLER_H


#define U_ARRAY_LENGTH 12
#define MODE_ARRAY_LENGTH 12
#define K_ARRAY_LENGTH 10


//Global variables
extern float *controllerInput_u;
extern float *mode;
extern float *k;

//malloc's global variables in this header file, 
    //returns error code as int (NO_ERROR_CODE, GENERAL_ERROR_CODE, MEMORY_ALLOCATION_ERROR_CODE)
extern int controllerSetup();

//control law function that:
    //takes in estimatedStateX (from Estimator.h)
    //takes in k
    //takes in mode
    //modifies controllerInputU
    //returns error code
int controlLawU();

//calculates mode
    //takes in estimatedStateX
    //modifies mode
    //returns error code
int controlMode();

#endif


