#include "Buffer.h"
#include "../../src/debug.h"
#if defined(ASTRA_BUFFER_DEBUG) or defined(ASTRA_FULL_DEBUG)
    #include <Arduino.h>
#endif

Buffer::Buffer(int elementSize, int numberOfElements, float* (*Fptr)() ){
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

void Buffer::addData(){
    float* add = fptr();
    float temp = 0;
    top++;
    if(top >= numElements){
        top = 0;
    }
#if defined(ASTRA_BUFFER_DEBUG) or defined(ASTRA_FULL_DEBUG)
    Serial.println("Adding element to top = ");
    Serial.print(top);
#endif
    
    for(int i = 0; i < eleSize; i++){

        temp = add[i];
        data[top][i] = temp;

#if defined(ASTRA_BUFFER_DEBUG) or defined(ASTRA_FULL_DEBUG)
        Serial.println("Adding element ");
        Serial.print(i);
        Serial.print(" ");
        Serial.print(add[i]);
        Serial.println();
        Serial.println(temp);
#endif
       
    }
}

void printData(){

#if defined(ASTRA_BUFFER_DEBUG) or defined(ASTRA_FULL_DEBUG)
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
#endif
    
}

float** Buffer::getData(){
#if defined(ASTRA_BUFFER_DEBUG) or defined(ASTRA_FULL_DEBUG)
    Serial.println("Input 1 FOR ACTUAL DATA");
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
#endif

    float temp;
    int i = 0;
    int index;
    int counter = 0;
    int j = 0;

    while (counter < numElements) {
        counter++;
        index = top - i;
        if (index < 0) {
            index = numElements + top - i;
        }

#if defined(ASTRA_BUFFER_DEBUG) or defined(ASTRA_FULL_DEBUG)
        Serial.println("Index of og array ");
        Serial.println(index);
        Serial.println("Index of new array");
        Serial.println(i);
#endif

        for (j = 0; j < eleSize; j++) {

            temp = data[index][j];
            auxData[i][j] = temp;

#if defined(ASTRA_BUFFER_DEBUG) or defined(ASTRA_FULL_DEBUG)
            Serial.print("Data Index ");
            Serial.println(index);
            Serial.print("j ");
            Serial.println(j);
            
            Serial.print("Data Getting ADDED ");
            Serial.print(temp);
            Serial.println();
#endif
            
        }

        i++;
    }

#if defined(ASTRA_BUFFER_DEBUG) or defined(ASTRA_FULL_DEBUG) 
    Serial.println("Aux Input 1");
    Serial.println(auxData[0][0]);
    Serial.println(auxData[0][1]);
    Serial.println(auxData[0][2]);
    Serial.println(auxData[0][3]);
    Serial.println("Aux Input 2");
    Serial.println(auxData[1][0]);
    Serial.println(auxData[1][1]);
    Serial.println(auxData[1][2]);
    Serial.println(auxData[1][3]);

    Serial.println("Input 3");
    Serial.println(auxData[2][0]);
    Serial.println(auxData[2][1]);
    Serial.println(auxData[2][2]);
    Serial.println(auxData[2][3]);
#endif

    return auxData;
}