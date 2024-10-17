/*
IMU.h - Header file for the IMU Driver
Description: Contains all functions and variables to interact with the IMU
Author: Vincent Palmerio
*/

#ifndef IMU_H
#define IMU_H

#include "FlightModule.h"
#include <Adafruit_LSM6DSOX.h>
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_AHRS.h>
#include <Adafruit_Sensor_Calibration.h>
#include <ArduinoEigenDense.h>
#include <Wire.h>

//uncomment to print debugging data to console

// Full orientation sensing using NXP/Madgwick/Mahony and a range of 9-DoF
// sensor sets.
// You *must* perform a magnetic calibration before this code will work.
//
// To view this data, use the Arduino Serial Monitor to watch the
// scrolling angles, or run the OrientationVisualiser example in Processing.
// Based on  https://github.com/PaulStoffregen/NXPMotionSense with adjustments
// to Adafruit Unified Sensor interface


// TODO: All IMU definitions (incl. calibrations) should be done in an IMU-specific
// header file
#if defined(ADAFRUIT_SENSOR_CALIBRATION_USE_EEPROM)
  static Adafruit_Sensor_Calibration_EEPROM cal;
#else
  Adafruit_Sensor_Calibration_SDFat cal;
#endif

#define FILTER_UPDATE_RATE_HZ (1200)
#define WIRE_UPDATE_CLOCK (400000)
#define IMU_WIRE Wire
#define IMU_NUMBER (1) /* Determined which IMU we are using (ie, physical IMU labeled as 1 or 2)*/

// slower == better quality output
static Adafruit_NXPSensorFusion filter; // slowest
//static Adafruit_Mahony filter; 
//static Adafruit_Madgwick filter;

namespace flightData {
  extern Eigen::VectorXd measurementVectorY;
}

class IMUModule : public FlightModule {
public:
    // Write directly to measurementVectorY
    // NO CHECKS! May clobber other values!
    IMUModule();

    // Write to passed pointer to Eigen::Vector, useful for
    // combining with other modules
    IMUModule(Eigen::VectorXd* measurementVector);
    int init() override;
    void update() override;
private:
    Eigen::VectorXd* measurementVector;

    int loadPresetCalibration();
};

#endif




