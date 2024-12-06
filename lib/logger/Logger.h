/*
  Logger.h - Logger Header file
  Description: Header file to Logger.cpp
  Created: 10/24/2024 by Jack Fang <fang355@purdue.edu>
  Last updated: 11/5/2024 by Jack Fang <fang355@purdue.edu>
*/

#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <iostream>
#include <string>

using namespace std;

enum LogLevel {
	DEBUG,
	INFO,
	WARNING,
	ERROR,
	FATAL,
	NEVER
};

string logLevelString(LogLevel logLevel);

/**
 * Gives the string representation of the given data using function overloading
 */
template <typename T>
typename enable_if<!is_same<T, string>::value &&
    !is_same<T, const char*>::value &&
    !is_same<T, char*>::value, const char*>::type
toConstChar(const T& data) {
    return data.printHuman().c_str();
}
const char * toConstChar(string data);
const char * toConstChar(char *data);
const char * toConstChar(const char *data);

class Logger {
public:
    // central logger fields
    LogLevel writeToSDLevel;
    LogLevel writeToConsoleLevel;

    // sub-logger fields
    string prefix;
    Logger *parent;
    LogLevel thresholdToPass;

    Logger(LogLevel writeToSDLevel = INFO, LogLevel writeToConsoleLevel = NEVER);
    Logger(string prefix, Logger *parent, LogLevel thresholdToPass = WARNING);
    

	/**
	 * NOTE: Template functions must be defined in the header file because
	 * types must be visible at compile time
	 */

	/**
	 * Logic to handle what happens when a message is added to be potentially logged
	 * the 'data' parameter will be a struct that has a 'print' function
	 * where the 'print' function will return a string representation of the struct
	 * 
	 * Refer to logger constructor comments to understand logic
	 */
    template <typename T>
	void addMessage(LogLevel logLevel, const T& data) {
		if (parent == NULL) {
			if (logLevel >= writeToConsoleLevel) {
				// the following message will be written to serial
				writeToSerial(logLevel, prefix, data);
			}
			if (logLevel >= writeToSDLevel) {
				// the following message will be written to SD card
				writeToSD(data);
			}
		} else if (logLevel >= this->thresholdToPass) {
			parent->addMessage(logLevel, data);
		} else {
			Serial.println("Message not passed to central logger");
		}
	}

    /**
	 * The central logger will write the data to serial
	 * 
	 * The associated prefix and log level for the data/message are also given
	 * as arguments to this function
	 */
	template <typename T>
	void writeToSerial(LogLevel logLevel, string prefix, const T& data) {
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
	typename enable_if<!is_same<T, string>::value &&
        !is_same<T, const char*>::value &&
    	!is_same<T, char*>::value, void>::type
	writeToSD(const T& data) {
		data.binaryPrint();
	}
	void writeToSD(string data);
	void writeToSD(char *data);
	void writeToSD(const char *data);
};

#endif // LOGGER_H