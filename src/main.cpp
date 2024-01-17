#include <Arduino.h>
#include <Adafruit_LSM6DSOX.h>
#include "../lib/buffer/Buffer.h"
#include "../lib/estimator/Estimator.h"
#include "../lib/controller/Controller.h"
#include "../lib/math/Integrator.h"
#include "../lib/math/Derivative.h"
#include "../lib/comms/Comms.h"
#include "../lib/drivers/ASTRA/IMU/src/IMU.h"
#include "../lib/encoders/Encoder.h"
#include <Servo.h>
//#include <ArduinoEigenDense.h>

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
CommsManager comms;

Buffer imuBuffer(3,5, getValues);
float ** data;
float* test;

fmav_traj_ack_t loadSD(int number) {
    comms.sendStatusText(MAV_SEVERITY_INFO, (String("DEBUG: Loading Mission #") + String(number)).c_str());

    // Return example affirmative
    fmav_traj_ack_t ack;
    ack.result = MAV_RESULT_ACCEPTED;
    ack.points_loaded = 592; // I made up a number
    return ack;
}

void setup() {

  //Sets up led
  pinMode(LED_BUILTIN, OUTPUT);
  //sets LED to on indefinitely so we know teensy is on if setup() fails
  digitalWrite(LED_BUILTIN, HIGH); 

  //ENCODER SETUP
  while (!encoderSetup(1, 2)) {
    Serial.println("Connecting to encoder...");
  }
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
  //---

  initializeEstimator();
  initializeController();
  
}

//turns the LED on and off every 3 seconds 
void led() {

  getAngleEncoder1();
  getAngleEncoder2();
  
  if (ledTime >= 3000) {

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
  //comms.spin();
  // comms.sendStatusText(MAV_SEVERITY_INFO, "Time between loop:");
  // comms.sendStatusText(MAV_SEVERITY_INFO, String(totalTimeElapsed-lastTime).c_str());
  lastTime = totalTimeElapsed;

  updateEstimator();
  updateController();

  controllerInputU; //the vector to access for outputs
  for (int i = 0; i < controllerInputU.size(); i++) {
    Serial.print(" ");
    Serial.print(controllerInputU(i));
  }

  //turns led on and off
  led();
}

