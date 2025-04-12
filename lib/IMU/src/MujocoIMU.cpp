#if IS_MUJOCO
#include "MujocoIMU.h"
#include "ASTRAError.h"
#include <cstdlib>
#include <iostream>

#include <mujoco/mujoco.h>
#include "VEigen.h"

#define NOISE_COEFF 0.1
#define SENSORS_RADS_TO_DPS 57.2957795

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
    (*this->measurementVector)(0) = this->d->sensordata[0];
    (*this->measurementVector)(1) = this->d->sensordata[1];
    (*this->measurementVector)(2) = this->d->sensordata[2];
    (*this->measurementVector)(3) = this->d->sensordata[3];
    (*this->measurementVector)(4) = this->d->sensordata[4];
    (*this->measurementVector)(5) = this->d->sensordata[5];
    (*this->measurementVector)(6) = this->d->sensordata[6];
    (*this->measurementVector)(7) = this->d->sensordata[7];
    (*this->measurementVector)(8) = this->d->sensordata[8];
    // printf("Y: [%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf]",
    //   flightData::measurementVectorY(0),
    //   flightData::measurementVectorY(1),
    //   flightData::measurementVectorY(2),
    //   flightData::measurementVectorY(3),
    //   flightData::measurementVectorY(4),
    //   flightData::measurementVectorY(5),
    //   flightData::measurementVectorY(6),
    //   flightData::measurementVectorY(7),
    //   flightData::measurementVectorY(8)
    // );
}

#endif // IS_MUJOCO