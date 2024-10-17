#include "LEDModule.h"
#include <Arduino.h>

#define SLOW_THRESHOLD 1000
#define FAST_THRESHOLD 400

LEDModule::LEDModule() {
    this->pin = LED_BUILTIN;
}

LEDModule::LEDModule(int pin) {
    this->pin = pin;
}

void LEDModule::init() {
    flightData::ledMode = 0;
    pinMode(this->pin, OUTPUT);
    //sets LED to on indefinitely so we know teensy is on if setup() fails
    digitalWrite(this->pin, HIGH); 
}

void LEDModule::update() {
    switch (flightData::ledMode) {
        case 1:
            this->blink(SLOW_THRESHOLD);
            break;
        case 2:
            this->blink(FAST_THRESHOLD);
            break;
        default:
            if (this->ledOn) {
                digitalWrite(this->pin, LOW);
                this->ledOn = false;
            }
            break;
    }
}

void LEDModule::blink(unsigned long threshold) {
    if (millis() - this->lastUpdate > threshold) {
        if (this->ledOn) {
            digitalWrite(this->pin, HIGH);
            this->ledOn = false;
        } else {
            digitalWrite(this->pin, LOW);
            this->ledOn = true;
        }
    }
}