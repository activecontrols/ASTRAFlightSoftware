//watch the videos
/*
Estimator.h - Estimator Header file
Description: Header file to Estimator.cpp
Author: Vincent Palmerio
Created: 9/18/2023
Last updated: 9/18/2023
*/

#define MEASUREMENTINPUTLENGTH 9 
#define ESTIMATORARRAYLENGTH 12

//12 sensors and their positions in the array returned from getMeasurements_y()
//(replace SENSOR{Number} with name of sensor)
#define SENSOR1MEASUREMENTARRAYPOSITION = 0
#define SENSOR2MEASUREMENTARRAYPOSITION = 1
#define SENSOR3MEASUREMENTARRAYPOSITION = 2
#define SENSOR4MEASUREMENTARRAYPOSITION = 3
#define SENSOR5MEASUREMENTARRAYPOSITION = 4
#define SENSOR6MEASUREMENTARRAYPOSITION = 5
#define SENSOR7MEASUREMENTARRAYPOSITION = 6
#define SENSOR8MEASUREMENTARRAYPOSITION = 7
#define SENSOR9MEASUREMENTARRAYPOSITION = 8
#define SENSOR10MEASUREMENTARRAYPOSITION = 9
#define SENSOR11MEASUREMENTARRAYPOSITION = 10
#define SENSOR12MEASUREMENTARRAYPOSITION = 11

//the amount of variables equates to ESTIMATORVARIABLELENGTH
//replace VARIABLE with the variable name
#define ESTIMATORVARIABLE1ARRAYPOSITION = 0
#define ESTIMATORVARIABLE2ARRAYPOSITION = 1
#define ESTIMATORVARIABLE3ARRAYPOSITION = 2
#define ESTIMATORVARIABLE4ARRAYPOSITION = 3
#define ESTIMATORVARIABLE5ARRAYPOSITION = 4
#define ESTIMATORVARIABLE6ARRAYPOSITION = 5
#define ESTIMATORVARIABLE7ARRAYPOSITION = 6
#define ESTIMATORVARIABLE8ARRAYPOSITION = 7
#define ESTIMATORVARIABLE9ARRAYPOSITION = 8
#define ESTIMATORVARIABLE10ARRAYPOSITION = 9
#define ESTIMATORVARIABLE11ARRAYPOSITION = 10
#define ESTIMATORVARIABLE12ARRAYPOSITION = 11


int* (estimatedState_x)[ESTIMATORARRAYLENGTH]; //variables in the array correspond to ESTIMATORVARIABLEARRAYPOSITION values
//will be accessible to controller.h file

int* getMeasurements_y(); // will return a pointer to int[] of size MEASUREMENTINPUTLENGTH where each place in the array corrsponds to MEASUREMENTARRAYPOSITION variables            
//should access buffer memory locations of all sensors, most likely will be done by accessing buffer pointers in another header file

int* getControllerInputs_u(); //will access pointer to int[] in Controller.h file 

void calculateEstimatedState_x(); //returns nothing, updates estimatedState_x 

int kalmanFilter();







