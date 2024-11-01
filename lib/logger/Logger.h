#ifndef LOGGER_H
#define LOGGER_H

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

template <typename T>
string toString(const T& data);
string toString(string data);
char *toString(char *data);
const char *toString(const char *data);

class Logger {
public:
    Logger(LogLevel writeToSDLevel = INFO, LogLevel writeToConsoleLevel = NEVER);
    Logger(string prefix, Logger *parent, LogLevel thresholdToLog = WARNING);

    template <typename T>
    void addMessage(LogLevel logLevel, const T& data);

    template <typename T>
    void writeToConsole(LogLevel logLevel, string prefix, const T& data);

    template <typename T>
    void writeToSerial(LogLevel logLevel, string prefix, const T& data);

    template <typename T>
    void writeToSD(const T& data);
    void writeToSD(string data);
    void writeToSD(char *data);
    void writeToSD(const char *data);
    

    // central logger fields
    LogLevel writeToSDLevel;
    LogLevel writeToConsoleLevel;

    // sub-logger fields
    string prefix;
    Logger *parent;
    LogLevel thresholdToLog;
};

// example struct for testing purposes
struct Person {
  int age;
  string name;
  
  string print() const {
    return name + " is " + to_string(age) + " years old.";
  }

  void binaryPrint() const {
    cout << "binary printing a person struct" << endl;
  }
};

#endif // LOGGER_H