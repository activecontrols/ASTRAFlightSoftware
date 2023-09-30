//watch the videos
/*
Estimator.h - Estimator Header file
Description: Header file to Estimator.cpp
Author: Vincent Palmerio
Created: 9/18/2023
Last updated: 9/18/2023
*/

#define MEASUREMENT_INPUT_LENGTH 9 
#define ESTIMATOR_ARRAY_LENGTH 12

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

//the amount of variables equates to ESTIMATOR_VARIABLE_LENGTH
//replace VARIABLE with the variable name
#define ESTIMATOR_VARIABLE1_ARRAY_POSITION = 0
#define ESTIMATOR_VARIABLE2_ARRAY_POSITION = 1
#define ESTIMATOR_VARIABLE3_ARRAY_POSITION = 2
#define ESTIMATOR_VARIABLE4_ARRAY_POSITION = 3
#define ESTIMATOR_VARIABLE5_ARRAY_POSITION = 4
#define ESTIMATOR_VARIABLE6_ARRAY_POSITION = 5
#define ESTIMATOR_VARIABLE7_ARRAY_POSITION = 6
#define ESTIMATOR_VARIABLE8_ARRAY_POSITION = 7
#define ESTIMATOR_VARIABLE9_ARRAY_POSITION = 8
#define ESTIMATOR_VARIABLE10_ARRAY_POSITION = 9
#define ESTIMATOR_VARIABLE11_ARRAY_POSITION = 10
#define ESTIMATOR_VARIABLE12_ARRAY_POSITION = 11

#define NO_ERROR_CODE = 0
#define GENERAL_ERROR_CODE = -1
#define MEMORY_ALLOCATION_ERROR_CODE = -2

//malloc's global variables in this header file
    //returns error code as int (NO_ERROR_CODE, GENERAL_ERROR_CODE, MEMORY_ALLOCATION_ERROR_CODE)
int setup(); 

//global variable
    //will be accessible to controller.h file
    //variables in the array correspond to ESTIMATOR_VARIABLE_ARRAY_POSITION values
float estimatedStateX[ESTIMATOR_ARRAY_LENGTH]; 

//calculates estimatedStateX
    //updates estimatedStateX
    //accesses measurement data (y) in buffer class
    //accesses controller inputs (u) in Controller.h
    //returns error code as int (NO_ERROR_CODE, GENERAL_ERROR_CODE)
int calculateEstimatedStateX(); 

float kalmanFilter();







