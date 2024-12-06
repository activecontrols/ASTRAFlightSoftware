/*
 * Structs.h - Structs Header file
 *
 * Contains declarations of all structs to be used with logger
 * Also contains definition of DataType enum for each struct
 *
 * Created: 11/9/24 by Ishan Goel <<goeli@purdue.edu>
 * Last updated: 11/23/2024 by Jack Fang <fang355@purdue.edu>
 *
 */

#ifndef STRUCTS_H
#define STRUCTS_H

#if IS_ARDUINO
#include <Arduino.h>
#include <SD.h>
#else
#include <fstream>
#include <iostream>
#endif

#define MAX_STRING_SIZE 1024

// so that we have a unique ID for each data type, to help with decoding
enum DataType : uint8_t {
    STRING,
    EXAMPLE1,
    EXAMPLE2,
};

struct Example1 {
    int data;
    float data2;
    char str[5];

#if IS_ARDUINO
    void binaryPrint(File outFile) const;
    const char* printHuman() const; // human readable
#else
    void binaryDecode(std::ifstream &inFile);
    std::string CSVHeader() const; // get csv header
    std::string printCSV() const;  // get as csv
#endif // IS_ARDUINO
};

struct Example2 {
    float a;
    char *str; // str points to data that should be included

#if IS_ARDUINO
    void binaryPrint(File outFile) const;
    const char* printHuman() const; // human readable
#else
    void binaryDecode(std::ifstream &inFile);
    std::string CSVHeader() const; // get csv header
    std::string printCSV() const;  // get as csv
#endif // IS_ARDUINO
};


#endif // STRUCTS_H

