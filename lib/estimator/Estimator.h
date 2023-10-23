/* 
Estimator.h - Estimator Header file
Description: Header file to Estimator.cpp
Author: Vincent Palmerio
Created: 9/18/2023
Last updated: 10/16/2023
*/

#ifndef ESTIMATOR_H
#define ESTIMATOR_H


#define MEASUREMENT_INPUT_LENGTH 9 
#define STATE_DIMENSION 12

//12 sensors and their positions in the array accessed from the class handling sensors
//(replace SENSOR{Number} with name of sensor)
#define SENSOR1_MEASUREMENT_ARRAY_POSITION = 0
#define SENSOR2_MEASUREMENT_ARRAY_POSITION = 1
#define SENSOR3_MEASUREMENT_ARRAY_POSITION = 2
#define SENSOR4_MEASUREMENT_ARRAY_POSITION = 3
#define SENSOR5_MEASUREMENT_ARRAY_POSITION = 4
#define SENSOR6_MEASUREMENT_ARRAY_POSITION = 5
#define SENSOR7_MEASUREMENT_ARRAY_POSITION = 6
#define SENSOR8_MEASUREMENT_ARRAY_POSITION = 7
#define SENSOR9_MEASUREMENT_ARRAY_POSITION = 8
#define SENSOR10_MEASUREMENT_ARRAY_POSITION = 9
#define SENSOR11_MEASUREMENT_ARRAY_POSITION = 10
#define SENSOR12_MEASUREMENT_ARRAY_POSITION = 11


//malloc's global variables in this header file
    //returns error code as int (NO_ERROR_CODE, GENERAL_ERROR_CODE, MEMORY_ALLOCATION_ERROR_CODE)
extern int estimatorSetup(); 

//global variable
    //will be accessible to controller.h file
    //variables in the array correspond to ESTIMATOR_VARIABLE_ARRAY_POSITION values
extern float* estimatedStateX;

//calculates estimatedStateX
    //updates estimatedStateX
    //accesses measurement data (y) in buffer class
    //accesses controller inputs (u) in Controller.h
    //returns error code as int (NO_ERROR_CODE, GENERAL_ERROR_CODE)
int calculateEstimatedStateX(); 

float kalmanFilter();

#endif
