#include "Integrator.h"
#include "../error/Error.h"

Eigen::VectorXd integratedData(0);


int integratorSetup(Eigen::VectorXd* pointerToData)
{

    dataToIntegrate = pointerToData;

    if (dataToIntegrate == nullptr) {
        return MEMORY_ALLOCATION_ERROR_CODE;
    }

    int vectorSize = pointerToData->size();

    Eigen::VectorXd v(vectorSize);

    integratedData = v;

    if (integratedData.size() == 0) {
        return VECTOR_INIT_ZERO_SIZE_ERROR;
    }


    return NO_ERROR_CODE;
}

int integratorUpdate()
{
    int changeInTime = timeBetweenIntegration;
    timeBetweenIntegration = 0;

    if (changeInTime > MAX_ALLOWED_TIME_BETWEEN_INTEGRATION) {
        return MAX_ALLOWED_TIME_BETWEEN_INTEGRATION_EXCEEDED;
    }

    integratedData += (*dataToIntegrate) * changeInTime;

    return NO_ERROR_CODE;
}
