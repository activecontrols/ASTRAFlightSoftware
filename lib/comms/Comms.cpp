#include "Comms.h"
//#include "../estimator/Estimator.h"
//#include <stdlib.h>
#include "../error/Error.h"
#include <cstdint>

/* ----- PRIVATE HELPER FORWARD-DECLS ----- */

/* We want to define a serial helper for our fastmavlink
 * library so it can more efficiently write messages to serial
 * without requiring intermediate buffers
 */
#define FASTMAVLINK_SERIAL_WRITE_CHAR
void fmav_serial_write_char(char c) {
    MAVLinkSerial.write(c);
}

/* ----- PUBLIC INTERFACES ----- */

/**
 * We leave the constructor empty for now; most initialization
 * functions should be handled in init() in the style of most Arduino
 * libraries.
 */
CommsManager::CommsManager() { }

/**
 * Init function; currently doesn't need to do anything
 */
void CommsManager::init(Stream *serial) { }

/**
 * Spin function; this should be run every loop. Processes
 * new data from serial input and sends periodic messages
 */
void CommsManager::spin() {
    uint16_t available = MAVLinkSerial.available();
    for (uint16_t i = 0; i < available; i++) {
        char c = MAVLinkSerial.read();
        uint8_t res = fmav_parse_to_msg(&(this->message), &(this->status), c);
        if (res == FASTMAVLINK_PARSE_RESULT_OK) {
            this->processMessage(&(this->message));
        }
    }

    if (millis() - this->lastHeartbeat > (1 / HEARTBEAT_HZ)) {
        this->sendHeartbeat();
        this->lastHeartbeat = millis();
    }

    if (millis() - lastTelem > (1 / TELEM_HZ)) {
        this->sendTelem();
        this->lastTelem = millis();
    }

    if (millis() - lastHealth > (1 / HEALTH_HZ)) {
        this->sendHealth();
        this->lastHealth = millis();
    }
}

/**
 * Update telemetry information to be sent periodically to the
 * ground control station.
 */
void CommsManager::updateTelem(fmav_control_system_state_t data) {
    this->state = data;
}

/**
 * Update mission progress information to be sent periodically to the
 * ground control station.
 */
void CommsManager::updateTrajectoryProgress(fmav_controller_status_t data) {
    this->trajectoryStatus = data;
    fmav_msg_controller_status_encode_to_serial(this->sysid, this->compid,
                                                &data, &(this->status));
}

/**
 * Update vehicle health information to be sent periodically to the
 * ground control station.
 */
void CommsManager::updateHealth(fmav_sys_status_t data) {
    this->health = data;
}

/**
 * Send a status text message to ground control station. Can be
 * used to send debug messages, warnings, or errors.
 */
void CommsManager::sendStatusText(MAV_SEVERITY severity, const char *text) {
}

/* ----- PRIVATE HELPERS ----- */

/**
 * General message processor. Routes messages to respective handlers.
 * TODO: Consider whether a dynamic subscriber system would be worth the
 * overhead compared to manually routing based on switch statement
 */
void CommsManager::processMessage(fmav_message_t *msg) {
    switch (msg->msgid) {
        case FASTMAVLINK_MSG_ID_COMMAND_LONG:
            fmav_command_long_t command;
            fmav_msg_command_long_decode(&command, msg);
            this->processCommand(&command);
            break;
        case FASTMAVLINK_MSG_ID_PARAM_REQUEST_LIST:
            break; // Handling currently not implemented
        case FASTMAVLINK_MSG_ID_PARAM_REQUEST_READ:
            break; // Handling currently not implemented
        case FASTMAVLINK_MSG_ID_PARAM_SET:
            break; // Handling currently not implemented
        case FASTMAVLINK_MSG_ID_TRAJ_INFO:
            break; // Handling currently not implemented
        case FASTMAVLINK_MSG_ID_TRAJ_K1:
            break; // Handling currently not implemented
        case FASTMAVLINK_MSG_ID_TRAJ_K2:
            break; // Handling currently not implemented
        case FASTMAVLINK_MSG_ID_TRAJ_PT:
            break; // Handling currently not implemented
        case FASTMAVLINK_MSG_ID_TRAJ_ACK:
            break; // Handling currently not implemented
        default:
            break; // Handling currently not implemented
    }
}

/**
 * Command processor. Further routes commands to respecting handlers.
 */
void CommsManager::processCommand(fmav_command_long_t *cmd) {
    switch (cmd->command) {
        case MAV_CMD_DO_SET_MISSION_CURRENT:
            break;
        case MAV_CMD_DO_PAUSE_CONTINUE:
            break;
        case MAV_CMD_NAV_LAND:
            break;
        case MAV_CMD_MISSION_START:
            break;
        case MAV_CMD_MISSION_LOAD_SD:
            break;
    }
}


/**
 * Send telemetry
 */
void CommsManager::sendTelem() {
    fmav_msg_control_system_state_encode_to_serial(
        this->sysid, this->compid,
        &(this->state), &(this->status)
    );
}

/**
 * Send health
 */
void CommsManager::sendHealth() {
    fmav_msg_sys_status_encode_to_serial(
        this->sysid, this->compid,
        &(this->health), &(this->status)
    );
}

/**
 * Handle mission start command
 */
void CommsManager::handleStartMission() {
}

/** 
 * Handle mission pause command
 */
void CommsManager::handlePauseMission() {
}

/**
 * Handle mission land command
 */
void CommsManager::handleLandMission() {
}

/** 
 * Send heartbeat message
 */
void CommsManager::sendHeartbeat() {
    fmav_heartbeat_t hb = {
        .base_mode = 0,
        .custom_mode = 0,
        .autopilot = 69,
        .type = MAV_TYPE_ROCKET,
        .system_status = MAV_STATE_ACTIVE, // TODO: Allow updating state
    };
    fmav_status_t status;
    fmav_msg_heartbeat_encode_to_serial(
        this->sysid, this->compid,
        &hb, &(this->status)
    );
}

/**
 * Send request for trajectory gain matrix part 1
 */
void CommsManager::sendTrajK1Req(int k) {
}

/**
 * Send request for trajectory gain matrix part 2
 */
void CommsManager::sendTrajK2Req(int k) {
}

/**
 * Send request for trajectory point
 */
void CommsManager::sendTrajPtReq(int n) {
}


/**
 * Private 
 */
