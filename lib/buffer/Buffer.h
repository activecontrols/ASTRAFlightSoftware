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