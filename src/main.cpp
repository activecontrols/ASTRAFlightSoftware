#include "Buffer.h"
#include "Estimator.h"
#include "Controller.h"
#include "Integrator.h"
#include "Derivative.h"
#include "IMU.h"
#include "Encoder.h"
#include "timer.h"
#include "settings.h"

#include <Servo.h>
#include <ArduinoEigen.h>

/*
main.cpp 
Description: Currently used to run tests for the entire flight software
Author: Vincent Palmerio
*/

// BATTERY VOLTAGE VARIABLES

#define battVPin 41 // SHOULD BE THE RIGHT PIN
#define CHECK_BATTERY_VOLTAGE 1
elapsedMillis fastLedTime;

#if USE_COMMS
  #include "Comms.h"
#endif
#if LOG_DATA
  #include "log.h"
#endif

elapsedMillis ledTime;

elapsedMicros totalTimeElapsed;
int lastTime = 0;

bool ledOn = false;

#if LOG_DATA
logger::Data data;
#endif

#if USE_COMMS
CommsManager comms;

fmav_traj_ack_t loadSD(int number) {
    comms.sendStatusText(MAV_SEVERITY_INFO, (String("DEBUG: Loading Mission #") + String(number)).c_str());

    // Return example affirmative
    fmav_traj_ack_t ack;
    ack.result = MAV_RESULT_ACCEPTED;
    ack.points_loaded = 592; // I made up a number
    return ack;
}
#endif

void setup() {
  Serial.begin(9600);
  Serial1.begin(57600);
  
  //Sets up led
  pinMode(LED_BUILTIN, OUTPUT);
  //sets LED to on indefinitely so we know teensy is on if setup() fails
  digitalWrite(LED_BUILTIN, HIGH); 
  

#if CHECK_BATTERY_VOLTAGE
  pinMode(battVPin, INPUT);
#endif

#if USE_COMMS
  Serial.print("Set up comms...");
  comms.init();
  comms.registerTrajSDLoadAction(loadSD);
  Serial.println("Done");
#endif

  estimator::initializeEstimator();

  controller::initializeController();
  delay(4000);

#if LOG_DATA
  initializeSD();
  logger::open("log.bin");
#endif

  timer::startMissionTimer();
  totalTimeElapsed = 0;
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
    
    ledTime = 0;
  }
}

//Mainly for battery voltage checker, could possibly be used for other things too
void fastLED() {
  if (fastLedTime >= 400) {

    //(HIGH and LOW are the voltage levels)
    if (ledOn == true) {
      digitalWrite(LED_BUILTIN, LOW);
      ledOn = false;
    } else if (ledOn == false) {
      digitalWrite(LED_BUILTIN, HIGH);
      ledOn = true;
    }
    
    fastLedTime = 0;
  }
}

void loop() {
  Serial.print("Loop time: ");
  Serial.println(totalTimeElapsed - lastTime);
  
#if CHECK_BATTERY_VOLTAGE
  int pinVal = analogRead(battVPin); //Reading analog pin
  float batteryV = (pinVal * 10) / 1023; //Converting analog signal into battery voltage

  if (batteryV < 6) {
    Serial.print("BATTERY VOLTAGE IS CRITICALLY LOW (<6.0V)");
    fastLED();
  }
  else if (batteryV < 6.4) {
    Serial.print("BATTERY VOLTAGE IS VERY LOW (<6.4V)");
    fastLED();
  }
  else if (batteryV < 7) {
    Serial.print("BATTERY VOLTAGE IS LOW (<7V)");
  }
#endif

  lastTime = totalTimeElapsed;
  
  estimator::updateEstimator();
  controller::updateController();

#if USE_COMMS
  comms.spin();
  comms.sendStatusText(MAV_SEVERITY_INFO, "Time between loop:");
  comms.sendStatusText(MAV_SEVERITY_INFO, String(totalTimeElapsed-lastTime).c_str());
#endif

#if LOG_DATA
  data.t = timer::getMissionTimeSeconds();
  data.battVoltage = analogRead(21); /* if the values are inaccurate, try analogReadAveraging() */
  for (int i = 0; i < ESTIMATED_STATE_DIMENSION; i++) {
    data.x[i] = estimator::estimatedStateX(i);
  }
  for (int i = 0; i < MEASUREMENT_DIMENSION; i++) {
    data.y[i] = estimator::measurementVectorY(i);
  }
  for (int i = 0; i < U_ROW_LENGTH; i++) {
    data.u[i] = controllerInputU(i);
  }
  logger::write(&data);
  Serial.printf("Delay: %.2f ms. Wrote: %d\n", (double) (totalTimeElapsed-lastTime) / 1000.0, logger::write_count);
#endif

  
  //DEBUGGING STATEMENTS ----------------------------------------------
  Serial.print("Time Elasped: ");
  Serial.println(millis()/1000.0, 3);

  Serial.print("Estimated State Vector: ");
  for (byte i = 0; i < ESTIMATED_STATE_DIMENSION; i++) {
    Serial.print(estimator::estimatedStateX(i), 3);
    if (i != ESTIMATED_STATE_DIMENSION - 1) Serial.print(", ");
  }
  Serial.println();

  Serial.print("Control Vector: ");
  for (byte i = 0; i < U_ROW_LENGTH; i++) {
    Serial.print(controller::controllerInputU(i), 3);
    if (i != U_ROW_LENGTH - 1) Serial.print(", ");
  }
  Serial.println();
  // -------------------------------------------------------------------
  
  //turns led on and off
  led();
}

