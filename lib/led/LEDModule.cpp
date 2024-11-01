#include "LEDModule.h"
#include "Error.h"
#include <Arduino.h>
#include <Logger.h>

#define SLOW_THRESHOLD 1000
#define FAST_THRESHOLD 400

LEDModule::LEDModule() {
    this->pin = LED_BUILTIN;
}

LEDModule::LEDModule(int pin) {
    this->pin = pin;
}

int LEDModule::init() {
    flightData::ledMode = 0;
    pinMode(this->pin, OUTPUT);
    //sets LED to on indefinitely so we know teensy is on if setup() fails
    digitalWrite(this->pin, HIGH); 
    return NO_ERROR_CODE;
}

void LEDModule::update(unsigned long time) {
    switch (flightData::ledMode) {
        case 1:
            this->blink(time, SLOW_THRESHOLD);
            break;
        case 2:
            this->blink(time, FAST_THRESHOLD);
            break;
        default:
            if (this->ledOn) {
                digitalWrite(this->pin, LOW);
                this->ledOn = false;
            }
            break;
    }
}

void LEDModule::blink(unsigned long time, unsigned long threshold) {
    if (time - this->lastUpdate > threshold * 1000) {
        if (this->ledOn) {
            digitalWrite(this->pin, HIGH);
            this->ledOn = false;
        } else {
            digitalWrite(this->pin, LOW);
            this->ledOn = true;
        }
    }
}