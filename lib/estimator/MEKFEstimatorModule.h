/**
 * Module that blinks the integrated LED in multiple modes.
 * Simple proof-of-concept FlightModule; if you're not sure how to write a new module,
 * start by copying this one.
 */
#ifndef MEKF_ESTIMATOR_MODULE_H
#define MEKF_ESTIMATOR_MODULE_H

#include "FlightModule.h"
#include "VEigen.h"

#define FILTER_UPDATE_RATE_HZ (1200)

/* 
 * To declare dependency on some data existing, use the flightData namespace
 * and declare data as extern. Backing storage for the data can be defined in main.cpp (recommended)
 * or elsewhere.
 */
namespace flightData {
    extern Eigen::VectorXd measurementVectorY; // TODO: document format
    extern Eigen::VectorXd estimatedStateX; // TODO: document format
};

class MEKFEstimatorModule: public FlightModule {
public:
    MEKFEstimatorModule();
    int init() override;
    void update(unsigned long time) override;
private:
    unsigned long lastUpdateTime;
};

#endif // MEKF_ESTIMATOR_MODULE_H