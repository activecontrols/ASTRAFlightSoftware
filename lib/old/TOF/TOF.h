/*
TOF.h - Header file for the Time-of-Flight Driver
Description: Contains all functions and variables to interact with the TOF Sensor
Author: Henry Hellman and Ani Gokhale

Last updated: 2/21/2024

*/

#ifndef TOF_H
#define TOF_H

#include <ArduinoEigenDense.h>
#include <Arduino.h>
#include <Wire.h>
#include <vl53l4cx_class.h>
// //#include <string.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <stdint.h>
// #include <assert.h>
// #include <stdlib.h>
#include <Buffer.h>

#define DEV_I2C Wire
#define TOF_INTERRUPT_PIN 23
#define TOF_XSHUT_PIN 22
#define NUM_TOF_MEASUREMENTS 10

namespace tof {
    extern VL53L4CX sensor_vl53l4cx_sat;

    extern volatile int interruptCount;

    extern float* latestData;

    extern float* updateTOFDistance();
    extern void measure();
    extern int setupTOFSensor(unsigned int interruptPin, unsigned int XSHUTPin);
    extern Buffer TOF_buffer;
}

#endif