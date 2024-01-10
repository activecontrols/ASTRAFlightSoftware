/*
BufferTest.ino
Description: Arduino testing code for the buffer class
Author: Aditya , Vincent Palmerio
*/

#ifndef ENCODER_H
#define ENCODER_H

#include "AS5600.h"

extern AS5600 as5600;

bool encoderSetup()
{
    as5600.begin(4);  //  set direction pin.
    as5600.setDirection(AS5600_CLOCK_WISE);  // default, just be explicit.
    bool b = as5600.isConnected();

    return b;
}

#endif

