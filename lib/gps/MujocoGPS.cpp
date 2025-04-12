#if IS_MUJOCO
#include "MujocoGPS.h"
#include "ASTRAError.h"
#include <cstdlib>
#include <iostream>

#include <mujoco/mujoco.h>
#include "VEigen.h"

#define NOISE_COEFF 0.1

MujocoGPSModule::MujocoGPSModule() {
    this->measurementVector = &flightData::measurementVectorY;
}

MujocoGPSModule::MujocoGPSModule(Eigen::VectorXd *measurementVector) {
    this->measurementVector = measurementVector;
}

int MujocoGPSModule::init() {
    this->measurementVector->setZero();
    return NO_ERROR_CODE;
}

void MujocoGPSModule::update(unsigned long time) {
    (*this->measurementVector)( 9) = this->d->qpos[0];
    (*this->measurementVector)(10) = this->d->qpos[1];
    (*this->measurementVector)(11) = this->d->qpos[2];
    (*this->measurementVector)(12) = this->d->qvel[0];
    (*this->measurementVector)(13) = this->d->qvel[1];
    (*this->measurementVector)(14) = this->d->qvel[2];
}

#endif // IS_MUJOCO