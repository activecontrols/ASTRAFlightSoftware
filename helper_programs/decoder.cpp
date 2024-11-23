/*
 * Decoder.cpp
 *
 * Contains function that decodes binary data from SD card file into structs
 *
 * Created: 11/9/24 by Jack Fang <fang355@purdue.edu>
 * Last updated: 11/23/2024 by Jack Fang <fang355@purdue.edu>
 *
 */

#define IS_ARDUINO 0

#include <iostream>
#include <fstream>
#include <map>

#include "../lib/data/Structs.h"
#include "../lib/data/Structs.cpp" // hack for now.

// #include "Structs.h"
// #include "Structs.cpp"

using namespace std;

map<int, ofstream*> fileMap;
ofstream* createCSVFile(DataType e, const char* fileName);
void closeAllCSVFiles();

int decode(ifstream &sdFile) {
    DataType currentDataType = (DataType) -1;

    if (!sdFile.read((char *) &currentDataType, sizeof(DataType))) {
        return 1;
    }
    
    map<int, ofstream*>::iterator it; // used for searching in fileMap

    switch (currentDataType) {
        case STRING:
            int str_size;
            sdFile.read((char *) &str_size, sizeof(str_size)); // read in string size
            char* str;
            sdFile.read(str, str_size); // read in string

            it = fileMap.find(EXAMPLE1);
            if (it == fileMap.end()) {
                ofstream& outFile = *createCSVFile(STRING, "strings.csv");
                outFile << "String" << endl;
                outFile << str << endl;
            } else {
                ofstream& outFile = *it->second;
                outFile << str << endl;
            }

            break;
        case EXAMPLE1:
            Example1 some1;
            some1.binaryDecode(sdFile);

            it = fileMap.find(EXAMPLE1);
            if (it == fileMap.end()) {
                ofstream& outFile = *createCSVFile(EXAMPLE1, "example1.csv");
                outFile << some1.CSVHeader() << endl;
                outFile << some1.printCSV() << endl;
            } else {
                ofstream& outFile = *it->second;
                outFile << some1.printCSV() << endl;
            }

            break;
        case EXAMPLE2:
            Example2 some2;
            some2.binaryDecode(sdFile);

            it = fileMap.find(EXAMPLE2);
            if (it == fileMap.end()) {
                ofstream& outFile = *createCSVFile(EXAMPLE2, "example2.csv");
                outFile << some2.CSVHeader() << endl;
                outFile << some2.printCSV() << endl;
            } else {
                ofstream& outFile = *it->second;
                outFile << some2.printCSV() << endl;
            }

            break;
        default:
            cerr << "No matching struct enum has been found for enum value ";
            cerr << ((uint8_t) currentDataType) << endl;
            return 1;
    }

    return 0;
}

/**
 * Opens file for writing and returns a pointer to it
 */
ofstream* createCSVFile(DataType e, const char* fileName) {
    ofstream* outFile = new ofstream();
    outFile->open(fileName);

    if (!outFile->is_open()) {
        delete outFile;
        return nullptr;
    }

    fileMap[e] = outFile;
    return outFile;
}

void closeAllCSVFiles() {
    for(auto& pair : fileMap) {
        if (pair.second != nullptr) {
            pair.second->close();
            delete pair.second;
        }
    }
    fileMap.clear();
}

int main() {
    const char *sdFileName = "./SD_Files/FILE1.OUT";

    // Open the SD file in binary mode
    ifstream sdFile(sdFileName, ios::binary);

    // Check if the file was opened successfully
    if (!sdFile) {
        cerr << "SD file could not be opened for reading!" << endl;
        return 1;
    }

    int numDecoded = 0;
    int maxDecodings = 1000; // can be set
    while (decode(sdFile) == 0 && numDecoded < maxDecodings) {
        numDecoded++;
    }
    cout << "Decoded ";
    cout << numDecoded;
    cout << ((numDecoded == 1) ? " struct" : " structs") << endl;

    sdFile.close();
    closeAllCSVFiles();

    return 0;
}