

class Buffer{

public:
Buffer(int elementSize, int numberOfElements){
    eleSize = elementSize;
    numElements = numberOfElements;
    data = new int*[numberOfElements];
    for(int i = 0 ; i < numberOfElements; i++){
        data[i] = new int[eleSize];
    }
    top = 0;
}

void addData(int* addition){
    top++;
    if(top >= numElements){
        top = 0;
    }
    data[top] = addition;
}

int** getData(){
    int** auxData = data;
    int i = top;
    int j = 0;
    do{
        auxData[j] = data[i];
        i++;
        j++;
        if(i >= numElements){
            i = 0;
        }
    }while(j != numElements);

    return auxData;
}

private:
int top;
int** data;
int eleSize;
int numElements;
};