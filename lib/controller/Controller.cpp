#include "Controller.h"
//#include "../estimator/Estimator.h"
//#include <stdlib.h>
#include "../error/Error.h"
#include "../estimator/Estimator.h"

//float* controllerInputU = (float *)malloc(U_ARRAY_LENGTH * sizeof(float));
Eigen::VectorXd controllerInputU(U_ARRAY_LENGTH);
double* mode = (double *)malloc(MODE_ARRAY_LENGTH * sizeof(double));
double* k = (double *)malloc(K_ARRAY_LENGTH * sizeof(double));
Eigen::Matrix4Xd deltaX(X_ROW_LENGTH,X_COLUMN_LENGTH);

Eigen::VectorXd xRef{{0,0,0,0,0,0,0}};
Eigen::VectorXd deltaXVector(7);

int initializeController() {
    
    //Check if memory allocation was successful
    if (mode == NULL || k == NULL) {
        
        return MEMORY_ALLOCATION_ERROR_CODE; //Memory Allocation Error Code
    }

    for (unsigned int i = 0; i < X_ROW_LENGTH*X_COLUMN_LENGTH; i++) {
        deltaX(i) = 0;
    }

    for (unsigned int i = 0; i < U_ARRAY_LENGTH; i++) {
        controllerInputU(i) = 1;
    }

    for (unsigned int i = 0; i < U_ARRAY_LENGTH; i++) {
        deltaX(i) = 1;
    }

    return NO_ERROR_CODE;
}

int getDeltaX(Eigen::VectorXd* x, Eigen::VectorXd* xRef) {
    deltaXVector = (*x)-(*xRef);

    return NO_ERROR_CODE;
}

Eigen::Quaterniond getConstrainedQuaternion(Eigen::Matrix4Xd* v) {
    //magnitude constraint of quaternion
    //prone to precision float errors
    double constraint = std::sqrt(
        1-
        (*v)(2,0)*(*v)(2,0)-
        (*v)(2,1)*(*v)(2,1)-
        (*v)(2,2)*(*v)(2,2)
    );

    Eigen::Quaterniond e(
        constraint,
        v[2](0),
        v[2](1),
        v[2](0)
    );
    return e;
}

int getDeltaX(Eigen::Matrix4Xd* x, Eigen::Matrix4Xd* xRef) {

    Eigen::VectorXd deltaR = (*x).row(0) - (*xRef).row(0);
    //magnitude of vector
    deltaX(0) = deltaR.norm();

    Eigen::VectorXd deltaV = (*x).row(1) - (*xRef).row(1);
    //magnitude of vector
    deltaX(1) = deltaV.norm();
    
    Eigen::Quaterniond throttle = getConstrainedQuaternion(x);

    Eigen::Quaterniond throttle2 = getConstrainedQuaternion(xRef);

    Eigen::Quaterniond e = throttle * throttle2;

    //theta
    deltaX(2) = 2*std::acos(e.w());

    Eigen::VectorXd deltaW = (*x).row(3) - (*xRef).row(3);
    //magnitude of vector
    deltaX(3) = deltaW.norm();

    return NO_ERROR_CODE;
}


