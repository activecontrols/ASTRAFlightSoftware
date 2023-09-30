/*
Controller.h - Controller Header file
Description: Header file to Controller.cpp
Author: Ani Gokhale, Vincent Palmerio
Created: 9/13/2023
Last updated: 9/13/2023
*/

//Import ESTIMATORVARIABLEARRAYPOSITION constants to access "x" from controller
#include "../estimator/Estimator.h";


#define UARRAYLENGTH 12
#define MODEARRAYLENGTH 12
#define KARRAYLENGTH 10

int* (controllerInput_u)[UARRAYLENGTH];
int* (mode)[MODEARRAYLENGTH];
int* (k)[KARRAYLENGTH];

//control law function that 
    //takes in x (from Estimator.h)
    //takes in k
    //takes in mode
//modifies u
void controlLaw_u();

//takes in x
//modifies mode
void controlMode_mode();





