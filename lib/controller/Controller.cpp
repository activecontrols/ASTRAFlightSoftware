#include "Controller.h"
//#include "../estimator/Estimator.h"
//#include <stdlib.h>
#include "../error/Error.h"
#include <ArduinoEigenDense.h>


//float* controllerInputU = (float *)malloc(U_ARRAY_LENGTH * sizeof(float));
Eigen::VectorXd controllerInputU(U_ARRAY_LENGTH);
float* mode = (float *)malloc(MODE_ARRAY_LENGTH * sizeof(float));
float* k = (float *)malloc(K_ARRAY_LENGTH * sizeof(float));



int initializeController() {
    
    //Check if memory allocation was successful
    if (mode == NULL || k == NULL) {
        
        return MEMORY_ALLOCATION_ERROR_CODE; //Memory Allocation Error Code
    }

    for (unsigned int i = 0; i < U_ARRAY_LENGTH; i++) {
        controllerInputU[i] = 1;
    }

    return NO_ERROR_CODE;
}
