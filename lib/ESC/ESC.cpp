
#include "ESC.h"

#include <HX711.h>
#include <Servo.h>

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 5;
const int LOADCELL_SCK_PIN = 4;
const int TARE_BUTTON = 2;

//ESC PWM wiring
const int ESC_PIN = 9;

//ESC PWM endpoints
const int PWM_LOW = 1020;
const int PWM_HIGH = 1980;

//number of times to arm
int arm_tries = 10;

//number of transient states to record
int transient_record = 100;

const double a = -0.0141443;
const double b = 0.000449718;

HX711 scale;

static float _throttleToPWM(float throttle) {
    return map(throttle, 0, 1.0, PWM_LOW, PWM_HIGH);
}

static bool _validPWM(int8_t pwm_value) {
    return ((pwm_value >= PWM_LOW) && (pwm_value <= PWM_HIGH));
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    // initialize tare button
    pinMode(TARE_BUTTON, INPUT_PULLDOWN);

    ESC esc = ESC(ESC_PIN);

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

int8_t ESC::getTemp() {
    if (buffer_loc == NULL) {
        return INVALID_BUFFER_LOC;
    }

    return buffer_loc[0];
}


int8_t ESC::getVoltageHigh() {
    if (buffer_loc == NULL) {
        return INVALID_BUFFER_LOC;
    }

    return buffer_loc[1];
}

int8_t ESC::getVoltageLow() {
    if (buffer_loc == NULL) {
        return INVALID_BUFFER_LOC;
    }

    return buffer_loc[2];
}

int8_t ESC::getCurrentHigh() {
    if (buffer_loc == NULL) {
        return INVALID_BUFFER_LOC;
    }

    return buffer_loc[3];
}

int8_t ESC::getCurrentLow() {
    if (buffer_loc == NULL) {
        return INVALID_BUFFER_LOC;
    }

    return buffer_loc[4];
}

int8_t ESC::getConsumptionHigh() {
    if (buffer_loc == NULL) {
        return INVALID_BUFFER_LOC;
    }

    return buffer_loc[5];
}

int8_t ESC::getConsumptionLow() {
    if (buffer_loc == NULL) {
        return INVALID_BUFFER_LOC;
    }

    return buffer_loc[6];
}

int8_t ESC::getERMPHigh() {
    if (buffer_loc == NULL) {
        return INVALID_BUFFER_LOC;
    }

    return buffer_loc[7];
}

int8_t ESC::getERMPLow() {
    if (buffer_loc == NULL) {
        return INVALID_BUFFER_LOC;
    }

    return buffer_loc[8];
}

int8_t ESC::getCRC8() {
    if (buffer_loc == NULL) {
        return INVALID_BUFFER_LOC;
    }

    return buffer_loc[9];
}

int8_t ESC::getThrottle() {
    return throttle;
}

ESC_Status ESC::setTLMBufferLocation(int8_t* a_buffer) {
    if (a_buffer != NULL) {
        return INVALID_BUFFER_LOC;
    }

    buffer_loc = a_buffer;
    return OK;
}

ESC_Status ESC::setThrottle(float _throttle) {
    throttle = _throttle;

    float PWM_output = _throttleToPWM(_throttle);

    if (_validPWM(PWM_output)) {
        throttle = _throttle;
        throttle_servo.writeMicroseconds(PWM_output);
    } else {
        return INVALID_THROTTLE;
    }

    return OK;
}