//
// Created by Ishan Goel on 2/23/24.
//

#include "log.h"

#include <SD.h>

namespace logger {
    int FILE_WRITE_ERR = -2, FILE_OPEN_ERR = -1;
    int write_count = 0;

    File file;
}


void logger::close() {
    file.flush();
    file.close();
}

void logger::write(Data* data) {
    // open file
    // File file = SD.open(filename, FILE_WRITE);
    // if (!file) return FILE_WRITE_ERR;
    // write data
    file.write((uint8_t*) data, sizeof(*data)); // TODO: maybe check if all bytes were written but even if they weren't there's nothing we can do about it
    write_count++;
    if (write_count % 100 == 0) { // flush every 100 writes
        file.flush();
    }
    // file.close();
}

int logger::open(const char* filename) {

    Serial.print("Initializing SD card...");

    if (!SD.begin(BUILTIN_SDCARD)) {
        Serial.println("initialization failed!");
        while (true);
    }
    Serial.println("initialization done.");

    file = SD.open(filename, FILE_WRITE | O_TRUNC);
    if (!file) return FILE_OPEN_ERR;
    return 0;
}