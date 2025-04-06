/*
FakeMotor.h - Motor module that fills measurement vector with random noise
Author: Vincent Wang
*/

#ifdef IS_MUJOCO
#ifndef MJCMotor_H
#define MJCMotor_H

#include "FlightModule.h"
#include <mujoco/mujoco.h>
#include "VEigen.h"


namespace flightData {
  extern Eigen::VectorXd controllerInputU;
}

class MujocoMotorModule : public FlightModule {
public:
    MujocoMotorModule();
    int init() override;
    void update(unsigned long time) override;

    mjData *d;
private:
};

#endif // MJCMotor_H
#endif // IS_MUJOCO