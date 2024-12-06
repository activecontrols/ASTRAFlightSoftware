/*
Encoder.h
Description: Arduino testing code for the buffer class
Author: Aditya , Vincent Palmerio
*/

#ifndef ENCODER_H
#define ENCODER_H

#include <settings.h>

#if USE_ENCODER
#include "AS5600.h"

namespace encoder {
    extern AS5600 magEncoder1;
    extern AS5600 magEncoder2;

    extern bool encoderSetup();

    /*
    * You must free the pointer and set it to NULL after using the pointer!
    */
    extern float* getAngleEncoder1();

    /*
    * You must free the pointer and set it to NULL after using the pointer!
    */
    extern float* getAngleEncoder2();

    extern float getSpeedEncoder1();

    extern float getSpeedEncoder2();
}
#endif
#endif
