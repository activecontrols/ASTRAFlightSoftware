#include "Estimator.h"
#include <stdlib.h>
#include "../error/Error.h"

//variables may need to be included outside the function in order to be defined globally instead of locally
//research using extern
float* estimatedStateX = (float *)malloc(STATE_DIMENSION * sizeof(float));

int initializeEstimator() {

    if (estimatedStateX == NULL) {
        return MEMORY_ALLOCATION_ERROR_CODE;
    }

    for (unsigned int i = 0; i < STATE_DIMENSION; i++) {
        estimatedStateX[i] = 1;
    }
    
    return NO_ERROR_CODE;
}