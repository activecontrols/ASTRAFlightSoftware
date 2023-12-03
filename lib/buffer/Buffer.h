/*
Buffer.h
Description: A class for a 2D buffer of floats
Author: Siddarth Goel, Vincent Palmerio
*/

#ifndef BUFFER_H
#define BUFFER_H

class Buffer{

public:

    Buffer(int elementSize, int numberOfElements, float* (*Fptr)() );

    int getTop(){
        return top;
    }

    void addData();

    void printData();

    float** getData();

private:
    int top;
    float** data;
    float** auxData;
    int eleSize;
    int numElements;
    float* (*fptr)();
};

#endif