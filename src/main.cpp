#include <Arduino.h>
#include <Adafruit_LSM6DSOX.h>
#include "../lib/buffer/Buffer.h"
#include "../lib/estimator/Estimator.h"
#include "../lib/controller/Controller.h"
#include "../lib/math/Integrator.h"
#include "../lib/math/Derivative.h"
#include "../lib/drivers/ASTRA/IMU/src/IMU.h"
#include "../lib/encoders/AS5600.h"
#include <Servo.h>
//#include <ArduinoEigenDense.h>

/*
main.cpp 
Description: Currently used to run tests for the entire flight software
Author: Vincent Palmerio
Last updated: 11/4/2023
*/

Eigen::MatrixXd m(24, 24);
Eigen::VectorXd v(24);

Eigen::VectorXd integrateAndDeriveTest(3);

Integrator gyroIntegrator;
Eigen::VectorXd gyroVector(3);

elapsedMillis ledTime;

elapsedMicros totalTimeElapsed;
int lastTime = 0;

bool ledOn = false;

//ERROR CODES
int controllerErrorCode = -20;
int estimatorErrorCode = -20;
int integratorErrorCode = -20;
int integratorGyroErrorCode = -20;
int derivativeErrorCode = -20;

//SERVOS
Servo beta;
Servo alpha;

//ENCODER
AS5600 as5600;

Buffer imuBuffer(3,5, getValues);
float ** data;
float* test;

void setup() {

  //Sets up led
  pinMode(LED_BUILTIN, OUTPUT);
  //sets LED to on indefinitely so we know teensy is on if setup() fails
  digitalWrite(LED_BUILTIN, HIGH); 

  //ENCODER SETUP
  // as5600.begin(4);  //  set direction pin.
  // as5600.setDirection(AS5600_CLOCK_WISE);  // default, just be explicit.
  // int b = as5600.isConnected();
  //---

  //IMU SETUP
  initializeIMU();
  //---

  //GYROSCOPE (IMU) INTEGRATOR SETUP
  Serial.print("Integrator error code:");
  integratorGyroErrorCode = gyroIntegrator.integratorSetup(&gyroVector);
  
  Serial.println(integratorGyroErrorCode);
  //---

  //SERVO SETUP
  beta.attach(2);
  alpha.attach(3);

  beta.write(90);
  alpha.write(90);
  //---

  
  // Serial.print("Derivative error code:");
  // derivativeErrorCode = derivativeSetup(&integrateAndDeriveTest, integrateAndDeriveTest);
  // Serial.println(derivativeErrorCode);

  // Serial.print("Controller error code:");
  // controllerErrorCode = controllerSetup();
  // Serial.println(controllerErrorCode);

  // Serial.print("Estimator error code:");
  // estimatorErrorCode = estimatorSetup();
  // Serial.println(estimatorErrorCode);

  // integrateAndDeriveTest << 1, 2, 3;

  // Serial.print("Integrator error code:");
  // integratorErrorCode = integratorSetup(&integrateAndDeriveTest);
  // Serial.println(integratorErrorCode);

  // Serial.print("Derivative error code:");
  // derivativeErrorCode = derivativeSetup(&integrateAndDeriveTest, integrateAndDeriveTest);
  // Serial.println(derivativeErrorCode);
  
  


  // for (unsigned int i = 0; i<U_ARRAY_LENGTH; i++) {
  //   Serial.print(controllerInputU(i));
  // }

  // for (unsigned int i = 0; i<STATE_DIMENSION; i++) {
  //   Serial.print(estimatedStateX[i]);
  // }

  // float theta = PI/2;


  // //ALWAYS INITIALIZE EACH VALUE IN VECTORS AND MATRICES
  // for (int i = 0; i < 24; i++) {
  //   for (int j = 0; j < 24; j++) {
  //     m(i, j) = 2;
  //   }
  // }

  // for (int i = 0; i < 24; i++) {
  //   v(i) = 2;
  // }

  

  //Undefined Vector Test
  // Serial.println("Vector w/o initialized values: ");
  // Serial.print(v(0));
  // Serial.println();
  // Serial.print(v(1));
  // Serial.println();
  // Serial.print(v(2));
  // Serial.println();
}

//turns the LED on and off every 3 seconds 
void led() {
  
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
  
  Serial.print("Time between loop: ");
  Serial.println(totalTimeElapsed-lastTime);
  lastTime = totalTimeElapsed;

  updateIMU();

  imuBuffer.addData();
  data = imuBuffer.getData();

  imuBuffer.printData();
  
  // Serial.print(millis());
  // Serial.print("\t");
  // Serial.print(as5600.readAngle());
  // Serial.print("\t");
  // Serial.println(as5600.rawAngle());
  // Serial.print("\t");
  // Serial.println(as5600.rawAngle() * AS5600_RAW_TO_DEGREES);

  // Serial.println("Sensor Fusion");
  // Serial.print("Roll: ");
  // Serial.print(roll);
  // Serial.print("Pitch: ");
  // Serial.print(pitch);
  // Serial.print(", Heading: ");
  // Serial.print(heading);

  gyroVector << gx, gy, gz;
  Serial.println("Integration data");
  gyroIntegrator.integratorUpdate();
  for (int i = 0; i < gyroIntegrator.integratedData.size(); i++) {
    Serial.println(gyroIntegrator.integratedData(i), 60);
  }
  
  int x = gyroIntegrator.integratedData(0);
  int y = gyroIntegrator.integratedData(1);

  int limit = 15;

  if (x > limit) {
    x = limit;
  } else if (x < -limit) {
    x = -limit;
  }

  if (y > limit) {
    y = limit;
  } else if (y < -limit) {
    y = -limit;
  }

  x += 90;
  y += 90;

  beta.write(x);
  alpha.write(y);

  delayMicroseconds(100);

  //int x_actual = beta.read();

  //Matrix Multiplication test
  // Eigen::VectorXd vo = m * v;
  // v = vo;


  // Serial.print("Controller error code:");
  // Serial.println(controllerErrorCode);

  // Serial.print("Estimator error code:");
  // Serial.println(estimatorErrorCode);

  // Serial.print("Integrator error code:");
  // Serial.println(integratorErrorCode);

  // Serial.print("Derivative error code:");
  // Serial.println(derivativeErrorCode);

  //integrateAndDeriveTest += 2;
  //2, 4, 6

  //Integrator and Derivative Test
  // integrateAndDeriveTest(0) = integrateAndDeriveTest(0) + 2;
  // integrateAndDeriveTest(1) = integrateAndDeriveTest(1) + 2;
  // integrateAndDeriveTest(2) = integrateAndDeriveTest(2) + 2;

  // Serial.println("Main.cpp Vector");
  // for (int i = 0; i < integrateAndDeriveTest.size(); i++) {
  //   Serial.println(integrateAndDeriveTest(i));
  // }

  // Serial.println("Integration data");
  // integratorUpdate();
  // Serial.println("Integration");
  // for (int i = 0; i < integratedData.size(); i++) {
  //   Serial.println(integratedData(i), 60);
  // }

  // Serial.println("Derivative data");
  // derivativeUpdate();
  // Serial.println("Derivative");
  // for (int i = 0; i < derivative.size(); i++) {
  //   Serial.println(derivative(i), 60);
  // }

  // Serial.println("New Vector: ");
  // Serial.print(v(0));
  // Serial.println();
  // Serial.print(v(1));
  // Serial.println();
  // Serial.print(v(2));
  // Serial.println();

  //turns led on and off
  led();
  

  // wait for a second
  //delay(1000);

  //end program after 1000000 micro seconds
  // if (totalTimeElapsed >= 1000000) {
  //   digitalWrite(LED_BUILTIN, HIGH);
  //   delay(20000000);
  // }
}

