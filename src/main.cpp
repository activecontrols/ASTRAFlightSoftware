//#include <Arduino.h>
//#include "../lib/estimator/Estimator.h"
//#include "../lib/controller/Controller.h"
#include "C:\Users\BizzM\OneDrive\Documents\GitHub\ASTRAFlightSoftware\lib\Buffer_development\buffer.cpp"
#include <iostream>

using namespace std;
// put function declarations here:
int myFunction(int, int);

void setup() {
  //controllerSetup();
  //estimatorSetup();
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
  cout << ("ITS WORKING!!!");
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}