#include "Integrator.h"
#include "../error/Error.h"

int integratorSetup(Eigen::VectorXd* pointerToData)
{

    dataToIntegrate = pointerToData;

    if (dataToIntegrate == NULL) {
        return MEMORY_ALLOCATION_ERROR_CODE;
    }

    int vectorSize = pointerToData->size();

    if (vectorSize == 0) {
        return VECTOR_INIT_ZERO_SIZE_ERROR;
    }

    Eigen::VectorXd v(vectorSize);

    integratedData = v;


    return NO_ERROR_CODE;
}

int integratorUpdate()
{
    int changeInTime = timeBetweenIntegration;
    timeBetweenIntegration = 0;

    if (changeInTime > maxAllowedTimeBetweenIntegration) {
        return MAX_ALLOWED_TIME_BETWEEN_INTEGRATION_EXCEEDED;
    }

    integratedData += (*dataToIntegrate) * changeInTime;

    return NO_ERROR_CODE;
}
