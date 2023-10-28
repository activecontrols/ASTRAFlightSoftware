//------------------------------
// The fastMavlink library
// (c) OlliW, OlliW42, www.olliw.eu
//------------------------------

#pragma once
#ifndef FASTMAVLINK_MSG_ENTRIES_H
#define FASTMAVLINK_MSG_ENTRIES_H


//------------------------------
//-- Message credentials
//-- The values of msg_entry_t for all messages in the dialect.
//-- msgid, extra crc, max length, flag, target sysid offset, target compid offset
//------------------------------

#define FASTMAVLINK_MSG_ENTRY_HEARTBEAT  {0, 50, 9, 0, 0, 0}
#define FASTMAVLINK_MSG_ENTRY_SYS_STATUS  {1, 124, 31, 0, 0, 0}
#define FASTMAVLINK_MSG_ENTRY_PARAM_REQUEST_READ  {20, 214, 20, 3, 2, 3}
#define FASTMAVLINK_MSG_ENTRY_PARAM_REQUEST_LIST  {21, 159, 2, 3, 0, 1}
#define FASTMAVLINK_MSG_ENTRY_PARAM_VALUE  {22, 220, 25, 0, 0, 0}
#define FASTMAVLINK_MSG_ENTRY_PARAM_SET  {23, 168, 23, 3, 4, 5}
#define FASTMAVLINK_MSG_ENTRY_COMMAND_LONG  {76, 152, 33, 3, 30, 31}
#define FASTMAVLINK_MSG_ENTRY_COMMAND_ACK  {77, 143, 10, 3, 8, 9}
#define FASTMAVLINK_MSG_ENTRY_CONTROL_SYSTEM_STATE  {146, 103, 100, 0, 0, 0}
#define FASTMAVLINK_MSG_ENTRY_PROTOCOL_VERSION  {300, 217, 22, 0, 0, 0}
#define FASTMAVLINK_MSG_ENTRY_CONTROLLER_STATUS  {51801, 246, 8, 0, 0, 0}


/*------------------------------
 * If only relatively few MAVLink messages are used, efficiency can
 * be much improved, both memory and computational time wise, by
 * limiting the known message entries to only those which are used.
 *
 * This can be achieved by commenting out in the below define of
 * FASTMAVLINK_MSG_ENTRIES all those message entries which are not used.
 *
 * Alternatively, one can define one's own FASTMAVLINK_MESSAGE_CRCS
 * using the above defines for each message entry. It is then MOST
 * important to keep the sequence in order since otherwise the default
 * binary search will fail. For instance:
 *
 * #include "pathtofastmavlink/thedialect/fmav_msg_entries.h"
 * #define FASTMAVLINK_MESSAGE_CRCS {\
 *     FASTMAVLINK_MSG_ENTRY_PARAM_REQUEST_READ,\
 *     FASTMAVLINK_MSG_ENTRY_PARAM_REQUEST_LIST,\
 *     FASTMAVLINK_MSG_ENTRY_PARAM_SET,\
 *     FASTMAVLINK_MSG_ENTRY_COMMAND_LONG,\
 *     FASTMAVLINK_MSG_ENTRY_AUTOPILOT_VERSION_REQUEST }
 ------------------------------*/

#define FASTMAVLINK_MSG_ENTRIES {\
  FASTMAVLINK_MSG_ENTRY_HEARTBEAT,\
  FASTMAVLINK_MSG_ENTRY_SYS_STATUS,\
  FASTMAVLINK_MSG_ENTRY_PARAM_REQUEST_READ,\
  FASTMAVLINK_MSG_ENTRY_PARAM_REQUEST_LIST,\
  FASTMAVLINK_MSG_ENTRY_PARAM_VALUE,\
  FASTMAVLINK_MSG_ENTRY_PARAM_SET,\
  FASTMAVLINK_MSG_ENTRY_COMMAND_LONG,\
  FASTMAVLINK_MSG_ENTRY_COMMAND_ACK,\
  FASTMAVLINK_MSG_ENTRY_CONTROL_SYSTEM_STATE,\
  FASTMAVLINK_MSG_ENTRY_PROTOCOL_VERSION,\
  FASTMAVLINK_MSG_ENTRY_CONTROLLER_STATUS\
}


#endif // FASTMAVLINK_MSG_ENTRIES_H
