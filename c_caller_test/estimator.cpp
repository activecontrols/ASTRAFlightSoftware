#include "estimator.h"
#include "VEigen.h"
#include "MEKF.h"
#include "MEKFEstimatorModule.h"

namespace flightData {
    Eigen::VectorXd measurementVectorY = Eigen::VectorXd(6);
    Eigen::VectorXd estimatedStateX = Eigen::VectorXd(6);
};

Eigen::VectorXd toEigen(double*);
double* toArray(Eigen::VectorXd);

Eigen::VectorXd toEigen(double* arr, int size) {
    return Eigen::VectorXd::Map(arr, size);
}

double* toArray(Eigen::VectorXd eigen) {
    return eigen.data();
}

long t = 1e9;
// auto estimator = MEKFEstimatorModule::MEKFEstimatorModule();

void init() {
    // estimator.init();
}

void run(double* input, double* output) {
    flightData::measurementVectorY = toEigen(input, 6);
    // estimator.update(t);
    // t += 1e9;
    memcpy(output, toArray(flightData::measurementVectorY), sizeof(double) * 6);
    // memcpy(output, input, sizeof(double) * 6);
}