#include "Logger.h"
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
            // the following message will be written to console
            parent->writeToConsole(logLevel, this->prefix, data);
        }
        if (logLevel >= parent->writeToSDLevel) {
            // the following message will be written to SD card
            // TBD
            parent->writeToSD(data);
        }
    } else {
        cout << "Message not passed to central logger" << endl;
    }
}

/**
 * Gives the string representation of the given data using function overloading
 */
template<typename T>
string toString(const T& data) {
    return data.print();
}
string toString(string data) {
    return data;
}
char *toString(char *data) {
    return data;
}
const char *toString(const char *data) {
    return data;
}

/**
 * The central logger will write the data to console
 * 
 * The associated prefix and log level for the data/message are also given
 * as arguments to this function
 */
template <typename T>
void Logger::writeToConsole(LogLevel logLevel, string prefix, const T& data) {
    cout << "CONSOLE" << endl;
    cout << "Log level: " << logLevelString(logLevel) << endl;
    cout << "Prefix: " << prefix << endl;
    cout << "Data: " << toString(data) << endl;
}

/**
 * The central logger will write the data to serial
 * 
 * The associated prefix and log level for the data/message are also given
 * as arguments to this function
 */
template <typename T>
void Logger::writeToSerial(LogLevel logLevel, string prefix, const T& data) {
    Serial.println("test");
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
    cout << "TBD SD Card" << endl;
}
void Logger::writeToSD(char *data) {
    // TBD
    cout << "TBD SD Card" << endl;
}
void Logger::writeToSD(const char *data) {
    // TBD
    cout << "TBD SD Card" << endl;
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
    cout << endl;

    imuLogger.addMessage(FATAL, p2);
    cout << endl;

    imuLogger.addMessage(ERROR, "Hello World");
    cout << endl;

    string emerg = "Emergency";
    imuLogger.addMessage(FATAL, "emergency");
    cout << endl;

    return 0;
}