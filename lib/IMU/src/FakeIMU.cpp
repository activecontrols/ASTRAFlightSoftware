#include "FakeIMU.h"
#include "Error.h"
#include <cstdlib>
// #include <iostream>

#include "VEigen.h"

#define NOISE_COEFF 0.1

FakeIMUModule::FakeIMUModule() {
    this->measurementVector = &flightData::measurementVectorY;
}

FakeIMUModule::FakeIMUModule(Eigen::VectorXd *measurementVector) {
    this->measurementVector = measurementVector;
}

int FakeIMUModule::init() {
    this->measurementVector->setZero();
    return NO_ERROR_CODE;
}

void FakeIMUModule::update(unsigned long time) {
    float ax = NOISE_COEFF * (rand() / (float)RAND_MAX - 0.5);
    float ay = NOISE_COEFF * (rand() / (float)RAND_MAX - 0.5);
    float az = NOISE_COEFF * (rand() / (float)RAND_MAX - 0.5) + 9.8;

    float gxDPS = NOISE_COEFF * (rand() / (float)RAND_MAX - 0.5);
    float gyDPS = NOISE_COEFF * (rand() / (float)RAND_MAX - 0.5);
    float gzDPS = NOISE_COEFF * (rand() / (float)RAND_MAX - 0.5);

    float mx = NOISE_COEFF * (rand() / (float)RAND_MAX - 0.5) + 1;
    float my = NOISE_COEFF * (rand() / (float)RAND_MAX - 0.5);
    float mz = NOISE_COEFF * (rand() / (float)RAND_MAX - 0.5);

    *(this->measurementVector) << ax, ay, az, gxDPS, gyDPS, gzDPS, mx, my, mz;
}