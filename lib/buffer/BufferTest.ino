#include <Arduino.h>
#include "../drivers/ASTRA/IMU/src/IMU.h"
#include "../encoders/Encoder.h"

/*
BufferTest.ino
Description: Arduino testing code for the buffer class
THERE IS A DATA FREE STATEMENT IN ADD DATA TO MAKE SURE DATA SPACE DOESNT RUN OUT. NEEDS TO BE TESTED. THERE ARE ATTACHED CODE PIECES IN IMU AND ENCODER DRIVERS
Author: Siddarth Goel
*/

class Buffer{

public:
Buffer(int elementSize, int numberOfElements, float* (*Fptr)() ){
    eleSize = elementSize;
    numElements = numberOfElements;
    data = new float*[numberOfElements];
    for(int i = 0 ; i < numberOfElements; i++){
        data[i] = new float[eleSize];
    }

    auxData = new float*[numberOfElements];
    for(int i = 0 ; i < numberOfElements; i++){
        auxData[i] = new float[eleSize];
    }
    top = -1;
    fptr = Fptr;
}

int getTop(){
  return top;
}

void addData(){
  float* add = fptr();
  float temp = 0;
  top++;
  if(top >= numElements){
      top = 0;
  }
  // Serial.println("Adding element to top = ");
  // Serial.print(top);
  for(int i = 0; i < eleSize; i++){
    // Serial.println("Adding element ");
    // Serial.print(i);
    // Serial.print(" ");
    // Serial.print(add[i]);
    // Serial.println();
    Serial.print("IM HERE");
    Serial.println();
    Serial.println(i);
    Serial.println();
    Serial.println(add[i]);
    temp = add[i];
    
    //Serial.print(temp);
    data[top][i] = temp;
  }
  free(add);
}

void printData(){
  // Serial.println("Input 1");
  // Serial.println(data[0][0]);
  // Serial.println(data[0][1]);
  // Serial.println(data[0][2]);
  // Serial.println(data[0][3]);
  // Serial.println("Input 2");
  // Serial.println(data[1][0]);
  // Serial.println(data[1][1]);
  // Serial.println(data[1][2]);
  // Serial.println(data[1][3]);
  // Serial.println("Input 3");
  // Serial.println(data[2][0]);
  // Serial.println(data[2][1]);
  // Serial.println(data[2][2]);
  // Serial.println(data[2][3]);
  
}

float* getAverage(){
  float avg[eleSize];
for(int i = 0; i < eleSize; i++){
  for(int j = 0; j < numElements; j++){
    avg[i] += data[i][j];
  }
  avg[i] = avg[i]/numElements;
}
return avg;

}

float** getData(){
  // Serial.println("Input 1 FOR ACTUAL DATA");
  // Serial.println(data[0][0]);
  // Serial.println(data[0][1]);
  // Serial.println(data[0][2]);
  // Serial.println(data[0][3]);
  // Serial.println("Input 2");
  // Serial.println(data[1][0]);
  // Serial.println(data[1][1]);
  // Serial.println(data[1][2]);
  // Serial.println(data[1][3]);
  // Serial.println("Input 3");
  // Serial.println(data[2][0]);
  // Serial.println(data[2][1]);
  // Serial.println(data[2][2]);
  // Serial.println(data[2][3]);
    float temp;
    int i = 0;
    int index;
    int counter = 0;
    int j = 0;
    while(counter < numElements){
      counter++;
      index = top - i;
      if(index < 0){
        index = numElements + top - i;
      }
      // Serial.println("Index of og array ");
      // Serial.println(index);
      // Serial.println("Index of new array");
      // Serial.println(i);
      for(j = 0; j < eleSize; j++){
        // Serial.print("Data Index ");
        // Serial.println(index);
        // Serial.print("j ");
        // Serial.println(j);
        temp = data[index][j];
        // Serial.print("Data Getting ADDED ");
        // Serial.print(temp);
        // Serial.println();
        auxData[i][j] = temp;
      }
      i++;
    }
  //   Serial.println("Aux Input 1");
  // Serial.println(auxData[0][0]);
  // Serial.println(auxData[0][1]);
  // Serial.println(auxData[0][2]);
  // Serial.println(auxData[0][3]);
  // Serial.println("Aux Input 2");
  // Serial.println(auxData[1][0]);
  // Serial.println(auxData[1][1]);
  // Serial.println(auxData[1][2]);
  // Serial.println(auxData[1][3]);

  // Serial.println("Input 3");
  // Serial.println(auxData[2][0]);
  // Serial.println(auxData[2][1]);
  // Serial.println(auxData[2][2]);
  // Serial.println(auxData[2][3]);
    return auxData;
}

private:
int top;
float** data;
float** auxData;
int eleSize;
int numElements;
float* (*fptr)();
};

float input1[4] = {1, 2, 3, 4};
float input2[4] = {4, 3, 2, 1};



float* testData(){
  return input1;
}

// float* getValues() {
//   float values[3] = {roll, pitch, heading};
//   return values;
// }


Buffer buff(3,5, getValues);
Buffer encoderBuff1(1,5, getAngleEncoder1);
Buffer encoderBuff2(1,5, getAngleEncoder2);
float ** data;
float* test;
void setup() {
  // put your setup code here, to run once:
  initializeIMU();

  Serial.begin(115200); 
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(50);
  updateIMU();
  test = getValues();

  Serial.println("Input 1");
  delay(5000);
  //Serial.println("Input 2");
  buff.addData();
  Serial.println("Input 3");
  // Serial.println("HELLOOO!");
  //buff.printData();
  data = buff.getData();
  Serial.println("Input 1");
  Serial.println(data[0][0]);
  Serial.println(data[0][1]);
  Serial.println(data[0][2]);
  Serial.println(data[0][3]);
  Serial.println("Input 2");
  Serial.println(data[1][0]);
  Serial.println(data[1][1]);
  Serial.println(data[1][2]);
  Serial.println(data[1][3]);

  Serial.println("Input 3");
  Serial.println(data[2][0]);
  Serial.println(data[2][1]);
  Serial.println(data[2][2]);
  Serial.println(data[2][3]);
}
