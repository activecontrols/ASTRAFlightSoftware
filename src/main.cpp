#include "Scheduler.h"
#include "Router.h"
#include "IMU.h"
#include "FlightModule.h"
#include "VoltageModule.h"
//#include "NXPEstimatorModule.h"
#include "MEKFEstimatorModule.h"
#include "LEDModule.h"
#include "Comms.h"
#include "ControllerModule.h"
#include "EncoderModule.h"
#include "MotorModule.h"
#include "settings.h"
#include "VEigen.h"
#include <Arduino.h>
//#include "Logger.h"

#define BATT_V_PIN 41 // SHOULD BE THE RIGHT PIN

//#include <iostream>

namespace flightData {
  int ledMode = 0;
  CONTROL_MODE currentMode = CONTROL_MODE::STABILIZE_MODE;
  float voltage[1] = {0.0f};
  float encoderPos[4] = {0.0f};
  float encoderSpeeds[4] = {0.0f};
  Router *router;
  Eigen::VectorXd measurementVectorY(9);
  Eigen::VectorXd estimatedStateX(6);
  Eigen::VectorXd controllerInputU(4);
}

LEDModule ledModule;
VoltageModule voltageModule(0, BATT_V_PIN);
IMUModule imuModule;
MEKFEstimatorModule estimatorModule;
CommsManager commsManager;
Controller controllerModule;
Encoder encoderModule;
MotorModule motorModule;
//Logger logger(LogLevel::INFO, LogLevel::INFO);

FlightModule* basicSchedule[] = {
 (FlightModule*) &ledModule,
//  (FlightModule*) &voltageModule,
//  (FlightModule*) &imuModule,
#if USE_ENCODER
  (FlightModule*) &encoderModule,
#endif
  // (FlightModule*) &estimatorModule,
  // (FlightModule*) &controllerModule,
  // (FlightModule*) &motorModule,
  (FlightModule*) &commsManager,
};

constexpr int scheduleSize = sizeof(basicSchedule) / sizeof(FlightModule*);

Scheduler scheduler(basicSchedule, scheduleSize);
Router centralRouter;
void setup() {
  Serial.begin(9600);
  // Serial1.begin(57600);

  flightData::router = &centralRouter;
  centralRouter.registerSchedule(ASTRA_MAINLOOP, &scheduler);
  centralRouter.changeSchedule(ASTRA_MAINLOOP);

  centralRouter.init();
}

void loop() {
  delay(1000);
  centralRouter.update(micros());
}