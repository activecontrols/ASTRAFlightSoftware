/*
settings.h
Description: Contains preprocessor variables to turn certain features on and off
Author: Vincent Palmerio
*/

#define USE_ENCODER (false)
#define USE_TOF_SENSOR (false)
#define USE_COMMS (true)
#define LOG_DATA (false)
#define REGULATE_ONLY (true)
#define DO_CONTROLS (false)

/* Define serial interface to use at compile time */
#define MAVLinkSerial (Serial2)
