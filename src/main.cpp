#include <Arduino.h>
#include "../lib/estimator/Estimator.h"
#include "../lib/controller/Controller.h"
#include "../lib/math/Integrator.h"
#include "../lib/math/Derivative.h"
//#include <ArduinoEigenDense.h>

Eigen::MatrixXd m(24, 24);
Eigen::VectorXd v(24);

Eigen::VectorXd integrateAndDerive(3);

elapsedMillis ledTime;

elapsedMillis timeToEnd;

bool ledOn = false;

int controllerErrorCode = -20;
int estimatorErrorCode = -20;
int integratorErrorCode = -20;
int derivativeErrorCode = -20;

void setup() {
  Serial.print("Controller error code:");
  controllerErrorCode = controllerSetup();
  Serial.println(controllerErrorCode);

  Serial.print("Estimator error code:");
  estimatorErrorCode = estimatorSetup();
  Serial.println(estimatorErrorCode);

  integrateAndDerive << 1, 2, 3;

  Serial.print("Integrator error code:");
  integratorErrorCode = integratorSetup(&integrateAndDerive);
  Serial.println(integratorErrorCode);

  Serial.print("Derivative error code:");
  derivativeErrorCode = derivativeSetup(&integrateAndDerive, integrateAndDerive);
  Serial.println(derivativeErrorCode);
  
  //Sets up led
  pinMode(LED_BUILTIN, OUTPUT);


  for (unsigned int i = 0; i<U_ARRAY_LENGTH; i++) {
    Serial.print(controllerInputU(i));
  }

  for (unsigned int i = 0; i<STATE_DIMENSION; i++) {
    Serial.print(estimatedStateX[i]);
  }

  float theta = PI/2;


  //ALWAYS INITIALIZE EACH VALUE IN VECTORS AND MATRICES
  for (int i = 0; i < 24; i++) {
    for (int j = 0; j < 24; j++) {
      m(i, j) = 2;
    }
  }

  for (int i = 0; i < 24; i++) {
    v(i) = 2;
  }

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
  
  //Matrix Multiplication test
  Eigen::VectorXd vo = m * v;
  v = vo;


  // Serial.print("Controller error code:");
  // Serial.println(controllerErrorCode);

  // Serial.print("Estimator error code:");
  // Serial.println(estimatorErrorCode);

  // Serial.print("Integrator error code:");
  // Serial.println(integratorErrorCode);

  // Serial.print("Derivative error code:");
  // Serial.println(derivativeErrorCode);

  //integrateAndDerive += 2;
  //2, 4, 6

  
  integrateAndDerive(0) = integrateAndDerive(0) + 2;
  integrateAndDerive(1) = integrateAndDerive(1) + 2;
  integrateAndDerive(2) = integrateAndDerive(2) + 2;

  Serial.println("Main.cpp Vector");
  for (int i = 0; i < integrateAndDerive.size(); i++) {
    Serial.println(integrateAndDerive(i));
  }

  Serial.println("Integration data");
  integratorUpdate();
  Serial.println("Integration");
  for (int i = 0; i < integratedData.size(); i++) {
    Serial.println(integratedData(i), 60);
  }

  Serial.println("Derivative data");
  derivativeUpdate();
  Serial.println("Derivative");
  for (int i = 0; i < derivative.size(); i++) {
    Serial.println(derivative(i), 60);
  }

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
  //delay(4000);

  //end program after 100 millis
  if (timeToEnd >= 100) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(20000000);
  }
}

