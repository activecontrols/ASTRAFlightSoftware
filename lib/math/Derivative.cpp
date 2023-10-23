#include "Derivative.h"
#include "../error/Error.h"

static Eigen::VectorXd* newestData = nullptr;
static Eigen::VectorXd previousData(0);
Eigen::VectorXd derivative(0);

int derivativeSetup(Eigen::VectorXd* pointerToData)
{
    newestData = pointerToData;

    if (newestData == nullptr) {
        return MEMORY_ALLOCATION_ERROR_CODE;
    }

    int vectorSize = newestData->size();

    Eigen::VectorXd v(vectorSize);

    previousData = v;

    derivative = v;

    if (previousData.size() == 0 || derivative.size() == 0) {
        return VECTOR_INIT_ZERO_SIZE_ERROR;
    }

    return NO_ERROR_CODE;
}

int derivativeUpdate()
{

    derivative = (*newestData) - previousData;

    previousData = (*newestData);

    return NO_ERROR_CODE;
}