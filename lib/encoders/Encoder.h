/*
Encoder.h
Description: Arduino testing code for the buffer class
Author: Aditya , Vincent Palmerio
*/

#ifndef ENCODER_H
#define ENCODER_H

#include "AS5600.h"

AS5600 magEncoder1;
AS5600 magEncoder2;

extern bool encoderSetup(int pin1, int pin2)
{
    magEncoder1.begin(pin1);  //  set direction pin.
    magEncoder1.setDirection(AS5600_CLOCK_WISE);  // default, just be explicit.
    bool a = magEncoder1.isConnected();

    magEncoder2.begin(pin2);  //  set direction pin.
    magEncoder2.setDirection(AS5600_CLOCK_WISE);  // default, just be explicit.
    bool b = magEncoder2.isConnected();

    return a && b;
}

/*
 * You must free the pointer and set it to NULL after using the pointer!
 */
extern float* getAngleEncoder1() {
    float* angle = (float*)malloc(1 * sizeof(float));
    angle[0] = (float) magEncoder1.readAngle();
    return angle;
}

/*
 * You must free the pointer and set it to NULL after using the pointer!
 */
extern float* getAngleEncoder2() {
    float* angle = (float*)malloc(1 * sizeof(float));
    angle[0] = (float) magEncoder1.readAngle();
    return angle;
}

#endif

