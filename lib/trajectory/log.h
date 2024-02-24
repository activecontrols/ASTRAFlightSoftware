//
// Created by Ishan Goel on 2/23/24.
//

#ifndef LOG_H
#define LOG_H

namespace logger {
    typedef struct {
        float t; // time
        float battVoltage; // battery voltage
        float x[6]; // state
        float y[9]; // measurement
        float u[4]; // control
    } Data;

    extern int FILE_WRITE_ERR, FILE_OPEN_ERR;

    extern int write_count;
    int open(const char* filename);
    void write(Data* data);
    void close();
}

void initializeSD();

#endif //LOG_H
