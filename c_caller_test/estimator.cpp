#include "estimator.h"
#include "VEigen.h"
#include "MEKF.h"
#include "MEKFEstimatorModule.h"

void run(double* input, double* time, double* output) {
    // Eigen::VectorXd measurementVectorY = toEigen(input, 6);
    // Eigen::VectorXd estimatedStateX;
    // update((long) time * 1e9);
    // memcpy(output, toArray(estimatedStateX), sizeof(double) * 6);
    memcpy(output, input, sizeof(double) * 6);
}

Eigen::VectorXd toEigen(double* arr, int size) {
    return Eigen::VectorXd::Map(arr, size);
}

double* toArray(Eigen::VectorXd eigen) {
    return eigen.data();
}