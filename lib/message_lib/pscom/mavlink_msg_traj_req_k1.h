//------------------------------
// The fastMavlink library
// (c) OlliW, OlliW42, www.olliw.eu
//------------------------------

#pragma once
#ifndef FASTMAVLINK_MSG_TRAJ_REQ_K1_H
#define FASTMAVLINK_MSG_TRAJ_REQ_K1_H


//----------------------------------------
//-- Message TRAJ_REQ_K1
//----------------------------------------

// fields are ordered, as they appear on the wire
FASTMAVLINK_PACK(
typedef struct _fmav_traj_req_k1_t {
    uint16_t k;
}) fmav_traj_req_k1_t;


#define FASTMAVLINK_MSG_ID_TRAJ_REQ_K1  51811

#define FASTMAVLINK_MSG_TRAJ_REQ_K1_PAYLOAD_LEN_MAX  2
#define FASTMAVLINK_MSG_TRAJ_REQ_K1_CRCEXTRA  75

#define FASTMAVLINK_MSG_TRAJ_REQ_K1_FLAGS  0
#define FASTMAVLINK_MSG_TRAJ_REQ_K1_TARGET_SYSTEM_OFS  0
#define FASTMAVLINK_MSG_TRAJ_REQ_K1_TARGET_COMPONENT_OFS  0

#define FASTMAVLINK_MSG_TRAJ_REQ_K1_FRAME_LEN_MAX  27



#define FASTMAVLINK_MSG_TRAJ_REQ_K1_FIELD_K_OFS  0


//----------------------------------------
//-- Message TRAJ_REQ_K1 pack,encode routines, for sending
//----------------------------------------

FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_req_k1_pack(
    fmav_message_t* _msg,
    uint8_t sysid,
    uint8_t compid,
    uint16_t k,
    fmav_status_t* _status)
{
    fmav_traj_req_k1_t* _payload = (fmav_traj_req_k1_t*)_msg->payload;

    _payload->k = k;


    _msg->sysid = sysid;
    _msg->compid = compid;
    _msg->msgid = FASTMAVLINK_MSG_ID_TRAJ_REQ_K1;
    _msg->target_sysid = 0;
    _msg->target_compid = 0;
    _msg->crc_extra = FASTMAVLINK_MSG_TRAJ_REQ_K1_CRCEXTRA;
    _msg->payload_max_len = FASTMAVLINK_MSG_TRAJ_REQ_K1_PAYLOAD_LEN_MAX;

    return fmav_finalize_msg(_msg, _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_req_k1_encode(
    fmav_message_t* _msg,
    uint8_t sysid,
    uint8_t compid,
    const fmav_traj_req_k1_t* _payload,
    fmav_status_t* _status)
{
    return fmav_msg_traj_req_k1_pack(
        _msg, sysid, compid,
        _payload->k,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_req_k1_pack_to_frame_buf(
    uint8_t* _buf,
    uint8_t sysid,
    uint8_t compid,
    uint16_t k,
    fmav_status_t* _status)
{
    fmav_traj_req_k1_t* _payload = (fmav_traj_req_k1_t*)(&_buf[FASTMAVLINK_HEADER_V2_LEN]);

    _payload->k = k;


    _buf[5] = sysid;
    _buf[6] = compid;
    _buf[7] = (uint8_t)FASTMAVLINK_MSG_ID_TRAJ_REQ_K1;
    _buf[8] = ((uint32_t)FASTMAVLINK_MSG_ID_TRAJ_REQ_K1 >> 8);
    _buf[9] = ((uint32_t)FASTMAVLINK_MSG_ID_TRAJ_REQ_K1 >> 16);

    return fmav_finalize_frame_buf(
        _buf,
        FASTMAVLINK_MSG_TRAJ_REQ_K1_PAYLOAD_LEN_MAX,
        FASTMAVLINK_MSG_TRAJ_REQ_K1_CRCEXTRA,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_req_k1_encode_to_frame_buf(
    uint8_t* _buf,
    uint8_t sysid,
    uint8_t compid,
    const fmav_traj_req_k1_t* _payload,
    fmav_status_t* _status)
{
    return fmav_msg_traj_req_k1_pack_to_frame_buf(
        _buf, sysid, compid,
        _payload->k,
        _status);
}


#ifdef FASTMAVLINK_SERIAL_WRITE_CHAR

FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_req_k1_pack_to_serial(
    uint8_t sysid,
    uint8_t compid,
    uint16_t k,
    fmav_status_t* _status)
{
    fmav_traj_req_k1_t _payload;

    _payload.k = k;


    return fmav_finalize_serial(
        sysid,
        compid,
        (uint8_t*)&_payload,
        FASTMAVLINK_MSG_ID_TRAJ_REQ_K1,
        FASTMAVLINK_MSG_TRAJ_REQ_K1_PAYLOAD_LEN_MAX,
        FASTMAVLINK_MSG_TRAJ_REQ_K1_CRCEXTRA,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_req_k1_encode_to_serial(
    uint8_t sysid,
    uint8_t compid,
    const fmav_traj_req_k1_t* _payload,
    fmav_status_t* _status)
{
    return fmav_finalize_serial(
        sysid,
        compid,
        (uint8_t*)_payload,
        FASTMAVLINK_MSG_ID_TRAJ_REQ_K1,
        FASTMAVLINK_MSG_TRAJ_REQ_K1_PAYLOAD_LEN_MAX,
        FASTMAVLINK_MSG_TRAJ_REQ_K1_CRCEXTRA,
        _status);
}
#endif


//----------------------------------------
//-- Message TRAJ_REQ_K1 decode routines, for receiving
//----------------------------------------
// For these functions to work correctly, the msg payload must be zero filled.
// Call the helper fmav_msg_zerofill() if needed, or set FASTMAVLINK_ALWAYS_ZEROFILL to 1
// Note that the parse functions do zerofill the msg payload, but that message generator functions
// do not. This means that for the msg obtained from parsing the below functions can safely be used,
// but that this is not so for the msg obtained from pack/encode functions.

FASTMAVLINK_FUNCTION_DECORATOR void fmav_msg_traj_req_k1_decode(fmav_traj_req_k1_t* payload, const fmav_message_t* msg)
{
#if FASTMAVLINK_ALWAYS_ZEROFILL
    if (msg->len < FASTMAVLINK_MSG_TRAJ_REQ_K1_PAYLOAD_LEN_MAX) {
        memcpy(payload, msg->payload, msg->len);
        // ensure that returned payload is zero filled
        memset(&(((uint8_t*)payload)[msg->len]), 0, FASTMAVLINK_MSG_TRAJ_REQ_K1_PAYLOAD_LEN_MAX - msg->len);
    } else {
        // note: msg->len can be larger than PAYLOAD_LEN_MAX if the message has unknown extensions
        memcpy(payload, msg->payload, FASTMAVLINK_MSG_TRAJ_REQ_K1_PAYLOAD_LEN_MAX);
    }
#else
    // this requires that msg payload had been zero filled before
    memcpy(payload, msg->payload, FASTMAVLINK_MSG_TRAJ_REQ_K1_PAYLOAD_LEN_MAX);
#endif
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_req_k1_get_field_k(const fmav_message_t* msg)
{
    uint16_t r;
    memcpy(&r, &(msg->payload[0]), sizeof(uint16_t));
    return r;
}





//----------------------------------------
//-- Pymavlink wrappers
//----------------------------------------
#ifdef FASTMAVLINK_PYMAVLINK_ENABLED

#define MAVLINK_MSG_ID_TRAJ_REQ_K1  51811

#define mavlink_traj_req_k1_t  fmav_traj_req_k1_t

#define MAVLINK_MSG_ID_TRAJ_REQ_K1_LEN  2
#define MAVLINK_MSG_ID_TRAJ_REQ_K1_MIN_LEN  2
#define MAVLINK_MSG_ID_51811_LEN  2
#define MAVLINK_MSG_ID_51811_MIN_LEN  2

#define MAVLINK_MSG_ID_TRAJ_REQ_K1_CRC  75
#define MAVLINK_MSG_ID_51811_CRC  75




#if MAVLINK_COMM_NUM_BUFFERS > 0

FASTMAVLINK_FUNCTION_DECORATOR uint16_t mavlink_msg_traj_req_k1_pack(
    uint8_t sysid,
    uint8_t compid,
    mavlink_message_t* _msg,
    uint16_t k)
{
    fmav_status_t* _status = mavlink_get_channel_status(MAVLINK_COMM_0);
    return fmav_msg_traj_req_k1_pack(
        _msg, sysid, compid,
        k,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t mavlink_msg_traj_req_k1_encode(
    uint8_t sysid,
    uint8_t compid,
    mavlink_message_t* _msg,
    const mavlink_traj_req_k1_t* _payload)
{
    return mavlink_msg_traj_req_k1_pack(
        sysid,
        compid,
        _msg,
        _payload->k);
}

#endif


FASTMAVLINK_FUNCTION_DECORATOR uint16_t mavlink_msg_traj_req_k1_pack_txbuf(
    char* _buf,
    fmav_status_t* _status,
    uint8_t sysid,
    uint8_t compid,
    uint16_t k)
{
    return fmav_msg_traj_req_k1_pack_to_frame_buf(
        (uint8_t*)_buf,
        sysid,
        compid,
        k,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR void mavlink_msg_traj_req_k1_decode(const mavlink_message_t* msg, mavlink_traj_req_k1_t* payload)
{
    fmav_msg_traj_req_k1_decode(payload, msg);
}

#endif // FASTMAVLINK_PYMAVLINK_ENABLED


#endif // FASTMAVLINK_MSG_TRAJ_REQ_K1_H
