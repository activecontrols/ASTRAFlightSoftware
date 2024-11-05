/*
 * Logger.cpp
 *
 * Contains functions that declare loggers and log messages to serial and SD
 *
 * Created: 10/24/24 by Jack Fang <fang355@purdue.edu>
 * Last updated: 11/5/2024 by Jack Fang <fang355@purdue.edu>
 *
 */

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

const char * toConstChar(string data) {
    return data.c_str();
}
const char * toConstChar(char *data) {
    return data;
}
const char * toConstChar(const char *data) {
    return data;
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
 * Sublogger passes message to central logger iff message priority >= thresholdToPass
 */
Logger::Logger(string prefix, Logger *parent, LogLevel thresholdToPass) {
    this->prefix = prefix;
    this->parent = parent;
    this->thresholdToPass = thresholdToPass;
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