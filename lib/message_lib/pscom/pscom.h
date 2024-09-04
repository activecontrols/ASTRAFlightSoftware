//------------------------------
// The fastMavlink library
// (c) OlliW, OlliW42, www.olliw.eu
//------------------------------

#pragma once
#ifndef FASTMAVLINK_PSCOM_H
#define FASTMAVLINK_PSCOM_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef FASTMAVLINK_BUILD_DATE
#define FASTMAVLINK_BUILD_DATE  "Sun Apr 21 2024"
#endif

#ifndef FASTMAVLINK_DIALECT_VERSION
#define FASTMAVLINK_DIALECT_VERSION  1  // this is the version specified in the dialect xml file
#endif


//------------------------------
//-- Message credentials
//-- The values of msg_entry_t for all messages in the dialect.
//-- msgid, extra crc, max length, flag, target sysid offset, target compid offset
//------------------------------

#include "pscom_msg_entries.h"

#ifndef FASTMAVLINK_MESSAGE_CRCS
#define FASTMAVLINK_MESSAGE_CRCS  FASTMAVLINK_MSG_ENTRIES
#endif


//------------------------------
//-- FastMavlink lib
//------------------------------

#include "../lib/fastmavlink.h"

#ifdef FASTMAVLINK_PYMAVLINK_ENABLED
#include "../lib/fastmavlink_pymavlink.h"
#endif


//------------------------------
//-- Enum definitons
//------------------------------

#ifndef FASTMAVLINK_TEST_EXCLUDE_ENUMS

#ifndef FASTMAVLINK_HAS_ENUM_MAV_PARAM_TYPE
#define FASTMAVLINK_HAS_ENUM_MAV_PARAM_TYPE
typedef enum MAV_PARAM_TYPE {
    MAV_PARAM_TYPE_UINT8 = 1,  // 8-bit unsigned integer 
    MAV_PARAM_TYPE_INT8 = 2,  // 8-bit signed integer 
    MAV_PARAM_TYPE_UINT16 = 3,  // 16-bit unsigned integer 
    MAV_PARAM_TYPE_INT16 = 4,  // 16-bit signed integer 
    MAV_PARAM_TYPE_UINT32 = 5,  // 32-bit unsigned integer 
    MAV_PARAM_TYPE_INT32 = 6,  // 32-bit signed integer 
    MAV_PARAM_TYPE_UINT64 = 7,  // 64-bit unsigned integer 
    MAV_PARAM_TYPE_INT64 = 8,  // 64-bit signed integer 
    MAV_PARAM_TYPE_REAL32 = 9,  // 32-bit floating-point 
    MAV_PARAM_TYPE_REAL64 = 10,  // 64-bit floating-point 
    MAV_PARAM_TYPE_ENUM_END = 11,  // end marker
} MAV_PARAM_TYPE;
#endif


#ifndef FASTMAVLINK_HAS_ENUM_MAV_RESULT
#define FASTMAVLINK_HAS_ENUM_MAV_RESULT
typedef enum MAV_RESULT {
    MAV_RESULT_ACCEPTED = 0,  // Command is valid (is supported and has valid parameters), and was executed. 
    MAV_RESULT_TEMPORARILY_REJECTED = 1,  // Command is valid, but cannot be executed at this time. This is used to indicate a problem that should be fixed just by waiting (e.g. a state machine is busy, can't arm because have not got GPS lock, etc.). Retrying later should work. 
    MAV_RESULT_DENIED = 2,  // Command is invalid (is supported but has invalid parameters). Retrying same command and parameters will not work. 
    MAV_RESULT_UNSUPPORTED = 3,  // Command is not supported (unknown). 
    MAV_RESULT_FAILED = 4,  // Command is valid, but execution has failed. This is used to indicate any non-temporary or unexpected problem, i.e. any problem that must be fixed before the command can succeed/be retried. For example, attempting to write a file when out of memory, attempting to arm when sensors are not calibrated, etc. 
    MAV_RESULT_IN_PROGRESS = 5,  // Command is valid and is being executed. This will be followed by further progress updates, i.e. the component may send further COMMAND_ACK messages with result MAV_RESULT_IN_PROGRESS (at a rate decided by the implementation), and must terminate by sending a COMMAND_ACK message with final result of the operation. The COMMAND_ACK.progress field can be used to indicate the progress of the operation. 
    MAV_RESULT_CANCELLED = 6,  // Command has been cancelled (as a result of receiving a COMMAND_CANCEL message). 
    MAV_RESULT_COMMAND_LONG_ONLY = 7,  // Command is only accepted when sent as a COMMAND_LONG. 
    MAV_RESULT_COMMAND_INT_ONLY = 8,  // Command is only accepted when sent as a COMMAND_INT. 
    MAV_RESULT_COMMAND_UNSUPPORTED_MAV_FRAME = 9,  // Command is invalid because a frame is required and the specified frame is not supported. 
    MAV_RESULT_ENUM_END = 10,  // end marker
} MAV_RESULT;
#endif


#ifndef FASTMAVLINK_HAS_ENUM_MAV_SYS_STATUS_SENSOR
#define FASTMAVLINK_HAS_ENUM_MAV_SYS_STATUS_SENSOR
typedef enum MAV_SYS_STATUS_SENSOR {
    MAV_SYS_STATUS_SENSOR_3D_GYRO = 1,  // 0x01 3D gyro 
    MAV_SYS_STATUS_SENSOR_3D_ACCEL = 2,  // 0x02 3D accelerometer 
    MAV_SYS_STATUS_SENSOR_3D_MAG = 4,  // 0x04 3D magnetometer 
    MAV_SYS_STATUS_SENSOR_ABSOLUTE_PRESSURE = 8,  // 0x08 absolute pressure 
    MAV_SYS_STATUS_SENSOR_DIFFERENTIAL_PRESSURE = 16,  // 0x10 differential pressure 
    MAV_SYS_STATUS_SENSOR_GPS = 32,  // 0x20 GPS 
    MAV_SYS_STATUS_SENSOR_OPTICAL_FLOW = 64,  // 0x40 optical flow 
    MAV_SYS_STATUS_SENSOR_VISION_POSITION = 128,  // 0x80 computer vision position 
    MAV_SYS_STATUS_SENSOR_LASER_POSITION = 256,  // 0x100 laser based position 
    MAV_SYS_STATUS_SENSOR_EXTERNAL_GROUND_TRUTH = 512,  // 0x200 external ground truth (Vicon or Leica) 
    MAV_SYS_STATUS_SENSOR_ANGULAR_RATE_CONTROL = 1024,  // 0x400 3D angular rate control 
    MAV_SYS_STATUS_SENSOR_ATTITUDE_STABILIZATION = 2048,  // 0x800 attitude stabilization 
    MAV_SYS_STATUS_SENSOR_YAW_POSITION = 4096,  // 0x1000 yaw position 
    MAV_SYS_STATUS_SENSOR_Z_ALTITUDE_CONTROL = 8192,  // 0x2000 z/altitude control 
    MAV_SYS_STATUS_SENSOR_XY_POSITION_CONTROL = 16384,  // 0x4000 x/y position control 
    MAV_SYS_STATUS_SENSOR_MOTOR_OUTPUTS = 32768,  // 0x8000 motor outputs / control 
    MAV_SYS_STATUS_SENSOR_RC_RECEIVER = 65536,  // 0x10000 RC receiver 
    MAV_SYS_STATUS_SENSOR_3D_GYRO2 = 131072,  // 0x20000 2nd 3D gyro 
    MAV_SYS_STATUS_SENSOR_3D_ACCEL2 = 262144,  // 0x40000 2nd 3D accelerometer 
    MAV_SYS_STATUS_SENSOR_3D_MAG2 = 524288,  // 0x80000 2nd 3D magnetometer 
    MAV_SYS_STATUS_GEOFENCE = 1048576,  // 0x100000 geofence 
    MAV_SYS_STATUS_AHRS = 2097152,  // 0x200000 AHRS subsystem health 
    MAV_SYS_STATUS_TERRAIN = 4194304,  // 0x400000 Terrain subsystem health 
    MAV_SYS_STATUS_REVERSE_MOTOR = 8388608,  // 0x800000 Motors are reversed 
    MAV_SYS_STATUS_LOGGING = 16777216,  // 0x1000000 Logging 
    MAV_SYS_STATUS_SENSOR_BATTERY = 33554432,  // 0x2000000 Battery 
    MAV_SYS_STATUS_SENSOR_PROXIMITY = 67108864,  // 0x4000000 Proximity 
    MAV_SYS_STATUS_SENSOR_SATCOM = 134217728,  // 0x8000000 Satellite Communication  
    MAV_SYS_STATUS_PREARM_CHECK = 268435456,  // 0x10000000 pre-arm check status. Always healthy when armed 
    MAV_SYS_STATUS_OBSTACLE_AVOIDANCE = 536870912,  // 0x20000000 Avoidance/collision prevention 
    MAV_SYS_STATUS_SENSOR_PROPULSION = 1073741824,  // 0x40000000 propulsion (actuator, esc, motor or propellor) 
    MAV_SYS_STATUS_EXTENSION_USED = 2147483648,  // 0x80000000 Extended bit-field are used for further sensor status bits (needs to be set in onboard_control_sensors_present only) 
    MAV_SYS_STATUS_SENSOR_ENUM_END = 2147483649,  // end marker
} MAV_SYS_STATUS_SENSOR;
#endif


#ifndef FASTMAVLINK_HAS_ENUM_MAV_SEVERITY
#define FASTMAVLINK_HAS_ENUM_MAV_SEVERITY
typedef enum MAV_SEVERITY {
    MAV_SEVERITY_EMERGENCY = 0,  // System is unusable. This is a "panic" condition. 
    MAV_SEVERITY_ALERT = 1,  // Action should be taken immediately. Indicates error in non-critical systems. 
    MAV_SEVERITY_CRITICAL = 2,  // Action must be taken immediately. Indicates failure in a primary system. 
    MAV_SEVERITY_ERROR = 3,  // Indicates an error in secondary/redundant systems. 
    MAV_SEVERITY_WARNING = 4,  // Indicates about a possible future error if this is not resolved within a given timeframe. Example would be a low battery warning. 
    MAV_SEVERITY_NOTICE = 5,  // An unusual event has occurred, though not an error condition. This should be investigated for the root cause. 
    MAV_SEVERITY_INFO = 6,  // Normal operational messages. Useful for logging. No action is required for these messages. 
    MAV_SEVERITY_DEBUG = 7,  // Useful non-operational messages that can assist in debugging. These should not occur during normal operation. 
    MAV_SEVERITY_ENUM_END = 8,  // end marker
} MAV_SEVERITY;
#endif


#ifndef FASTMAVLINK_HAS_ENUM_MAV_CMD
#define FASTMAVLINK_HAS_ENUM_MAV_CMD
typedef enum MAV_CMD {
    MAV_CMD_NAV_LAND = 21,  // Land at current location. | Minimum target altitude if landing is aborted (0 = undefined/use system default). | (IGNORED) Precision land mode. | Empty. | Desired yaw angle. NaN to use the current system yaw heading mode (e.g. yaw towards next waypoint, yaw to home, etc.). | (IGNORED) Latitude. | (IGNORED) Longitude. | Landing altitude (ground level in current frame).
    MAV_CMD_DO_PAUSE_CONTINUE = 193,  // Hold the current position or continue. | 0: Pause current mission or reposition command, hold current position. 1: Continue mission. On pause, enter hover mode. | Reserved | Reserved | Reserved | Reserved | Reserved | Reserved
    MAV_CMD_DO_SET_MISSION_CURRENT = 224,  //           Start the trajectory segment specified by Number. Note that this should also be used for RTL, since ASTRA does not support RTL.         | Mission sequence value to set. -1 for the current mission item (use to reset mission without changing current mission item). | (CURRENTLY IGNORED) Resets mission. 1: true, 0: false. Resets jump counters to initial values and changes mission state "completed" to be "active" or "paused". | Empty | Empty | Empty | Empty | Empty
    MAV_CMD_MISSION_START = 300,  // start running a mission | first_item: the first mission item to run | last_item:  the last mission item to run (after this item is run, the vehicle lands in place) | Reserved (default:0) | Reserved (default:0) | Reserved (default:0) | Reserved (default:0) | Reserved (default:0)
    MAV_CMD_MISSION_LOAD_SD = 51801,  // Load a trajectory mission from the SD card (/missions/mission{number}.atf) into PSRAM. | the mission number to load | Reserved (default:0) | Reserved (default:0) | Reserved (default:0) | Reserved (default:0) | Reserved (default:0) | Reserved (default:0)
    MAV_CMD_ENUM_END = 51802,  // end marker
} MAV_CMD;
#endif

#endif // FASTMAVLINK_DO_NOT_INCLUDE_ENUMS


//------------------------------
//-- Message definitions
//------------------------------

#ifdef FASTMAVLINK_IGNORE_WADDRESSOFPACKEDMEMBER
  #if defined __GNUC__ && __GNUC__ >= 9
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Waddress-of-packed-member"
  #endif
#endif

#include "./mavlink_msg_sys_status.h"
#include "./mavlink_msg_raw_imu.h"
#include "./mavlink_msg_control_system_state.h"
#include "./mavlink_msg_command_long.h"
#include "./mavlink_msg_command_ack.h"
#include "./mavlink_msg_param_request_read.h"
#include "./mavlink_msg_param_request_list.h"
#include "./mavlink_msg_param_value.h"
#include "./mavlink_msg_param_set.h"
#include "./mavlink_msg_statustext.h"
#include "./mavlink_msg_controller_status.h"
#include "./mavlink_msg_traj_info.h"
#include "./mavlink_msg_traj_req_k1.h"
#include "./mavlink_msg_traj_k1.h"
#include "./mavlink_msg_traj_req_k2.h"
#include "./mavlink_msg_traj_k2.h"
#include "./mavlink_msg_traj_req_pt.h"
#include "./mavlink_msg_traj_pt.h"
#include "./mavlink_msg_traj_ack.h"
#include "./mavlink_msg_command_simple.h"

#ifdef FASTMAVLINK_IGNORE_WADDRESSOFPACKEDMEMBER
  #if defined __GNUC__ && __GNUC__ >= 9
    #pragma GCC diagnostic pop
  #endif
#endif


//------------------------------
//-- Dialect includes
//------------------------------

#include "../minimal/minimal.h"


#ifdef __cplusplus
}
#endif

#endif // FASTMAVLINK_PSCOM_H
