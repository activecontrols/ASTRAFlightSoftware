/**
 * Module that blinks the integrated LED in multiple modes.
 * Simple proof-of-concept FlightModule; if you're not sure how to write a new module,
 * start by copying this one.
 */
#ifndef NXP_ESTIMATOR_MODULE_H
#define NXP_ESTIMATOR_MODULE_H

#include "FlightModule.h"
#include "VEigen.h"

#include <Adafruit_AHRS.h>

#define FILTER_UPDATE_RATE_HZ (1200)

/* 
 * To declare dependency on some data existing, use the flightData namespace
 * and declare data as extern. Backing storage for the data can be defined in main.cpp (recommended)
 * or elsewhere.
 */
namespace flightData {
    extern Eigen::VectorXd measurementVectorY;
    extern Eigen::VectorXd estimatedStateX;
};

class NXPEstimatorModule: public FlightModule {
public:
    NXPEstimatorModule();
    int init() override;
    void update(unsigned long time) override;
private:
    // slower == better quality output
    Adafruit_NXPSensorFusion filter; // slowest
    //Adafruit_Mahony filter; 
    //Adafruit_Madgwick filter;
};

#endif // NXP_ESTIMATOR_MODULE_H