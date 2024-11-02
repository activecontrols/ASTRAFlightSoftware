#include "Logger.h"
#include <Arduino.h>
#include <iostream>
#include <string>

using namespace std;

/**
 * Function to convert log level to string representation
 */
string logLevelString(LogLevel logLevel) {
  switch (logLevel) {
    case DEBUG:
      return "DEBUG";
    case INFO:
      return "INFO";
    case WARNING:
      return "WARNING";
    case ERROR:
      return "ERROR";
    case FATAL:
      return "FATAL";
    case NEVER:
      return "NEVER";
    default:
      return "UNKNOWN";
  }
}

/**
 * Constructor for central logger
 * 
 * Logger writes to SD card iff message priority >= writeToSDLevel
 * Logger writes to console iff message priority >= writeToConsoleLevel
 * Refer to .h file for enum definition of LogLevel
 */
Logger::Logger(LogLevel writeToSDLevel, LogLevel writeToConsoleLevel) {
    this->writeToSDLevel = writeToSDLevel;
    this->writeToConsoleLevel = writeToConsoleLevel;

    this->prefix = "CENTRAL";
    this->parent = NULL;
}

/**
 * Constructor for sublogger
 * 
 * Each module will have its own sublogger
 * prefix: the prefix associated with this sublogger
 * parent: the sublogger can pass data/messages to the central logger
 * Sublogger passes message to central logger iff message priority >= thresholdToLog
 */
Logger::Logger(string prefix, Logger *parent, LogLevel thresholdToLog) {
    this->prefix = prefix;
    this->parent = parent;
    this->thresholdToLog = thresholdToLog;
}

/**
 * Logic to handle what happens when a message is added to be potentially logged
 * the 'data' parameter will be a struct that has a 'print' function
 * where the 'print' function will return a string representation of the struct
 * 
 * Refer to logger constructor comments to understand logic
 */
template <typename T>
void Logger::addMessage(LogLevel logLevel, const T& data) {
    if (parent == NULL || logLevel >= this->thresholdToLog) {
        // pass to parent logger, or use the current logger if no parent
        Logger *parent = this->parent;
        if (parent == NULL) {
            parent = this;
        }
        if (logLevel >= parent->writeToConsoleLevel) {
            // the following message will be written to serial
            // parent->writeToConsole(logLevel, this->prefix, data);
            parent->writeToSerial(logLevel, this->prefix, data);
        }
        if (logLevel >= parent->writeToSDLevel) {
            // the following message will be written to SD card
            parent->writeToSD(data);
        }
    } else {
        Serial.println("Message not passed to central logger");
    }
}

/**
 * Gives the string representation of the given data using function overloading
 */
template<typename T>
const char * toConstChar(const T& data) {
    return data.print().c_str();
}
const char * toConstChar(string data) {
    return data.c_str();
}
const char * toConstChar(char *data) {
    return data;
}
const char * toConstChar(const char *data) {
    return data;
}

// /**
//  * The central logger will write the data to console
//  * 
//  * The associated prefix and log level for the data/message are also given
//  * as arguments to this function
//  */
// template <typename T>
// void Logger::writeToConsole(LogLevel logLevel, string prefix, const T& data) {
//     cout << "CONSOLE" << endl;
//     cout << "Log level: " << logLevelString(logLevel) << endl;
//     cout << "Prefix: " << prefix << endl;
//     cout << "Data: " << toString(data) << endl;
// }

/**
 * The central logger will write the data to serial
 * 
 * The associated prefix and log level for the data/message are also given
 * as arguments to this function
 */
template <typename T>
void Logger::writeToSerial(LogLevel logLevel, string prefix, const T& data) {
    Serial.print("Log Level: ");
    Serial.println(logLevelString(logLevel).c_str());
    Serial.print("Prefix: ");
    Serial.println(prefix.c_str());
    Serial.print("Data: ");
    Serial.println(toConstChar(data));
}

/**
 * The central logger will write the data to the SD card
 * 
 */
template <typename T>
void Logger::writeToSD(const T& data) {
    data.binaryPrint();
}
void Logger::writeToSD(string data) {
    // TBD
    Serial.println("TBD write to SD Card");
}
void Logger::writeToSD(char *data) {
    // TBD
    Serial.println("TBD write to SD Card");
}
void Logger::writeToSD(const char *data) {
    // TBD
    Serial.println("TBD write to SD Card");
}

// for testing purposes
int main() {
    Logger centralLogger(INFO, WARNING);
    Logger imuLogger("IMU", &centralLogger, ERROR); // sublogger

    // for testing purposes
    Person p1;
    p1.age = 10;
    p1.name = "Alice";

    Person p2;
    p2.age = 25;
    p2.name = "Bob";

    centralLogger.addMessage(ERROR, p1);
    Serial.println();

    imuLogger.addMessage(FATAL, p2);
    Serial.println();

    imuLogger.addMessage(ERROR, "Hello World");
    Serial.println();

    string emerg = "Emergency";
    imuLogger.addMessage(FATAL, "emergency");
    Serial.println();

    return 0;
}