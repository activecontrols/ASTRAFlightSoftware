#include "MEKFEstimatorModule.h"
#include "MEKF.h"
#include "MathFunctions.h"
#include "Error.h"

#include "VEigen.h"

#define SENSORS_RADS_TO_DPS 57.2957795

#define ESTIMATE_COVAR 0.0
#define GYRO_COVAR 0.0000194955
#define GYRO_BIAS_COVAR 0.000000000000000025032
#define ACCEL_COVAR 0.0000000024616355
#define ACCEL_BIAS_COVAR 0.0
#define ACCEL_OBS_COVAR 0.0003
#define MAG_COVAR 0.0000000024616355
#define MAG_BIAS_COVAR 0.0
#define MAG_OBS_COVAR 0.0003

MEKFEstimatorModule::MEKFEstimatorModule() {
}

int MEKFEstimatorModule::init() {
    this->lastUpdateTime = 0;
    initKalman(Eigen::Quaterniond::Identity(), ESTIMATE_COVAR, GYRO_COVAR, GYRO_BIAS_COVAR, ACCEL_COVAR, ACCEL_BIAS_COVAR, MAG_COVAR, MAG_BIAS_COVAR, ACCEL_OBS_COVAR, MAG_OBS_COVAR);
    flightData::estimatedStateX.setZero();
    return NO_ERROR_CODE;
}

void MEKFEstimatorModule::update(unsigned long time) {
    // Extract data
    float ax = flightData::measurementVectorY(0);
    float ay = flightData::measurementVectorY(1);
    float az = flightData::measurementVectorY(2);
    Eigen::Vector3d accel(-ax/9.8, -ay/9.8, -az/9.8);

    float gx = flightData::measurementVectorY(3);
    float gy = flightData::measurementVectorY(4);
    float gz = flightData::measurementVectorY(5);
    Eigen::Vector3d gyro(gx, gy, gz);

    // float mx = flightData::measurementVectorY(6);
    // float my = flightData::measurementVectorY(7);
    // float mz = flightData::measurementVectorY(8);
    // Eigen::Vector3d mag(mx, my, mz);
    Eigen::Vector3d mag(0.0, 0.0, 0.0);

    // Add measurements to filter
    updateKalman(gyro, accel, mag, (double)(time - this->lastUpdateTime)/1000000.0);
    this->lastUpdateTime = time;

    // Get data out of filter
    float qw = estimate.w();
    float qx = estimate.x();
    float qy = estimate.y();
    float qz = estimate.z();

    Eigen::Vector3d v = math::quatToEuler(estimate);
    float roll = v.x() * SENSORS_RADS_TO_DPS;
    float pitch = v.y() * SENSORS_RADS_TO_DPS;
    float yaw = v.z() * SENSORS_RADS_TO_DPS;
    
    flightData::estimatedStateX(0) = qx;
    flightData::estimatedStateX(1) = qy;
    flightData::estimatedStateX(2) = qz;
    flightData::estimatedStateX(3) = flightData::measurementVectorY(3);
    flightData::estimatedStateX(4) = flightData::measurementVectorY(4);
    flightData::estimatedStateX(5) = flightData::measurementVectorY(5);

    // flightData::estimatedStateX(3) = roll;
    // flightData::estimatedStateX(4) = pitch;
    // flightData::estimatedStateX(5) = yaw;
}