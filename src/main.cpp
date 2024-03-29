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

#if USE_COMMS
  Serial.print("Set up comms...");
  comms.init();
  comms.registerTrajSDLoadAction(loadSD);
  Serial.println("Done");
#endif

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
  delay(4000);

#if LOG_DATA
  initializeSD();
  logger::open("log.bin");
#endif

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
    
    ledTime = 0;
  }
}

void loop() {

  lastTime = totalTimeElapsed;

  Eigen::VectorXd controllerInputU(U_ROW_LENGTH);
  controllerInputU = controller::getControlInputs();

  updateEstimator();
  controller::updateController();

#if USE_COMMS
  comms.spin();
  comms.sendStatusText(MAV_SEVERITY_INFO, "Time between loop:");
  comms.sendStatusText(MAV_SEVERITY_INFO, String(totalTimeElapsed-lastTime).c_str());
#endif

#if LOG_DATA
  data.t = getMissionTimeSeconds();
  data.battVoltage = analogRead(21); /* if the values are inaccurate, try analogReadAveraging() */
  for (int i = 0; i < ESTIMATED_STATE_DIMENSION; i++) {
    data.x[i] = estimatedStateX(i);
  }
  for (int i = 0; i < MEASUREMENT_DIMENSION; i++) {
    data.y[i] = measurementVectorY(i);
  }
  for (int i = 0; i < U_ROW_LENGTH; i++) {
    data.u[i] = controllerInputU(i);
  }
  logger::write(&data);
  Serial.printf("Delay: %.2f ms. Wrote: %d\n", (double) (totalTimeElapsed-lastTime) / 1000.0, logger::write_count);
#endif

  

  // controllerInputU; //the vector to access for outputs
  // for (int i = 0; i < controllerInputU.size(); i++) {
  //   Serial.print(" ");
  //   Serial.print(controllerInputU(i));
  // }
  // Serial.println();

  Serial.print(millis()/1000.0, 3); Serial.print(", ");
  for (byte i = 0; i < ESTIMATED_STATE_DIMENSION; i++) {
    Serial.print(estimatedStateX(i), 3);
    if (i != ESTIMATED_STATE_DIMENSION - 1) Serial.print(", ");
  }
  Serial.println();
  for (byte i = 0; i < U_ROW_LENGTH; i++) {
    Serial.print(controllerInputU(i), 3);
    if (i != U_ROW_LENGTH - 1) Serial.print(", ");
  }
  Serial.println();
  
  //turns led on and off
  led();
}

