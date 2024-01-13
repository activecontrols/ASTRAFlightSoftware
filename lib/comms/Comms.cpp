#include "Comms.h"
//#include "../estimator/Estimator.h"
//#include <stdlib.h>
#include "../error/Error.h"
#include <Arduino.h>
#include <cstring>

/* ----- PUBLIC INTERFACES ----- */

/**
 * We leave the constructor and init empty for now; most initialization
 * functions should be handled in init() in the style of most Arduino
 * libraries.
 */
CommsManager::CommsManager() { }
void CommsManager::init() { }

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

    if (millis() - this->lastHeartbeat > (1000 / HEARTBEAT_HZ)) {
        this->sendHeartbeat();
        this->lastHeartbeat = millis();
    }

    if (millis() - lastTelem > (1000 / TELEM_HZ)) {
        this->sendTelem();
        this->lastTelem = millis();
    }

    if (millis() - lastHealth > (1000 / HEALTH_HZ)) {
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
    int len = strlen(text);
    if (this->statusTextID == 0) {
        this->statusTextID = 1;
    }
    while (len > 0) {
        fmav_statustext_t data;
        data.severity = severity;
        // TODO: We should limit this to read less than 50 at the tail
        // But for some reason that duplicates the text
        // So for now we do this and assume memory security isn't an issue
        memcpy(data.text, text, 50);
        data.id = this->statusTextID;
        fmav_msg_statustext_encode_to_serial(this->sysid, this->compid,
                                             &data, &(this->status));
        len -= 50;
        text += 50;
    }

    this->statusTextID += 1; // Will underflow automatically, no round necessary
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
            this->processCommand(msg->sysid, msg->compid, &command);
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
void CommsManager::processCommand(uint8_t sysid, uint8_t compid, fmav_command_long_t *cmd) {
    // TODO: More generic method of registering handlers
    switch (cmd->command) {
        case MAV_CMD_DO_PAUSE_CONTINUE: {
            if (this->missionPauseCallback == NULL) {
                this->rejectCommand(cmd->command, (String("IGNORE CMD: No callback to execute pause registered. cont=") + String((int)cmd->param1)).c_str());
                return;
            }
            fmav_command_ack_t ack = this->missionPauseCallback((int)cmd->param1);
            fmav_msg_command_ack_encode_to_serial(this->sysid, this->compid,
                                              &ack, &(this->status));
            break;
        }
        case MAV_CMD_NAV_LAND: {
            if (this->missionLandCallback == NULL) {
                this->rejectCommand(cmd->command, "IGNORE CMD: No callback to execute LAND registered.");
                return;
            }
            break;
        }
        case MAV_CMD_MISSION_START: {
            if (this->missionStartCallback == NULL) {
                this->rejectCommand(cmd->command, "IGNORE CMD: No callback to execute mission start registered.");
                return;
            }
            fmav_command_ack_t ack = this->missionStartCallback();
            fmav_msg_command_ack_encode_to_serial(this->sysid, this->compid,
                                              &ack, &(this->status));
            break;
        }
        case MAV_CMD_MISSION_LOAD_SD: {
            if (this->trajLoadSDCallback == NULL) {
                this->rejectCommand(cmd->command, (String("IGNORE CMD: No callback to load trajectory from SD card registered. mission=") + String((int)cmd->param1)).c_str());
                return;
            }
            fmav_traj_ack_t ack = this->trajLoadSDCallback((int)cmd->param1);
            fmav_msg_traj_ack_encode_to_serial(this->sysid, this->compid,
                                               &ack, &(this->status));
            break;
        }
        default: {
            // We return the generic acknowledgement UNSUPPORTED
            fmav_command_ack_t ack;
            ack.result = MAV_RESULT_UNSUPPORTED;
            ack.command = cmd->command;
            ack.target_system = sysid;
            ack.target_component = compid;
            fmav_msg_command_ack_encode_to_serial(this->sysid, this->compid,
                                              &ack, &(this->status));
            break;
        }
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
 * Register handlers for certain command messages
 * These functions should be used with pointers to handling functions
 * to trigger actions on certain commands (e.g. comms.registerMissionStartAction(func)
 * will call func when the mission start command is received)
 */
void CommsManager::registerMissionStartAction(fmav_command_ack_t (*callback) (void)) {
    this->missionStartCallback = callback;
}
void CommsManager::registerMissionPauseAction(fmav_command_ack_t (*callback) (int)) {
    this->missionPauseCallback = callback;
}
void CommsManager::registerLandAction(fmav_command_ack_t (*callback) (void)) {
    this->missionLandCallback = callback;
}
void CommsManager::registerTrajSDLoadAction(fmav_traj_ack_t (*callback) (int number)) {
    this->trajLoadSDCallback = callback;
}

/**
 * Send acknowledgement of trajectory (trajectory read success? not enough memory?)
 */
// void CommsManager::sendTrajAck(fmav_traj_ack_t ack) {
//     fmav_msg_traj_ack_encode_to_serial(this->sysid, this->compid,
//                                        &ack, &(this->status));
// }


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
        .custom_mode = 0,
        .type = MAV_TYPE_ROCKET,
        .autopilot = 69,
        .base_mode = 0,
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

/**
 * Reject command with FAILED, mostly used for missing handlers
 */
void CommsManager::rejectCommand(uint16_t command, const char *reason) {
    this->sendStatusText(MAV_SEVERITY_WARNING, reason);
    fmav_command_ack_t ack;
    ack.command = command;
    ack.result = MAV_RESULT_FAILED;
    fmav_msg_command_ack_encode_to_serial(this->sysid, this->compid,
                                      &ack, &(this->status));
}

/* ----- PRIVATE HELPER ----- */

/* We want to define a serial helper for our fastmavlink
 * library so it can more efficiently write messages to serial
 * without requiring intermediate buffers
 */
#define FASTMAVLINK_SERIAL_WRITE_CHAR 1
void fmav_serial_write_char(char c) {
    MAVLinkSerial.write(c);
}
