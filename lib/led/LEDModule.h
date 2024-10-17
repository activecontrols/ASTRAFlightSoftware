/**
 * Module that blinks the integrated LED in multiple modes.
 * Simple proof-of-concept FlightModule; if you're not sure how to write a new module,
 * start by copying this one.
 */
#ifndef LED_MODULE_H
#define LED_MODULE_H

#include "FlightModule.h"
#include <Arduino.h>

/* 
 * To declare dependency on some data existing, use the flightData namespace
 * and declare data as extern. Backing storage for the data can be defined in main.cpp (recommended)
 * or elsewhere.
 */
namespace flightData {
    extern int ledMode;
};

class LEDModule: public FlightModule {
public:
    LEDModule();
    LEDModule(int pin);
    int init() override;
    void update() override;
private:
    void blink(unsigned long threshold);
    int pin = LED_BUILTIN;
    int ledOn = false;
    unsigned long lastUpdate = 0;
};

#endif // LED_MODULE_H