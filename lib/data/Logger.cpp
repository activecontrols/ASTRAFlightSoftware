/*
 * Logger.cpp
 *
 * Contains functions that declare loggers and log messages to serial and SD
 *
 * Created: 10/24/24 by Jack Fang <fang355@purdue.edu>
 * Last updated: 11/16/2024 by Jack Fang <fang355@purdue.edu>
 *
 */

#include <Arduino.h>
#include <SD.h>

#include "Logger.h"
#include "Structs.h"

/**
 * Function to convert log level to string representation
 */
const char * logLevelString(LogLevel logLevel) {
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

    strcpy(this->prefix, "CENTRAL");
    (this->prefix)[MAX_PREFIX_LENGTH - 1] = '\0';

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
Logger::Logger(const char * prefix, Logger *parent, LogLevel thresholdToPass) {
    strncpy(this->prefix, prefix, MAX_PREFIX_LENGTH);
    (this->prefix)[MAX_PREFIX_LENGTH - 1] = '\0';
    this->parent = parent;
    this->thresholdToPass = thresholdToPass;
}

void Logger::writeToSD(char *data, File outFile) {
    // encode unique id
    DataType e = STRING;
    outFile.write((const uint8_t *) &e, sizeof(e));

    // encode size of string, including null terminator
    int str_size = strlen(data) + 1;
    outFile.write((const uint8_t *) &str_size,  sizeof(str_size));
    // encode string
    outFile.write((const uint8_t *) data, str_size);
}

void Logger::writeToSD(const char *data, File outFile) {
    // encode unique id
    DataType e = STRING;
    outFile.write((const uint8_t *) &e, sizeof(e));
    
    // encode size of string, including null terminator
    int str_size = strlen(data) + 1;
    outFile.write((const uint8_t *) &str_size,  sizeof(str_size));
    // encode string
    outFile.write((const uint8_t *) data, str_size);
}