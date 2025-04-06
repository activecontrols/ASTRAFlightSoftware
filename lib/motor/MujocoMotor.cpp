#if IS_MUJOCO
#include "MujocoMotor.h"
#include "ASTRAError.h"
#include <cstdlib>
#include <iostream>

#include <mujoco/mujoco.h>
#include "VEigen.h"

#define NOISE_COEFF 0.1

MujocoMotorModule::MujocoMotorModule() {
}

int MujocoMotorModule::init() {
    return NO_ERROR_CODE;
}

void MujocoMotorModule::update(unsigned long time) {
    this->d->ctrl[0] = flightData::controllerInputU(0);
    this->d->ctrl[1] = flightData::controllerInputU(1);
    this->d->ctrl[2] = flightData::controllerInputU(2);
    this->d->ctrl[3] = flightData::controllerInputU(3);
}

#endif // IS_MUJOCO