#include "Controller.h"
#include "../estimator/Estimator.h"
#include <stdlib.h>
#include "../error/Error.h"

int controllerSetup() {
    controllerInput_u = (float *)malloc(U_ARRAY_LENGTH * sizeof(float));
    mode = (float *)malloc(MODE_ARRAY_LENGTH * sizeof(float));
    k = (float *)malloc(K_ARRAY_LENGTH * sizeof(float));

    //Check if memory allocation was successful
    if (controllerInput_u == NULL || mode == NULL || k == NULL) {
        
        return MEMORY_ALLOCATION_ERROR_CODE; //Memory Allocation Error Code
    }

    // Now, you can access and use these arrays just like any other arrays.

    // Don't forget to free the memory when you're done with these arrays to avoid memory leaks.
}
