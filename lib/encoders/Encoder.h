/*
BufferTest.ino
Description: Arduino testing code for the buffer class
Author: Aditya , Vincent Palmerio
*/

#ifndef ENCODER_H
#define ENCODER_H

#include "AS5600.h"

extern AS5600 magEncoder1;
extern AS5600 magEncoder2;

bool encoderSetup(int pin1, int pin2)
{
    magEncoder1.begin(pin1);  //  set direction pin.
    magEncoder1.setDirection(AS5600_CLOCK_WISE);  // default, just be explicit.
    bool a = magEncoder1.isConnected();

    magEncoder2.begin(pin2);  //  set direction pin.
    magEncoder2.setDirection(AS5600_CLOCK_WISE);  // default, just be explicit.
    bool b = magEncoder2.isConnected();

    return a && b;
}

int getAngleEncoder1() {
    return magEncoder1.readAngle();
}

int getAngleEncoder2() {
    return magEncoder2.readAngle();
}

#endif

