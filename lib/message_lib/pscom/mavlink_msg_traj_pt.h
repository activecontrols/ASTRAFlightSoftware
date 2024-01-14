//------------------------------
// The fastMavlink library
// (c) OlliW, OlliW42, www.olliw.eu
//------------------------------

#pragma once
#ifndef FASTMAVLINK_MSG_TRAJ_PT_H
#define FASTMAVLINK_MSG_TRAJ_PT_H


//----------------------------------------
//-- Message TRAJ_PT
//----------------------------------------

// fields are ordered, as they appear on the wire
FASTMAVLINK_PACK(
typedef struct _fmav_traj_pt_t {
    uint32_t k;
    float x[12];
    float uref[4];
    float time;
}) fmav_traj_pt_t;


#define FASTMAVLINK_MSG_ID_TRAJ_PT  51816

#define FASTMAVLINK_MSG_TRAJ_PT_PAYLOAD_LEN_MAX  72
#define FASTMAVLINK_MSG_TRAJ_PT_CRCEXTRA  55

#define FASTMAVLINK_MSG_TRAJ_PT_FLAGS  0
#define FASTMAVLINK_MSG_TRAJ_PT_TARGET_SYSTEM_OFS  0
#define FASTMAVLINK_MSG_TRAJ_PT_TARGET_COMPONENT_OFS  0

#define FASTMAVLINK_MSG_TRAJ_PT_FRAME_LEN_MAX  97

#define FASTMAVLINK_MSG_TRAJ_PT_FIELD_X_NUM  12 // number of elements in array
#define FASTMAVLINK_MSG_TRAJ_PT_FIELD_X_LEN  48 // length of array = number of bytes
#define FASTMAVLINK_MSG_TRAJ_PT_FIELD_UREF_NUM  4 // number of elements in array
#define FASTMAVLINK_MSG_TRAJ_PT_FIELD_UREF_LEN  16 // length of array = number of bytes

#define FASTMAVLINK_MSG_TRAJ_PT_FIELD_K_OFS  0
#define FASTMAVLINK_MSG_TRAJ_PT_FIELD_X_OFS  4
#define FASTMAVLINK_MSG_TRAJ_PT_FIELD_UREF_OFS  52
#define FASTMAVLINK_MSG_TRAJ_PT_FIELD_TIME_OFS  68


//----------------------------------------
//-- Message TRAJ_PT pack,encode routines, for sending
//----------------------------------------

FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_pt_pack(
    fmav_message_t* _msg,
    uint8_t sysid,
    uint8_t compid,
    uint32_t k, const float* x, const float* uref, float time,
    fmav_status_t* _status)
{
    fmav_traj_pt_t* _payload = (fmav_traj_pt_t*)_msg->payload;

    _payload->k = k;
    _payload->time = time;
    memcpy(&(_payload->x), x, sizeof(float)*12);
    memcpy(&(_payload->uref), uref, sizeof(float)*4);

    _msg->sysid = sysid;
    _msg->compid = compid;
    _msg->msgid = FASTMAVLINK_MSG_ID_TRAJ_PT;
    _msg->target_sysid = 0;
    _msg->target_compid = 0;
    _msg->crc_extra = FASTMAVLINK_MSG_TRAJ_PT_CRCEXTRA;
    _msg->payload_max_len = FASTMAVLINK_MSG_TRAJ_PT_PAYLOAD_LEN_MAX;

    return fmav_finalize_msg(_msg, _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_pt_encode(
    fmav_message_t* _msg,
    uint8_t sysid,
    uint8_t compid,
    const fmav_traj_pt_t* _payload,
    fmav_status_t* _status)
{
    return fmav_msg_traj_pt_pack(
        _msg, sysid, compid,
        _payload->k, _payload->x, _payload->uref, _payload->time,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_pt_pack_to_frame_buf(
    uint8_t* _buf,
    uint8_t sysid,
    uint8_t compid,
    uint32_t k, const float* x, const float* uref, float time,
    fmav_status_t* _status)
{
    fmav_traj_pt_t* _payload = (fmav_traj_pt_t*)(&_buf[FASTMAVLINK_HEADER_V2_LEN]);

    _payload->k = k;
    _payload->time = time;
    memcpy(&(_payload->x), x, sizeof(float)*12);
    memcpy(&(_payload->uref), uref, sizeof(float)*4);

    _buf[5] = sysid;
    _buf[6] = compid;
    _buf[7] = (uint8_t)FASTMAVLINK_MSG_ID_TRAJ_PT;
    _buf[8] = ((uint32_t)FASTMAVLINK_MSG_ID_TRAJ_PT >> 8);
    _buf[9] = ((uint32_t)FASTMAVLINK_MSG_ID_TRAJ_PT >> 16);

    return fmav_finalize_frame_buf(
        _buf,
        FASTMAVLINK_MSG_TRAJ_PT_PAYLOAD_LEN_MAX,
        FASTMAVLINK_MSG_TRAJ_PT_CRCEXTRA,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_pt_encode_to_frame_buf(
    uint8_t* _buf,
    uint8_t sysid,
    uint8_t compid,
    const fmav_traj_pt_t* _payload,
    fmav_status_t* _status)
{
    return fmav_msg_traj_pt_pack_to_frame_buf(
        _buf, sysid, compid,
        _payload->k, _payload->x, _payload->uref, _payload->time,
        _status);
}


#ifdef FASTMAVLINK_SERIAL_WRITE_CHAR

FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_pt_pack_to_serial(
    uint8_t sysid,
    uint8_t compid,
    uint32_t k, const float* x, const float* uref, float time,
    fmav_status_t* _status)
{
    fmav_traj_pt_t _payload;

    _payload.k = k;
    _payload.time = time;
    memcpy(&(_payload.x), x, sizeof(float)*12);
    memcpy(&(_payload.uref), uref, sizeof(float)*4);

    return fmav_finalize_serial(
        sysid,
        compid,
        (uint8_t*)&_payload,
        FASTMAVLINK_MSG_ID_TRAJ_PT,
        FASTMAVLINK_MSG_TRAJ_PT_PAYLOAD_LEN_MAX,
        FASTMAVLINK_MSG_TRAJ_PT_CRCEXTRA,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_pt_encode_to_serial(
    uint8_t sysid,
    uint8_t compid,
    const fmav_traj_pt_t* _payload,
    fmav_status_t* _status)
{
    return fmav_finalize_serial(
        sysid,
        compid,
        (uint8_t*)_payload,
        FASTMAVLINK_MSG_ID_TRAJ_PT,
        FASTMAVLINK_MSG_TRAJ_PT_PAYLOAD_LEN_MAX,
        FASTMAVLINK_MSG_TRAJ_PT_CRCEXTRA,
        _status);
}
#endif


//----------------------------------------
//-- Message TRAJ_PT decode routines, for receiving
//----------------------------------------
// For these functions to work correctly, the msg payload must be zero filled.
// Call the helper fmav_msg_zerofill() if needed, or set FASTMAVLINK_ALWAYS_ZEROFILL to 1
// Note that the parse functions do zerofill the msg payload, but that message generator functions
// do not. This means that for the msg obtained from parsing the below functions can safely be used,
// but that this is not so for the msg obtained from pack/encode functions.

FASTMAVLINK_FUNCTION_DECORATOR void fmav_msg_traj_pt_decode(fmav_traj_pt_t* payload, const fmav_message_t* msg)
{
#if FASTMAVLINK_ALWAYS_ZEROFILL
    if (msg->len < FASTMAVLINK_MSG_TRAJ_PT_PAYLOAD_LEN_MAX) {
        memcpy(payload, msg->payload, msg->len);
        // ensure that returned payload is zero filled
        memset(&(((uint8_t*)payload)[msg->len]), 0, FASTMAVLINK_MSG_TRAJ_PT_PAYLOAD_LEN_MAX - msg->len);
    } else {
        // note: msg->len can be larger than PAYLOAD_LEN_MAX if the message has unknown extensions
        memcpy(payload, msg->payload, FASTMAVLINK_MSG_TRAJ_PT_PAYLOAD_LEN_MAX);
    }
#else
    // this requires that msg payload had been zero filled before
    memcpy(payload, msg->payload, FASTMAVLINK_MSG_TRAJ_PT_PAYLOAD_LEN_MAX);
#endif
}


FASTMAVLINK_FUNCTION_DECORATOR uint32_t fmav_msg_traj_pt_get_field_k(const fmav_message_t* msg)
{
    uint32_t r;
    memcpy(&r, &(msg->payload[0]), sizeof(uint32_t));
    return r;
}


FASTMAVLINK_FUNCTION_DECORATOR float fmav_msg_traj_pt_get_field_time(const fmav_message_t* msg)
{
    float r;
    memcpy(&r, &(msg->payload[68]), sizeof(float));
    return r;
}


FASTMAVLINK_FUNCTION_DECORATOR float* fmav_msg_traj_pt_get_field_x_ptr(const fmav_message_t* msg)
{
    return (float*)&(msg->payload[4]);
}


FASTMAVLINK_FUNCTION_DECORATOR float fmav_msg_traj_pt_get_field_x(uint16_t index, const fmav_message_t* msg)
{
    if (index >= FASTMAVLINK_MSG_TRAJ_PT_FIELD_X_NUM) return 0;
    return ((float*)&(msg->payload[4]))[index];
}


FASTMAVLINK_FUNCTION_DECORATOR float* fmav_msg_traj_pt_get_field_uref_ptr(const fmav_message_t* msg)
{
    return (float*)&(msg->payload[52]);
}


FASTMAVLINK_FUNCTION_DECORATOR float fmav_msg_traj_pt_get_field_uref(uint16_t index, const fmav_message_t* msg)
{
    if (index >= FASTMAVLINK_MSG_TRAJ_PT_FIELD_UREF_NUM) return 0;
    return ((float*)&(msg->payload[52]))[index];
}


//----------------------------------------
//-- Pymavlink wrappers
//----------------------------------------
#ifdef FASTMAVLINK_PYMAVLINK_ENABLED

#define MAVLINK_MSG_ID_TRAJ_PT  51816

#define mavlink_traj_pt_t  fmav_traj_pt_t

#define MAVLINK_MSG_ID_TRAJ_PT_LEN  72
#define MAVLINK_MSG_ID_TRAJ_PT_MIN_LEN  72
#define MAVLINK_MSG_ID_51816_LEN  72
#define MAVLINK_MSG_ID_51816_MIN_LEN  72

#define MAVLINK_MSG_ID_TRAJ_PT_CRC  55
#define MAVLINK_MSG_ID_51816_CRC  55

#define MAVLINK_MSG_TRAJ_PT_FIELD_X_LEN 12
#define MAVLINK_MSG_TRAJ_PT_FIELD_UREF_LEN 4


#if MAVLINK_COMM_NUM_BUFFERS > 0

FASTMAVLINK_FUNCTION_DECORATOR uint16_t mavlink_msg_traj_pt_pack(
    uint8_t sysid,
    uint8_t compid,
    mavlink_message_t* _msg,
    uint32_t k, const float* x, const float* uref, float time)
{
    fmav_status_t* _status = mavlink_get_channel_status(MAVLINK_COMM_0);
    return fmav_msg_traj_pt_pack(
        _msg, sysid, compid,
        k, x, uref, time,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t mavlink_msg_traj_pt_encode(
    uint8_t sysid,
    uint8_t compid,
    mavlink_message_t* _msg,
    const mavlink_traj_pt_t* _payload)
{
    return mavlink_msg_traj_pt_pack(
        sysid,
        compid,
        _msg,
        _payload->k, _payload->x, _payload->uref, _payload->time);
}

#endif


FASTMAVLINK_FUNCTION_DECORATOR uint16_t mavlink_msg_traj_pt_pack_txbuf(
    char* _buf,
    fmav_status_t* _status,
    uint8_t sysid,
    uint8_t compid,
    uint32_t k, const float* x, const float* uref, float time)
{
    return fmav_msg_traj_pt_pack_to_frame_buf(
        (uint8_t*)_buf,
        sysid,
        compid,
        k, x, uref, time,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR void mavlink_msg_traj_pt_decode(const mavlink_message_t* msg, mavlink_traj_pt_t* payload)
{
    fmav_msg_traj_pt_decode(payload, msg);
}

#endif // FASTMAVLINK_PYMAVLINK_ENABLED


#endif // FASTMAVLINK_MSG_TRAJ_PT_H
