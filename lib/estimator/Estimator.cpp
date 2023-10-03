#include "Estimator.h"
#include <stdlib.h>
#include "../error/Error.h"

int estimatorSetup() {
    estimatedStateX = (float *)malloc(ESTIMATOR_ARRAY_LENGTH * sizeof(float));

    if (estimatedStateX == NULL) {
        return MEMORY_ALLOCATION_ERROR_CODE;
    }
    
    return 0;
}