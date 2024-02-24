//
// Created by Ishan Goel on 2/23/24.
//

#ifndef LOG_H
#define LOG_H

#include "Estimator.h"
#include "Controller.h"

namespace logger {
    typedef struct {
        float t; // time
        float battVoltage; // battery voltage
        float x[ESTIMATED_STATE_DIMENSION]; // state
        float y[MEASUREMENT_DIMENSION]; // measurement
        float u[U_ROW_LENGTH]; // control
    } Data;

    extern int FILE_WRITE_ERR, FILE_OPEN_ERR;

    extern int write_count;
    int open(const char* filename);
    void write(Data* data);
    void close();
}

void initializeSD();

#endif //LOG_H
