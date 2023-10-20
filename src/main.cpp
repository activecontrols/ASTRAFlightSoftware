#include <Arduino.h>
#include "../lib/estimator/Estimator.h"
#include "../lib/controller/Controller.h"
#include "../lib/math/Integrator.h"
//#include <ArduinoEigenDense.h>

Eigen::MatrixXd m(3, 3);
Eigen::VectorXd v(3);

void setup() {
  Serial.print("Controller error code:");
  Serial.print(controllerSetup());

  Serial.print("Estimator error code:");
  Serial.print(estimatorSetup());
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);

  for (unsigned int i = 0; i<U_ARRAY_LENGTH; i++) {
    Serial.print(controllerInputU(i));
  }

  for (unsigned int i = 0; i<STATE_DIMENSION; i++) {
    Serial.print(estimatedStateX[i]);
  }

  float theta = PI/2;


  
  m(0, 0) = 1;
  m(1, 0) = 0;
  m(2, 0) = 0;
  m(0, 1) = 0;
  m(0, 2) = 0;
  m(1, 1) = cos(theta);
  m(1, 2) = -sin(theta);
  m(2, 1) = sin(theta);
  m(2, 2) = cos(theta);

  

  v(0) = 0;
  v(1) = 1;
  v(2) = 0;
   
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.print("ITS WORKING!!!");
  
  Eigen::VectorXd vo = m * v;
  v = vo;

  Serial.println("New Vector: ");
  Serial.print(v(0));
  Serial.println();
  Serial.print(v(1));
  Serial.println();
  Serial.print(v(2));
  Serial.println();

  // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, HIGH);
  // wait for a second
  delay(3000);
  //elapsedMillis
  // turn the LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN, LOW);
   // wait for a second
  delay(1000);
}