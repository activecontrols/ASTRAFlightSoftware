/*
FakeGPS.h - GPS module that fills measurement vector with random noise
Author: Vincent Wang
*/

#ifdef IS_MUJOCO
#ifndef MJCGPS_H
#define MJCGPS_H

#include "FlightModule.h"
#include <mujoco/mujoco.h>
#include "VEigen.h"


namespace flightData {
  extern Eigen::VectorXd measurementVectorY;
}

class MujocoGPSModule : public FlightModule {
public:
    // Write directly to measurementVectorY
    // NO CHECKS! May clobber other values!
    MujocoGPSModule();

    // Write to passed pointer to Eigen::Vector, useful for
    // combining with other modules
    MujocoGPSModule(Eigen::VectorXd* measurementVector);
    int init() override;
    void update(unsigned long time) override;

    mjData *d;
private:
    Eigen::VectorXd* measurementVector;
};

#endif // MJCGPS_H
#endif // IS_MUJOCO