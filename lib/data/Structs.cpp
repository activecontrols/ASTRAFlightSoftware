/*
 * Structs.cpp
 *
 * Contains definitions of functions for structs declared in Structs.h
 *
 * Created: 11/9/24 by Ishan Goel <<goeli@purdue.edu>
 * Last updated: 11/23/2024 by Jack Fang <fang355@purdue.edu>
 *
 */
#include "Structs.h"
#if IS_ARDUINO
#include "CString.h"
#else
#include <fstream>
#endif

namespace {
    static char buffer[MAX_STRING_SIZE];  // for returning strings by structs
}

#if IS_ARDUINO
void Example1::binaryPrint(File outFile) const { // File from SD.h
    DataType e = EXAMPLE1;  // first, encode unique ID
    outFile.write((const uint8_t *) &e, sizeof(e));
    outFile.write((const uint8_t *) this, sizeof(*this));
    // decoder knows how many bytes to read in since we encoded the unique ID.
}
const char* Example1::printHuman() const { // human readable
    CString<100> humanString;
    humanString << data << " " << data2 << " " << str;

    strncpy(buffer, humanString.str, MAX_STRING_SIZE - 1);
    buffer[MAX_STRING_SIZE - 1] = '\0';
    return buffer;
}
#else
void Example1::binaryDecode(std::ifstream &inFile) {
    inFile.read((char *) this, sizeof(*this));
}
std::string Example1::CSVHeader() const { // get csv header 
    return "Data1,Data2,Data3";
}
std::string Example1::printCSV() const {  // get as csv
    std::string csvString;
    csvString.append(std::to_string(data));
    csvString.append(",");
    csvString.append(std::to_string(data2));
    csvString.append(",");
    csvString.append(str);
    return csvString;
}
#endif // IS_ARDUINO


#if IS_ARDUINO
void Example2::binaryPrint(File outFile) const { // File from SD.h
    // encode unique id
    DataType e = EXAMPLE2;
    outFile.write((const uint8_t *) &e, sizeof(e));

    // encode float
    outFile.write((const uint8_t *) &a, sizeof(a));

    // encode size of string, including null terminator
    int str_size = strlen(str) + 1;
    outFile.write((const uint8_t *) &str_size,  sizeof(str_size));
    // encode string
    outFile.write((const uint8_t *) str, str_size);
}
const char * Example2::printHuman() const { // human readable
    CString<100> humanString;
    humanString << a << " " << str;
    humanString.print();

    strncpy(buffer, humanString.str, MAX_STRING_SIZE - 1);
    buffer[MAX_STRING_SIZE - 1] = '\0';
    return buffer;
}
#else
void Example2::binaryDecode(std::ifstream &inFile) {
    inFile.read((char *) &a, sizeof(a)); // id has been read, float is next

    int str_size;
    inFile.read((char *) &str_size, sizeof(str_size)); // read in string size

    inFile.read(str, str_size); // read in string
}
std::string Example2::CSVHeader() const { // get csv header 
    return "Data1,Data2";
}
std::string Example2::printCSV() const {  // get as csv
    std::string csvString;
    csvString.append(std::to_string(a));
    csvString.append(",");
    csvString.append(str);
    return csvString;
}
#endif // IS_ARDUINO