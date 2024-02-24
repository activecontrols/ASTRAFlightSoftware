#include <stdio.h>
#include <stdint.h>

#define ESTIMATED_STATE_DIMENSION 6 
#define MEASUREMENT_DIMENSION 9      
#define U_ROW_LENGTH 4         

typedef struct {
    float t;                               // time
    float battVoltage;                     // battery voltage
    float x[ESTIMATED_STATE_DIMENSION];    // state
    float y[MEASUREMENT_DIMENSION];        // measurement
    float u[U_ROW_LENGTH];                 // control
} Data;

int main() {
    //FILE *file = fopen("/Volumes/Untitled/log.bin", "rb");
    FILE *file = fopen("./log.bin", "rb");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Determine the size of the file
    fseek(file, 0, SEEK_END);
    long amount = ftell(file)/sizeof(Data);
    rewind(file);

    printf("Amount: %ld", amount);

    for (int i = 0; i < amount; i++) {
        // Read the binary data into a buffer
        uint8_t buffer[sizeof(Data)];
        //Data data;
        fread(buffer, sizeof(uint8_t), sizeof(Data), file);
        

        // Cast the buffer to the structure type
        Data *data_ptr = (Data *)buffer;

        // Access the members of the structure
        printf("Time: %f\n", data_ptr->t);
        printf("Battery Voltage: %f\n", data_ptr->battVoltage);
        // Accessing array members
        for (int i = 0; i < ESTIMATED_STATE_DIMENSION; i++) {
            printf("x[%d]: %f\n", i, data_ptr->x[i]);
        }
        for (int i = 0; i < MEASUREMENT_DIMENSION; i++) {
            printf("y[%d]: %f\n", i, data_ptr->y[i]);
        }
        for (int i = 0; i < U_ROW_LENGTH; i++) {
            printf("u[%d]: %f\n", i, data_ptr->u[i]);
        }

        // // Access the members of the structure
        // printf("Time: %f\n", data.t);
        // printf("Battery Voltage: %f\n", data.battVoltage);
        // // Accessing array members
        // for (int i = 0; i < ESTIMATED_STATE_DIMENSION; i++) {
        //     printf("x[%d]: %f\n", i, data.x[i]);
        // }
        // for (int i = 0; i < MEASUREMENT_DIMENSION; i++) {
        //     printf("y[%d]: %f\n", i, data.y[i]);
        // }
        // for (int i = 0; i < U_ROW_LENGTH; i++) {
        //     printf("u[%d]: %f\n", i, data.u[i]);
        // }
    }

    fclose(file);

    return 0;
}
