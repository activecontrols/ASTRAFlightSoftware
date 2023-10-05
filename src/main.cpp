#include <Arduino.h>
#include "../lib/estimator/Estimator.h"
#include "../lib/controller/Controller.h"


// put function declarations here:
int myFunction(int, int);

void setup() {
  controllerSetup();
  estimatorSetup();
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("ITS WORKING!!!");
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}