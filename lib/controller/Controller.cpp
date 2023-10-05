#include "Controller.h"
//#include "../estimator/Estimator.h"
#include <stdlib.h>
#include "../error/Error.h"

float* controllerInput_u = (float *)malloc(U_ARRAY_LENGTH * sizeof(float));
float* mode = (float *)malloc(MODE_ARRAY_LENGTH * sizeof(float));
float* k = (float *)malloc(K_ARRAY_LENGTH * sizeof(float));

int controllerSetup() {
    
    //Check if memory allocation was successful
    if (controllerInput_u == NULL || mode == NULL || k == NULL) {
        
        return MEMORY_ALLOCATION_ERROR_CODE; //Memory Allocation Error Code
    }

    return NO_ERROR_CODE;
}
