/*
FakeIMU.h - IMU module that fills measurement vector with random noise
Author: Vincent Wang
*/

#ifndef FAKEIMU_H
#define FAKEIMU_H

#include "FlightModule.h"
#include "VEigen.h"


namespace flightData {
  extern Eigen::VectorXd measurementVectorY;
}

class FakeIMUModule : public FlightModule {
public:
    // Write directly to measurementVectorY
    // NO CHECKS! May clobber other values!
    FakeIMUModule();

    // Write to passed pointer to Eigen::Vector, useful for
    // combining with other modules
    FakeIMUModule(Eigen::VectorXd* measurementVector);
    int init() override;
    void update(unsigned long time) override;
private:
    Eigen::VectorXd* measurementVector;
};

#endif // FAKEIMU_H