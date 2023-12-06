#include <Arduino.h>
#include "../lib/estimator/Estimator.h"
#include "../lib/controller/Controller.h"
#include "../lib/math/Integrator.h"
#include "../lib/math/Derivative.h"
#include "../lib/comms/Comms.h"
//#include <ArduinoEigenDense.h>

Eigen::MatrixXd m(3, 3);
Eigen::VectorXd v(3);

Eigen::VectorXd integrateAndDerive(3);

CommsManager comms;

elapsedMillis ledTime;
int lastLED;

bool ledOn = false;

fmav_traj_ack_t loadSD(int number) {
    comms.sendStatusText(MAV_SEVERITY_INFO, (String("DEBUG: Loading Mission #") + String(number)).c_str());

    // Return example affirmative
    fmav_traj_ack_t ack;
    ack.result = MAV_RESULT_ACCEPTED;
    ack.points_loaded = 592; // I made up a number
    return ack;
}

void setup() {
  Serial.print("Controller error code:");
  Serial.print(controllerSetup());

  Serial.print("Estimator error code:");
  Serial.print(estimatorSetup());

  Serial.print("Set up comms...");
  comms.init();
  comms.registerTrajSDLoadAction(loadSD);
  Serial.println("Done");

  integrateAndDerive << 1, 2, 3;

  Serial.print("Integrator error code:");
  Serial.print(integratorSetup(&integrateAndDerive));

  Serial.print("Derivative error code:");
  Serial.print(derivativeSetup(&integrateAndDerive));
  
  //Sets up led
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

  //Undefined Vector Test
  Serial.println("Vector w/o initialized values: ");
  Serial.print(v(0));
  Serial.println();
  Serial.print(v(1));
  Serial.println();
  Serial.print(v(2));
  Serial.println();

  v(0) = 0;
  v(1) = 1;
  v(2) = 0;
   
  

}

//turns the LED on and off every 3 seconds 
void led() {
  
  if (ledTime == 3000) {

    //(HIGH and LOW are the voltage levels)
    if (ledOn == true) {
      digitalWrite(LED_BUILTIN, LOW);
      ledOn = false;
    } else if (ledOn == false) {
      digitalWrite(LED_BUILTIN, HIGH);
      ledOn = true;
    }
    

    ledTime -= 3000;
  }
}

void loop() {
  comms.spin();
  if (millis() - lastLED > 1000) {
    Eigen::VectorXd vo = m * v;
    v = vo;

    integrateAndDerive *= 2;

    integratorUpdate();
    Serial.println("Integration");
    for (int i = 0; i < integratedData.size(); i++) {
      Serial.println(integratedData(i));
    }

    derivativeUpdate();
    Serial.println("Derivative");
    for (int i = 0; i < derivative.size(); i++) {
      Serial.println(derivative(i));
    }

    Serial.println("New Vector: ");
    Serial.print(v(0));
    Serial.println();
    Serial.print(v(1));
    Serial.println();
    Serial.print(v(2));
    Serial.println();

    // turns led on and off
    led();
    lastLED = millis();
  }
}

