
#include "ESC.h"

//#include <Controller.h>
#include <Error.h>
#include <Arduino.h>
#include <Servo.h>

#define THROTTLE_MIN (0)
#define THROTTLE_MAX (1)

//ESC PWM endpoints
const int PWM_LOW = 1020;
const int PWM_HIGH = 1980;

//number of times to arm
int arm_tries = 10;

static float _throttleToPWM(float throttle) {
    return map(throttle, THROTTLE_MIN, THROTTLE_MAX, PWM_LOW, PWM_HIGH);
}

static bool _validPWM(int8_t pwm_value) {
    return ((pwm_value >= PWM_LOW) && (pwm_value <= PWM_HIGH));
}

ESC::ESC(int _throttle_pin) {
    throttle_pin = _throttle_pin;

    // initialize esc pwm out
    throttle_servo.attach(throttle_pin);
    // write default value of 0 to ESC on startup
    throttle_servo.writeMicroseconds(0);

    // arm motor
    for (byte i = 0; i < arm_tries; i++) {
        throttle_servo.writeMicroseconds(1000);
    }

    throttle = 0;
    buffer_loc = NULL;
}

bool ESC::checkAttached() {
    return throttle_servo.attached();
}

int8_t ESC::getTemp() {
    if (buffer_loc == NULL) {
        return INVALID_ESC_BUFFER_INIT;
    }

    return buffer_loc[0];
}


int8_t ESC::getVoltageHigh() {
    if (buffer_loc == NULL) {
        return INVALID_ESC_BUFFER_INIT;
    }

    return buffer_loc[1];
}

int8_t ESC::getVoltageLow() {
    if (buffer_loc == NULL) {
        return INVALID_ESC_BUFFER_INIT;
    }

    return buffer_loc[2];
}

int8_t ESC::getCurrentHigh() {
    if (buffer_loc == NULL) {
        return INVALID_ESC_BUFFER_INIT;
    }

    return buffer_loc[3];
}

int8_t ESC::getCurrentLow() {
    if (buffer_loc == NULL) {
        return INVALID_ESC_BUFFER_INIT;
    }

    return buffer_loc[4];
}

int8_t ESC::getConsumptionHigh() {
    if (buffer_loc == NULL) {
        return INVALID_ESC_BUFFER_INIT;
    }

    return buffer_loc[5];
}

int8_t ESC::getConsumptionLow() {
    if (buffer_loc == NULL) {
        return INVALID_ESC_BUFFER_INIT;
    }

    return buffer_loc[6];
}

int8_t ESC::getERMPHigh() {
    if (buffer_loc == NULL) {
        return INVALID_ESC_BUFFER_INIT;
    }

    return buffer_loc[7];
}

int8_t ESC::getERMPLow() {
    if (buffer_loc == NULL) {
        return INVALID_ESC_BUFFER_INIT;
    }

    return buffer_loc[8];
}

int8_t ESC::getCRC8() {
    if (buffer_loc == NULL) {
        return INVALID_ESC_BUFFER_INIT;
    }

    return buffer_loc[9];
}

int8_t ESC::getThrottle() {
    return throttle;
}

int ESC::setTLMBufferLocation(int8_t* a_buffer) {
    if (a_buffer != NULL) {
        return INVALID_ESC_BUFFER_INIT;
    }

    buffer_loc = a_buffer;
    return NO_ERROR_CODE;
}

int ESC::setThrottle(float _throttle) {

    float PWM_output = _throttleToPWM(_throttle);

    if (_validPWM(PWM_output)) {
        throttle = _throttle;
        throttle_servo.writeMicroseconds(PWM_output);
    } else {
        return INVALID_THROTTLE_ESC;
    }

    return NO_ERROR_CODE;
}