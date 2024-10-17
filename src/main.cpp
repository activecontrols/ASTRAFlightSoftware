#include "Scheduler.h"
#include "IMU.h"
#include "FlightModule.h"
#include "VoltageModule.h"
#include "NXPEstimatorModule.h"
#include "LEDModule.h"
#include <Arduino.h>

#define BATT_V_PIN 41 // SHOULD BE THE RIGHT PIN

namespace flightData {
  int ledMode = 0;
  float voltage[1] = {0.0f};
  Eigen::VectorXd measurementVectorY(9);
  Eigen::VectorXd estimatedStateX(6);
}

std::vector<FlightModule> basicSchedule = {
  LEDModule(),
  VoltageModule(0, BATT_V_PIN),
  IMUModule(),
  NXPEstimatorModule(),
};
Scheduler scheduler(basicSchedule);

void setup() {
  Serial.begin(9600);
  Serial1.begin(57600);
  scheduler.init();
}

void loop() {
  scheduler.update();
}