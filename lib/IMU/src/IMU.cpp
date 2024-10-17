#include <debug.h>
#include <Error.h>

#include "IMU.h"
#include "LSM6DS_LIS3MDL.h"

#include <ArduinoEigenDense.h>

/*
IMU.cpp 
Description: Function definititions for declarations in IMU.h
Author: Vincent Palmerio

*/
IMUModule::IMUModule() {
    this->measurementVector = &flightData::measurementVectorY;
}

IMUModule::IMUModule(Eigen::VectorXd *measurementVector) {
    this->measurementVector = measurementVector;
}

int IMUModule::init() {
    /* Start calibration helper */
    if (!cal.begin()) {
    //Failed to initialize calibration helper
    #if defined(ASTRA_FULL_DEBUG) or defined(ASTRA_IMU_DEBUG)
        Serial.print("Failed to initialize calibration helper");
    #endif
        return FAILED_CALIBRATION_HELPER_INIT;
    }

    /* Load calibration preset */
    this->loadPresetCalibration();
    if (!cal.loadCalibration()) {
        //No calibration loaded/found
        static bool triedLoadPresetCalibration = false;
        if (!triedLoadPresetCalibration) {
            if (loadPresetCalibration() == FAILED_LOAD_CALIBRATION) {
                return FAILED_LOAD_CALIBRATION;
            }
        }
    }

    /* Initialize sensors on the wire */
    if (!init_sensors(&IMU_WIRE)) {
      //Failed to find sensors
  #if defined(ASTRA_FULL_DEBUG) or defined(ASTRA_IMU_DEBUG)
      Serial.println("Failed to find sensors");
  #endif
      return FAILED_SENSOR_INIT;
    }

    /* Set up sensor data/clock rates and ranges */
    setup_sensors();
    Wire.setClock(WIRE_UPDATE_CLOCK); 
    
    return NO_ERROR_CODE;
}

void IMUModule::update() {
    /* Read the motion sensors */
    sensors_event_t accel, gyro, mag;
    accelerometer->getEvent(&accel);
    gyroscope->getEvent(&gyro);
    magnetometer->getEvent(&mag);
    
    /* Apply calibration corrections to sensors */
    cal.calibrate(mag);
    cal.calibrate(accel);
    cal.calibrate(gyro);

    float ax = accel.acceleration.x;
    float ay = accel.acceleration.y;
    float az = accel.acceleration.z;
    
    // Gyroscope needs to be converted from Rad/s to Degree/s
    // the rest are not unit-important
    float gxDPS = gyro.gyro.x * SENSORS_RADS_TO_DPS; //omega x
    float gyDPS = gyro.gyro.y * SENSORS_RADS_TO_DPS; //omega y
    float gzDPS = gyro.gyro.z * SENSORS_RADS_TO_DPS; //omega z

    float mx = mag.magnetic.x;
    float my = mag.magnetic.y;
    float mz = mag.magnetic.z;
    
    *(this->measurementVector) << ax, ay, az, gxDPS, gyDPS, gzDPS, mx, my, mz;
}

int IMUModule::loadPresetCalibration() {
    #if IMU_NUMBER == 1
        //Magnetic Hard Offset
        cal.mag_hardiron[0] = 18.00;
        cal.mag_hardiron[1] = 37.79;
        cal.mag_hardiron[2] = -84.00;
  
        //Magnetic Soft Offset
        // in uTesla
        cal.mag_softiron[0] = 0.98;
        cal.mag_softiron[1] = 0.05;
        cal.mag_softiron[2] = 0.01;  
        cal.mag_softiron[3] = 0.05;
        cal.mag_softiron[4] = 1.06;
        cal.mag_softiron[5] = 0.0;  
        cal.mag_softiron[6] = 0.01;
        cal.mag_softiron[7] = 0.0;
        cal.mag_softiron[8] = 0.96;  
  
        //Gyro zero rate offset
        // in Radians/s
        cal.gyro_zerorate[0] = 0.05;
        cal.gyro_zerorate[1] = -0.01;
        cal.gyro_zerorate[2] = -0.01;
    #endif
  
    #if IMU_NUMBER == 2 /* NOT CALIBRATED YET */
        //Magnetic Hard Offset
        cal.mag_hardiron[0] = 0.0;
        cal.mag_hardiron[1] = 0.0;
        cal.mag_hardiron[2] = 0.0;
  
        //Magnetic Soft Offset
        // in uTesla
        cal.mag_softiron[0] = 0.0;
        cal.mag_softiron[1] = 0.0;
        cal.mag_softiron[2] = 0.0;  
        cal.mag_softiron[3] = 0.0;
        cal.mag_softiron[4] = 0.0;
        cal.mag_softiron[5] = 0.0;  
        cal.mag_softiron[6] = 0.0;
        cal.mag_softiron[7] = 0.0;
        cal.mag_softiron[8] = 0.0;  
  
        //Gyro zero rate offset
        // in Radians/s
        cal.gyro_zerorate[0] = 0.0;
        cal.gyro_zerorate[1] = 0.0;
        cal.gyro_zerorate[2] = 0.0;
    #endif

    if (cal.saveCalibration()) {
      return FAILED_LOAD_CALIBRATION;
    }
    return NO_ERROR_CODE;
}