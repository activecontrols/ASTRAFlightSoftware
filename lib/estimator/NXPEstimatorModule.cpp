#include "NXPEstimatorModule.h"
#include "Integrator.h"
#include "Error.h"
#include "IMU.h"
#include "MathFunctions.h"
#include "TOF.h"
#include "settings.h"

#include <ArduinoEigenDense.h>

NXPEstimatorModule::NXPEstimatorModule() {
}

int NXPEstimatorModule::init() {
    this->filter.begin();
    flightData::estimatedStateX.setZero();
    return NO_ERROR_CODE;
}

void NXPEstimatorModule::update() {
    // Extract data
    float ax = flightData::measurementVectorY(0);
    float ay = flightData::measurementVectorY(1);
    float az = flightData::measurementVectorY(2);

    float gx = flightData::measurementVectorY(3);
    float gy = flightData::measurementVectorY(4);
    float gz = flightData::measurementVectorY(5);

    float mx = flightData::measurementVectorY(6);
    float my = flightData::measurementVectorY(7);
    float mz = flightData::measurementVectorY(8);

    // Add measurements to filter
    filter.update(gx, gy, gz,
                  ax, ay, az,
                  mx, my, mz);
    
    // Get data out of filter
    float linearAccel[3];
    float q[4];
    // float roll = filter.getRoll();
    // float pitch = filter.getPitch();
    // float yaw = filter.getYaw();
    filter.getQuaternion(&q[0], &q[1], &q[2], &q[3]);
    filter.getLinearAcceleration(&linearAccel[0], &linearAccel[1], &linearAccel[2]);

    flightData::estimatedStateX(0) = q[1];
    flightData::estimatedStateX(1) = q[2];
    flightData::estimatedStateX(2) = q[3];
    flightData::estimatedStateX(3) = flightData::measurementVectorY(0);
    flightData::estimatedStateX(4) = flightData::measurementVectorY(1);
    flightData::estimatedStateX(5) = flightData::measurementVectorY(2);
}