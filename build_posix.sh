#!/bin/bash

g++ \
-std=c++17 \
-I lib/error/ \
-I lib/math/ \
-I lib/flightmodule \
-I lib/IMU/include/ \
-I lib/IMU/src/ \
lib/flightmodule/Scheduler.cpp \
lib/flightmodule/Router.cpp \
lib/IMU/src/FakeIMU.cpp \
src/main_posix.cpp
