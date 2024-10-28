#include "FakeIMU.h"
#include "Error.h"
#include <cstdlib>
#include <iostream>

#include "VEigen.h"

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
    float ax = 10 * (rand() / (float)RAND_MAX - 0.5);
    float ay = 10 * (rand() / (float)RAND_MAX - 0.5);
    float az = 10 * (rand() / (float)RAND_MAX - 0.5);

    float gxDPS = 10 * (rand() / (float)RAND_MAX - 0.5);
    float gyDPS = 10 * (rand() / (float)RAND_MAX - 0.5);
    float gzDPS = 10 * (rand() / (float)RAND_MAX - 0.5);

    float mx = 10 * (rand() / (float)RAND_MAX - 0.5);
    float my = 10 * (rand() / (float)RAND_MAX - 0.5);
    float mz = 10 * (rand() / (float)RAND_MAX - 0.5);

    *(this->measurementVector) << ax, ay, az, gxDPS, gyDPS, gzDPS, mx, my, mz;
}