#include "estimator.h"
#include "VEigen.h"
#include "MEKF.h"
#include "MEKFEstimatorModule.h"

namespace flightData {
    Eigen::VectorXd measurementVectorY;
    Eigen::VectorXd estimatedStateX;
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
MEKFEstimatorModule estimator;

void init() {
    estimator.init();
    flightData::measurementVectorY = Eigen::VectorXd(9);
    flightData::estimatedStateX = Eigen::VectorXd(6);
}

void run(double* i, double* o) {
    flightData::measurementVectorY = toEigen(i, 9);
    estimator.update(t);
    t += 1e9;
    memcpy(o, toArray(flightData::estimatedStateX), sizeof(double) * 6);
    // memcpy(o, i, sizeof(double) * 6);
}