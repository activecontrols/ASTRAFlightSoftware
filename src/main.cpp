#include <Arduino.h>
#include "Buffer.h"
#include "Estimator.h"
#include "Controller.h"
#include "Integrator.h"
#include "Derivative.h"
// #include "Comms.h"
#include "IMU.h"
#include "Encoder.h"
#include "timer.h"

#include <Servo.h>
#include <ArduinoEigen.h>
#include <stdexcept>

#define ASTRA_FULL_DEBUG

/*
main.cpp 
Description: Currently used to run tests for the entire flight software
Author: Vincent Palmerio
*/

elapsedMillis ledTime;

elapsedMicros totalTimeElapsed;
int lastTime = 0;

bool ledOn = false;

// COMMS
// CommsManager comms;

// Buffer imuBuffer(3,5, getValues);
float ** data;
float* test;

// fmav_traj_ack_t loadSD(int number) {
//     comms.sendStatusText(MAV_SEVERITY_INFO, (String("DEBUG: Loading Mission #") + String(number)).c_str());

//     // Return example affirmative
//     fmav_traj_ack_t ack;
//     ack.result = MAV_RESULT_ACCEPTED;
//     ack.points_loaded = 592; // I made up a number
//     return ack;
// }

void setup() {
  Serial.begin(9600);
  
  //Sets up led
  pinMode(LED_BUILTIN, OUTPUT);
  //sets LED to on indefinitely so we know teensy is on if setup() fails
  digitalWrite(LED_BUILTIN, HIGH); 

  //ENCODER SETUP
#if USE_ENCODER
  while (!encoderSetup()) {
    Serial.println("Connecting to encoder...");
  }
#endif
  //---
  // Serial.print("Set up comms...");
  //comms.init();
  //comms.registerTrajSDLoadAction(loadSD);
  // Serial.println("Done");

  //IMU SETUP
  int errorCode = initializeIMU();
  while(errorCode != 0) {
    Serial.print("Failed to initialize IMU, error code: ");
    Serial.print(errorCode);
    Serial.println(". Retrying...");
    errorCode = initializeIMU();
  }
  //- --

  initializeEstimator();
  controller::initializeController();

  startMissionTimer();
  
}

//turns the LED on and off every 3 seconds 
void led() {
  
  if (ledTime >= 1000) {

    //(HIGH and LOW are the voltage levels)
    if (ledOn == true) {
      digitalWrite(LED_BUILTIN, LOW);
      ledOn = false;
    } else if (ledOn == false) {
      digitalWrite(LED_BUILTIN, HIGH);
      ledOn = true;
    }
    
    // Serial.println("LED Time");
    // Serial.println(ledTime);

    ledTime = 0;
  }
}

void loop() {

  // if (logger::write_count > 1000) {
  //   logger::close();
  //   return;
  // }

  //comms.spin();
  // comms.sendStatusText(MAV_SEVERITY_INFO, "Time between loop:");
  // comms.sendStatusText(MAV_SEVERITY_INFO, String(totalTimeElapsed-lastTime).c_str());
  // Serial.printf("Delay: %.2f ms. Wrote: %d\n", (double) (totalTimeElapsed-lastTime) / 1000.0, logger::write_count);
  lastTime = totalTimeElapsed;

  // logger::Data data;
  // data.t = totalTimeElapsed / 1000.0;
  // data.battVoltage = 3.3 + sin(totalTimeElapsed / 100000.0);
  // for (int i = 0; i < 6; i++) {
  //   data.x[i] = 10.0 + i;
  // }
  // for (int i = 0; i < 9; i++) {
  //   data.y[i] = 20.0 + i;
  // }
  // for (int i = 0; i < 4; i++) {
  //   data.u[i] = 30.0 + i;
  // }
  // logger::write(&data);

  updateEstimator();
  controller::updateController();

  // controllerInputU; //the vector to access for outputs
  // for (int i = 0; i < controllerInputU.size(); i++) {
  //   Serial.print(" ");
  //   Serial.print(controllerInputU(i));
  // }
  // Serial.println();

  Eigen::VectorXd controllerInputU(U_ROW_LENGTH);
  controllerInputU = controller::getControlInputs();
  Serial.print(millis()/1000.0, 3); Serial.print(", ");
  for (byte i = 0; i < ESTIMATED_STATE_DIMENSION; i++) {
    Serial.print(estimatedStateX(i), 3); Serial.print(", ");
  }
  for (byte i = 0; i < U_ROW_LENGTH; i++) {
    Serial.print(controllerInputU(i), 3);
    if (i != U_ROW_LENGTH) Serial.print(", ");
  }
  Serial.println();
  // delay(1);
  //turns led on and off
  led();
}

