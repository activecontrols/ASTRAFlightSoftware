/**
 * Module that reads battery voltage.
 */
#ifndef VOLTAGE_MODULE_H
#define VOLTAGE_MODULE_H

#include "FlightModule.h"
#include <Arduino.h>

/* 
 * To declare dependency on some data existing, use the flightData namespace
 * and declare data as extern. Backing storage for the data can be defined in main.cpp (recommended)
 * or elsewhere.
 */
namespace flightData {
    extern int ledMode;
    extern float voltage[];
};

class VoltageModule: public FlightModule {
public:
    VoltageModule(int idx, int battVPin);
    int init() override;
    void update(unsigned long time) override;
private:
    float *voltagePtr;
    int battVPin;
};

#endif // VOLTAGE_MODULE_H