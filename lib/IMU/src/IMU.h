/*
IMU.h - Header file for the IMU Driver
Description: Contains all functions and variables to interact with the IMU
Author: Vincent Palmerio
*/

#ifndef IMU_H
#define IMU_H

#include <Adafruit_LSM6DSOX.h>
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_AHRS.h>
#include <Adafruit_Sensor_Calibration.h>
#include <Adafruit_AHRS.h>
#include <ArduinoEigenDense.h>
#include <Wire.h>

//uncomment to print debugging data to console
#define ASTRA_IMU_DEBUG 
//#define AHRS_DEBUG_OUTPUT


// Full orientation sensing using NXP/Madgwick/Mahony and a range of 9-DoF
// sensor sets.
// You *must* perform a magnetic calibration before this code will work.
//
// To view this data, use the Arduino Serial Monitor to watch the
// scrolling angles, or run the OrientationVisualiser example in Processing.
// Based on  https://github.com/PaulStoffregen/NXPMotionSense with adjustments
// to Adafruit Unified Sensor interface


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

namespace imu {
  extern float orientationArray[3];
  extern float roll, pitch, yaw;
  extern Eigen::VectorXd linearAccelVector;
  extern float linearAccelX, linearAccelY, linearAccelZ;

  extern Eigen::VectorXd gyroscopeVector;
  extern Eigen::VectorXd magnetometerVector;

  extern float gx, gy, gz; //degrees per second on gyro
  extern float qw, qx, qy, qz; //quaternarion

  int initializeIMU();
  int updateIMU();
  int loadPresetCalibration();
  float* updateOrientationArray();
}

#endif




