//------------------------------
// The fastMavlink library
// (c) OlliW, OlliW42, www.olliw.eu
//------------------------------

#pragma once
#ifndef FASTMAVLINK_MSG_TRAJ_ACK_H
#define FASTMAVLINK_MSG_TRAJ_ACK_H


//----------------------------------------
//-- Message TRAJ_ACK
//----------------------------------------

// fields are ordered, as they appear on the wire
FASTMAVLINK_PACK(
typedef struct _fmav_traj_ack_t {
    uint32_t points_loaded;
    uint8_t result;
}) fmav_traj_ack_t;


#define FASTMAVLINK_MSG_ID_TRAJ_ACK  51820

#define FASTMAVLINK_MSG_TRAJ_ACK_PAYLOAD_LEN_MAX  5
#define FASTMAVLINK_MSG_TRAJ_ACK_CRCEXTRA  218

#define FASTMAVLINK_MSG_TRAJ_ACK_FLAGS  0
#define FASTMAVLINK_MSG_TRAJ_ACK_TARGET_SYSTEM_OFS  0
#define FASTMAVLINK_MSG_TRAJ_ACK_TARGET_COMPONENT_OFS  0

#define FASTMAVLINK_MSG_TRAJ_ACK_FRAME_LEN_MAX  30



#define FASTMAVLINK_MSG_TRAJ_ACK_FIELD_POINTS_LOADED_OFS  0
#define FASTMAVLINK_MSG_TRAJ_ACK_FIELD_RESULT_OFS  4


//----------------------------------------
//-- Message TRAJ_ACK pack,encode routines, for sending
//----------------------------------------

FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_ack_pack(
    fmav_message_t* _msg,
    uint8_t sysid,
    uint8_t compid,
    uint32_t points_loaded, uint8_t result,
    fmav_status_t* _status)
{
    fmav_traj_ack_t* _payload = (fmav_traj_ack_t*)_msg->payload;

    _payload->points_loaded = points_loaded;
    _payload->result = result;


    _msg->sysid = sysid;
    _msg->compid = compid;
    _msg->msgid = FASTMAVLINK_MSG_ID_TRAJ_ACK;
    _msg->target_sysid = 0;
    _msg->target_compid = 0;
    _msg->crc_extra = FASTMAVLINK_MSG_TRAJ_ACK_CRCEXTRA;
    _msg->payload_max_len = FASTMAVLINK_MSG_TRAJ_ACK_PAYLOAD_LEN_MAX;

    return fmav_finalize_msg(_msg, _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_ack_encode(
    fmav_message_t* _msg,
    uint8_t sysid,
    uint8_t compid,
    const fmav_traj_ack_t* _payload,
    fmav_status_t* _status)
{
    return fmav_msg_traj_ack_pack(
        _msg, sysid, compid,
        _payload->points_loaded, _payload->result,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_ack_pack_to_frame_buf(
    uint8_t* _buf,
    uint8_t sysid,
    uint8_t compid,
    uint32_t points_loaded, uint8_t result,
    fmav_status_t* _status)
{
    fmav_traj_ack_t* _payload = (fmav_traj_ack_t*)(&_buf[FASTMAVLINK_HEADER_V2_LEN]);

    _payload->points_loaded = points_loaded;
    _payload->result = result;


    _buf[5] = sysid;
    _buf[6] = compid;
    _buf[7] = (uint8_t)FASTMAVLINK_MSG_ID_TRAJ_ACK;
    _buf[8] = ((uint32_t)FASTMAVLINK_MSG_ID_TRAJ_ACK >> 8);
    _buf[9] = ((uint32_t)FASTMAVLINK_MSG_ID_TRAJ_ACK >> 16);

    return fmav_finalize_frame_buf(
        _buf,
        FASTMAVLINK_MSG_TRAJ_ACK_PAYLOAD_LEN_MAX,
        FASTMAVLINK_MSG_TRAJ_ACK_CRCEXTRA,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_ack_encode_to_frame_buf(
    uint8_t* _buf,
    uint8_t sysid,
    uint8_t compid,
    const fmav_traj_ack_t* _payload,
    fmav_status_t* _status)
{
    return fmav_msg_traj_ack_pack_to_frame_buf(
        _buf, sysid, compid,
        _payload->points_loaded, _payload->result,
        _status);
}


#ifdef FASTMAVLINK_SERIAL_WRITE_CHAR

FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_ack_pack_to_serial(
    uint8_t sysid,
    uint8_t compid,
    uint32_t points_loaded, uint8_t result,
    fmav_status_t* _status)
{
    fmav_traj_ack_t _payload;

    _payload.points_loaded = points_loaded;
    _payload.result = result;


    return fmav_finalize_serial(
        sysid,
        compid,
        (uint8_t*)&_payload,
        FASTMAVLINK_MSG_ID_TRAJ_ACK,
        FASTMAVLINK_MSG_TRAJ_ACK_PAYLOAD_LEN_MAX,
        FASTMAVLINK_MSG_TRAJ_ACK_CRCEXTRA,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_ack_encode_to_serial(
    uint8_t sysid,
    uint8_t compid,
    const fmav_traj_ack_t* _payload,
    fmav_status_t* _status)
{
    return fmav_finalize_serial(
        sysid,
        compid,
        (uint8_t*)_payload,
        FASTMAVLINK_MSG_ID_TRAJ_ACK,
        FASTMAVLINK_MSG_TRAJ_ACK_PAYLOAD_LEN_MAX,
        FASTMAVLINK_MSG_TRAJ_ACK_CRCEXTRA,
        _status);
}
#endif


//----------------------------------------
//-- Message TRAJ_ACK decode routines, for receiving
//----------------------------------------
// For these functions to work correctly, the msg payload must be zero filled.
// Call the helper fmav_msg_zerofill() if needed, or set FASTMAVLINK_ALWAYS_ZEROFILL to 1
// Note that the parse functions do zerofill the msg payload, but that message generator functions
// do not. This means that for the msg obtained from parsing the below functions can safely be used,
// but that this is not so for the msg obtained from pack/encode functions.

FASTMAVLINK_FUNCTION_DECORATOR void fmav_msg_traj_ack_decode(fmav_traj_ack_t* payload, const fmav_message_t* msg)
{
#if FASTMAVLINK_ALWAYS_ZEROFILL
    if (msg->len < FASTMAVLINK_MSG_TRAJ_ACK_PAYLOAD_LEN_MAX) {
        memcpy(payload, msg->payload, msg->len);
        // ensure that returned payload is zero filled
        memset(&(((uint8_t*)payload)[msg->len]), 0, FASTMAVLINK_MSG_TRAJ_ACK_PAYLOAD_LEN_MAX - msg->len);
    } else {
        // note: msg->len can be larger than PAYLOAD_LEN_MAX if the message has unknown extensions
        memcpy(payload, msg->payload, FASTMAVLINK_MSG_TRAJ_ACK_PAYLOAD_LEN_MAX);
    }
#else
    // this requires that msg payload had been zero filled before
    memcpy(payload, msg->payload, FASTMAVLINK_MSG_TRAJ_ACK_PAYLOAD_LEN_MAX);
#endif
}


FASTMAVLINK_FUNCTION_DECORATOR uint32_t fmav_msg_traj_ack_get_field_points_loaded(const fmav_message_t* msg)
{
    uint32_t r;
    memcpy(&r, &(msg->payload[0]), sizeof(uint32_t));
    return r;
}


FASTMAVLINK_FUNCTION_DECORATOR uint8_t fmav_msg_traj_ack_get_field_result(const fmav_message_t* msg)
{
    uint8_t r;
    memcpy(&r, &(msg->payload[4]), sizeof(uint8_t));
    return r;
}





//----------------------------------------
//-- Pymavlink wrappers
//----------------------------------------
#ifdef FASTMAVLINK_PYMAVLINK_ENABLED

#define MAVLINK_MSG_ID_TRAJ_ACK  51820

#define mavlink_traj_ack_t  fmav_traj_ack_t

#define MAVLINK_MSG_ID_TRAJ_ACK_LEN  5
#define MAVLINK_MSG_ID_TRAJ_ACK_MIN_LEN  5
#define MAVLINK_MSG_ID_51820_LEN  5
#define MAVLINK_MSG_ID_51820_MIN_LEN  5

#define MAVLINK_MSG_ID_TRAJ_ACK_CRC  218
#define MAVLINK_MSG_ID_51820_CRC  218




#if MAVLINK_COMM_NUM_BUFFERS > 0

FASTMAVLINK_FUNCTION_DECORATOR uint16_t mavlink_msg_traj_ack_pack(
    uint8_t sysid,
    uint8_t compid,
    mavlink_message_t* _msg,
    uint32_t points_loaded, uint8_t result)
{
    fmav_status_t* _status = mavlink_get_channel_status(MAVLINK_COMM_0);
    return fmav_msg_traj_ack_pack(
        _msg, sysid, compid,
        points_loaded, result,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t mavlink_msg_traj_ack_encode(
    uint8_t sysid,
    uint8_t compid,
    mavlink_message_t* _msg,
    const mavlink_traj_ack_t* _payload)
{
    return mavlink_msg_traj_ack_pack(
        sysid,
        compid,
        _msg,
        _payload->points_loaded, _payload->result);
}

#endif


FASTMAVLINK_FUNCTION_DECORATOR uint16_t mavlink_msg_traj_ack_pack_txbuf(
    char* _buf,
    fmav_status_t* _status,
    uint8_t sysid,
    uint8_t compid,
    uint32_t points_loaded, uint8_t result)
{
    return fmav_msg_traj_ack_pack_to_frame_buf(
        (uint8_t*)_buf,
        sysid,
        compid,
        points_loaded, result,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR void mavlink_msg_traj_ack_decode(const mavlink_message_t* msg, mavlink_traj_ack_t* payload)
{
    fmav_msg_traj_ack_decode(payload, msg);
}

#endif // FASTMAVLINK_PYMAVLINK_ENABLED


#endif // FASTMAVLINK_MSG_TRAJ_ACK_H
