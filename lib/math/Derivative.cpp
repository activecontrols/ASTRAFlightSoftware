#include "Derivative.h"
#include "../error/Error.h"

int derivativeSetup(Eigen::VectorXd* pointerToData)
{
    newestData = pointerToData;

    if (newestData == NULL) {
        return MEMORY_ALLOCATION_ERROR_CODE;
    }

    int vectorSize = newestData->size();

    if (vectorSize == 0) {
        return VECTOR_INIT_ZERO_SIZE_ERROR;
    }

    Eigen::VectorXd v(vectorSize);

    previousData = v;

    derivative = v;

    return NO_ERROR_CODE;
}

int derivativeUpdate()
{

    derivative = (*newestData) - previousData;

    previousData = (*newestData);

    return NO_ERROR_CODE;
}