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

#include <Arduino.h>
#include "../message_lib/pscom/pscom.h"
#include <ArduinoEigenDense.h>
#include <stdint.h>
#include "settings.h"

/* Define telemetry frequencies */
#define TELEM_HZ 5
#define HEALTH_HZ 1
#define HEARTBEAT_HZ 1

class CommsManager {
public:
    CommsManager(); // Empty Constructor
    void init(); // Initialize; does nothing
    void spin(); // Should be run every loop; automatically pulls 1 byte from serial and sends messages if necessary
    void updateTelem(fmav_control_system_state_t data); // Send telemetry info e.g. position, rotation, thrust
    void updateTrajectoryProgress(fmav_controller_status_t fata); // Send telemetry info e.g. position, rotation, thrust
    void updateHealth(fmav_sys_status_t data); // Send telemetry health info e.g. battery
    void sendStatusText(MAV_SEVERITY severity, const char *text); // Send status text. ONLY USE SPARINGLY (high bandwidth usage).

    // Register callbacks for mission control events
    void registerMissionStartAction(fmav_command_ack_t (*callback) (void));
    void registerMissionNextSegmentAction(fmav_command_ack_t (*callback) (float* K, float* trajStart, uint16_t trajLen));
    void registerMissionPauseAction(fmav_command_ack_t (*callback) (int));
    void registerLandAction(fmav_command_ack_t (*callback) (void));

    // Register callbacks for trajectory manager
    void registerTrajSDLoadAction(fmav_traj_ack_t (*trajLoadSDCallback) (int number));
    // void registerTrajK(void (*callback) (int k, int dim1, int dim2, float *K));
    // void registerTrajPt(void (*callback) (int k, fmav_traj_pt_t pt));
    // void sendTrajAck(fmav_traj_ack_t ack);
private:
    // Highest-level: handle all incoming messages/commands
    void processMessage(fmav_message_t *msg);
    void processCommand(uint8_t sysid, uint8_t compid, fmav_command_long_t *cmd);
    // High-level microservice handlers
    // Parameter Protocol - currently unimplemented (figure out what to do about STL and maps)

    // Mission Protocol
    void handleStartMission();
    void handlePauseMission();
    void handleLandMission();

    // Low-level individual message abstractions
    void sendHeartbeat();              // Send heartbeat message so we know the system is alive
    void sendHealth();
    void sendTelem();
    void sendTrajectoryStatus();
    void sendTrajK1Req(int k);       // Send request for part 1 of a gain matrix K
    void sendTrajK2Req(int k);       // Send request for part 2 of a gain matrix K
    void sendTrajPtReq(int n);       // Send request for trajectory point
    void sendTrajAck();               // Send final message indicating end of mission upload
    void rejectCommand(uint16_t command, const char *reason); // Send ack with command cancelled with an error

    // Timers for periodics
    int lastHeartbeat = 0;
    int lastTelem = 0;
    int lastHealth = 0;

    // Status text ID tracker
    uint16_t statusTextID = 1;

    // Telemetry Data
    fmav_control_system_state_t state;
    fmav_controller_status_t trajectoryStatus;
    fmav_sys_status_t health;

    // Internal Data
    Stream *serial;
    uint8_t sysid = 68;
    uint8_t compid = 1;
    fmav_status_t status;
    fmav_message_t message;

    // Callback handling
    fmav_command_ack_t (*missionStartCallback) (void);
    fmav_command_ack_t (*missionPauseCallback) (int); // pause = 0, continue = 1
    fmav_command_ack_t (*missionLandCallback) (void);
    fmav_traj_ack_t (*trajLoadSDCallback) (int number);
};

#endif
