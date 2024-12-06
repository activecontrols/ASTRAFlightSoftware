#ifndef ENCODER_MODULE_H
#define ENCODER_MODULE_H

#include "FlightModule.h"
#include "Wire.h"
#include "AS5600.h"

#define OUTER_GIMBAL_INDEX 0
#define INNER_GIMBAL_INDEX 1
#define LEFT_VANE_INDEX 2
#define RIGHT_VANE_INDEX 3

#define OUTER_GIMBAL_WIRE &Wire
#define INNER_GIMBAL_WIRE &Wire1
#define LEFT_VANE_WIRE &Wire2
/** 
 * Can multiple encoders be on the same I2C bus? 
 * If they use the same address and the address can't be changed then we have a problem 
 * No 3rd I2C bus on Teensy 4.1 :(
 */
#define RIGHT_VANE_WIRE &Wire2 

namespace flightData {
    extern float encoderPos[4];
    extern float encoderSpeeds[4];
}

class Encoder : public FlightModule {

public:
    int init() override;
    void update(unsigned long time) override;
private:
    AS5600 encoders[4] ={
        AS5600(OUTER_GIMBAL_WIRE),
        AS5600(INNER_GIMBAL_WIRE),
        AS5600(LEFT_VANE_WIRE),
        AS5600(RIGHT_VANE_WIRE)
    };
};

#endif