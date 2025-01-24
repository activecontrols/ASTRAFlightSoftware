#if IS_MUJOCO
#include "MujocoIMU.h"
#include "Error.h"
#include <cstdlib>
#include <iostream>

#include <mujoco/mujoco.h>
#include "VEigen.h"

#define NOISE_COEFF 0.1

MujocoIMUModule::MujocoIMUModule() {
    this->measurementVector = &flightData::measurementVectorY;
}

MujocoIMUModule::MujocoIMUModule(Eigen::VectorXd *measurementVector) {
    this->measurementVector = measurementVector;
}

int MujocoIMUModule::init() {
    this->measurementVector->setZero();
    return NO_ERROR_CODE;
}

void MujocoIMUModule::update(unsigned long time) {
    float ax = 0;
    float ay = 0;
    float az = 0;
    float gxDPS = 0;
    float gyDPS = 0;
    float gzDPS = 0;
    float mx = 0;
    float my = 0;
    float mz = 0;
    *(this->measurementVector) << ax, ay, az, gxDPS, gyDPS, gzDPS, mx, my, mz;
}

#endif // IS_MUJOCO