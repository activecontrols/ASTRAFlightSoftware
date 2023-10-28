/*
Comms.h - Comms Header file
Description: Header file to Comms.cpp. Wraps libmavskipper code to be more class-like the way other modules may expect it.
Author: Vincent Wang, Teresa Wan
Created: 2023-10-27
*/

#ifndef COMMS_H 
#define COMMS_H 

#include "../libmavskipper/include/mavskipper/process_mavlink.h"
#include "../message_lib/pscom/pscom.h"
#include <ArduinoEigenDense.h>

#define U_ARRAY_LENGTH 12
#define MODE_ARRAY_LENGTH 12
#define K_ARRAY_LENGTH 10

#define TELEM_HZ 5
#define HEALTH_HZ 1

class CommsManager {
public:
    CommsManager(); // Empty Constructor
    void init(Stream *serial); // Initialize
    void spin(); // Should be run every loop; automatically pulls 1 byte from serial and sends messages if necessary
    void updateTelem(fmav_control_system_state_t data); // Send telemetry info e.g. position, rotation, thrust
    void updateTrajectoryProgress(fmav_controller_status_t fata); // Send telemetry info e.g. position, rotation, thrust
    void updateHealth(fmav_sys_status_t data); // Send telemetry health info e.g. battery

    // Register callbacks for mission control events
    void registerMissionStartAction(void (*callback) (void));
    void registerMissionNextSegmentAction(void (*callback) (float* K, float* trajStart, uint16_t trajLen));
    void registerLandAction(void (*callback) (void));

    // Manage
private:
    // Highest-level: handle all incoming messages/commands
    void processMessage(fmav_message_t *msg);
    // High-level microservice handlers
    // Parameter Protocol - currently unimplemented (figure out what to do about STL and maps)

    // Mission Protocol
    void handleStartMission();
    void handleStartMission();
    void handleStartMission();

    // Low-level individual message abstractions
    void sendHeartbeat();              // Send heartbeat message so we know the system is alive
    void sendTrajK1Req(int k);       // Send request for part 1 of a gain matrix K
    void sendTrajK2Req(int k);       // Send request for part 2 of a gain matrix K
    void sendTrajPtReq(int k, int n);       // Send request for trajectory point
    void sendTrajAck();               // Send final message indicating end of mission upload

    // Data
    Stream *serial;
};

#endif
