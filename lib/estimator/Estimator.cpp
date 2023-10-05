#include "Estimator.h"
#include <stdlib.h>
#include "../error/Error.h"

//variables may need to be included outside the function in order to be defined globally instead of locally
//research using extern
float* estimatedStateX = (float *)malloc(ESTIMATOR_ARRAY_LENGTH * sizeof(float));

int estimatorSetup() {

    if (estimatedStateX == NULL) {
        return MEMORY_ALLOCATION_ERROR_CODE;
    }
    
    return NO_ERROR_CODE;
}