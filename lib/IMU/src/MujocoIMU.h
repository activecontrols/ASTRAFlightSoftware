/*
FakeIMU.h - IMU module that fills measurement vector with random noise
Author: Vincent Wang
*/

#ifdef IS_MUJOCO
#ifndef MJCIMU_H
#define MJCIMU_H

#include "FlightModule.h"
#include <mujoco/mujoco.h>
#include "VEigen.h"


namespace flightData {
  extern Eigen::VectorXd measurementVectorY;
}

class MujocoIMUModule : public FlightModule {
public:
    // Write directly to measurementVectorY
    // NO CHECKS! May clobber other values!
    MujocoIMUModule();

    // Write to passed pointer to Eigen::Vector, useful for
    // combining with other modules
    MujocoIMUModule(Eigen::VectorXd* measurementVector);
    int init() override;
    void update(unsigned long time) override;

    mjData *d;
private:
    Eigen::VectorXd* measurementVector;
};

#endif // MJCIMU_H
#endif // IS_MUJOCO