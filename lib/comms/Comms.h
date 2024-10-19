/*
Comms.h - Comms Header file
Description: Header file to Comms.cpp. Wraps libmavskipper code to be more class-like the way other modules may expect it.
Author: Vincent Wang, Teresa Wan
Created: 2023-10-27
*/

#ifndef COMMS_H 
#define COMMS_H 

#define FASTMAVLINK_SERIAL_WRITE_CHAR 1
#define FASTMAVLINK_IGNORE_WADDRESSOFPACKEDMEMBER
void fmav_serial_write_char(char c);

#include <cstring>
#include "../message_lib/pscom/pscom.h"
#include "FlightModule.h"
#include "Router.h"
#include <ArduinoEigenDense.h>
#include <stdint.h>

/* Define telemetry frequencies */
#define TELEM_HZ 5
#define HEALTH_HZ 1
#define HEARTBEAT_HZ 1

namespace flightData {
    extern float voltage[];
    extern Eigen::VectorXd measurementVectorY;
    extern Eigen::VectorXd estimatedStateX;
    extern Router *router;
};

class CommsManager : public FlightModule {
public:
    CommsManager(); // Empty Constructor
    int init() override; // Initialize; does nothing
    void update(unsigned long time); // Should be run every loop; automatically pulls 1 byte from serial and sends messages if necessary
    void sendStatusText(MAV_SEVERITY severity, const char *text); // Send status text. ONLY USE SPARINGLY (high bandwidth usage).

private:
    // Highest-level: handle all incoming messages/commands
    void processMessage(fmav_message_t *msg);
    void processCommand(uint8_t sysid, uint8_t compid, fmav_command_long_t *cmd);
    // High-level microservice handlers
    // Parameter Protocol - currently unimplemented (figure out what to do about STL and maps)

    // Low-level individual message abstractions
    void sendHeartbeat();              // Send heartbeat message so we know the system is alive
    void sendHealth();            
    void sendTelem(unsigned long time);              

    void ackCommand(uint16_t command, uint8_t code, uint8_t sysid, uint8_t compid);
    void rejectCommand(uint16_t command, const char *reason); // Send ack with command cancelled with an error
    
    // Timers for periodics
    int lastHeartbeat = 0;
    int lastTelem = 0;
    int lastHealth = 0;

    // Status text ID tracker
    uint16_t statusTextID = 1;

    // Internal Data
    Stream *serial;
    uint8_t sysid = 68;
    uint8_t compid = 1;
    fmav_status_t status;
    fmav_message_t message;
};

#endif
