#include "Scheduler.h"
#include "Router.h"
#include "IMU.h"
#include "FlightModule.h"
#include "VoltageModule.h"
#include "NXPEstimatorModule.h"
#include "LEDModule.h"
#include "Comms.h"
#include <Arduino.h>

#define BATT_V_PIN 41 // SHOULD BE THE RIGHT PIN

namespace flightData {
  int ledMode = 0;
  float voltage[1] = {0.0f};
  Eigen::VectorXd measurementVectorY(9);
  Eigen::VectorXd estimatedStateX(6);
  Router *router;
}

std::vector<FlightModule> basicSchedule = {
  LEDModule(),
  VoltageModule(0, BATT_V_PIN),
  IMUModule(),
  NXPEstimatorModule(),
  CommsManager()
};
Scheduler scheduler(basicSchedule);
Router centralRouter;
void setup() {
  Serial.begin(9600);
  Serial1.begin(57600);

  flightData::router = &centralRouter;
  centralRouter.registerSchedule(ASTRA_MAINLOOP, &scheduler);
  centralRouter.changeSchedule(ASTRA_MAINLOOP);

  centralRouter.init();
}

void loop() {
  centralRouter.update(micros());
}