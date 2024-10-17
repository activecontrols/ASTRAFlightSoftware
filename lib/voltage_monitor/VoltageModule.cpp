#include "VoltageModule.h"
#include "Error.h"
#include <Arduino.h>

#define SLOW_THRESHOLD 1000
#define FAST_THRESHOLD 400

VoltageModule::VoltageModule(int idx, int battVPin) {
    this->voltagePtr = (flightData::voltage + idx);
    this->battVPin = battVPin;
}

int VoltageModule::init() {
    *(this->voltagePtr) = 0;
    pinMode(this->battVPin, INPUT);
    return NO_ERROR_CODE;
}

void VoltageModule::update() {
  int pinVal = analogRead(this->battVPin); //Reading analog pin
  float batteryV = (pinVal * 10) / 1023; //Converting analog signal into battery voltage

  if (batteryV < 6) {
    // Serial.print("BATTERY VOLTAGE IS CRITICALLY LOW (<6.0V)");
    flightData::ledMode = 2;
  }
  else if (batteryV < 6.4) {
    // Serial.print("BATTERY VOLTAGE IS VERY LOW (<6.4V)");
    flightData::ledMode = 2;
  }
  else if (batteryV < 7) {
    // Serial.print("BATTERY VOLTAGE IS LOW (<7V)");
  }
}