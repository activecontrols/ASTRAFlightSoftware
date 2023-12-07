//------------------------------
// The fastMavlink library
// (c) OlliW, OlliW42, www.olliw.eu
//------------------------------

#pragma once
#ifndef FASTMAVLINK_MSG_TRAJ_INFO_H
#define FASTMAVLINK_MSG_TRAJ_INFO_H


//----------------------------------------
//-- Message TRAJ_INFO
//----------------------------------------

// fields are ordered, as they appear on the wire
FASTMAVLINK_PACK(
typedef struct _fmav_traj_info_t {
    uint32_t n;
    uint16_t k;
    uint16_t k_dim1;
    uint16_t k_dim2;
}) fmav_traj_info_t;


#define FASTMAVLINK_MSG_ID_TRAJ_INFO  51810

#define FASTMAVLINK_MSG_TRAJ_INFO_PAYLOAD_LEN_MAX  10
#define FASTMAVLINK_MSG_TRAJ_INFO_CRCEXTRA  227

#define FASTMAVLINK_MSG_TRAJ_INFO_FLAGS  0
#define FASTMAVLINK_MSG_TRAJ_INFO_TARGET_SYSTEM_OFS  0
#define FASTMAVLINK_MSG_TRAJ_INFO_TARGET_COMPONENT_OFS  0

#define FASTMAVLINK_MSG_TRAJ_INFO_FRAME_LEN_MAX  35



#define FASTMAVLINK_MSG_TRAJ_INFO_FIELD_N_OFS  0
#define FASTMAVLINK_MSG_TRAJ_INFO_FIELD_K_OFS  4
#define FASTMAVLINK_MSG_TRAJ_INFO_FIELD_K_DIM1_OFS  6
#define FASTMAVLINK_MSG_TRAJ_INFO_FIELD_K_DIM2_OFS  8


//----------------------------------------
//-- Message TRAJ_INFO pack,encode routines, for sending
//----------------------------------------

FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_info_pack(
    fmav_message_t* _msg,
    uint8_t sysid,
    uint8_t compid,
    uint16_t k, uint32_t n, uint16_t k_dim1, uint16_t k_dim2,
    fmav_status_t* _status)
{
    fmav_traj_info_t* _payload = (fmav_traj_info_t*)_msg->payload;

    _payload->n = n;
    _payload->k = k;
    _payload->k_dim1 = k_dim1;
    _payload->k_dim2 = k_dim2;


    _msg->sysid = sysid;
    _msg->compid = compid;
    _msg->msgid = FASTMAVLINK_MSG_ID_TRAJ_INFO;
    _msg->target_sysid = 0;
    _msg->target_compid = 0;
    _msg->crc_extra = FASTMAVLINK_MSG_TRAJ_INFO_CRCEXTRA;
    _msg->payload_max_len = FASTMAVLINK_MSG_TRAJ_INFO_PAYLOAD_LEN_MAX;

    return fmav_finalize_msg(_msg, _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_info_encode(
    fmav_message_t* _msg,
    uint8_t sysid,
    uint8_t compid,
    const fmav_traj_info_t* _payload,
    fmav_status_t* _status)
{
    return fmav_msg_traj_info_pack(
        _msg, sysid, compid,
        _payload->k, _payload->n, _payload->k_dim1, _payload->k_dim2,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_info_pack_to_frame_buf(
    uint8_t* _buf,
    uint8_t sysid,
    uint8_t compid,
    uint16_t k, uint32_t n, uint16_t k_dim1, uint16_t k_dim2,
    fmav_status_t* _status)
{
    fmav_traj_info_t* _payload = (fmav_traj_info_t*)(&_buf[FASTMAVLINK_HEADER_V2_LEN]);

    _payload->n = n;
    _payload->k = k;
    _payload->k_dim1 = k_dim1;
    _payload->k_dim2 = k_dim2;


    _buf[5] = sysid;
    _buf[6] = compid;
    _buf[7] = (uint8_t)FASTMAVLINK_MSG_ID_TRAJ_INFO;
    _buf[8] = ((uint32_t)FASTMAVLINK_MSG_ID_TRAJ_INFO >> 8);
    _buf[9] = ((uint32_t)FASTMAVLINK_MSG_ID_TRAJ_INFO >> 16);

    return fmav_finalize_frame_buf(
        _buf,
        FASTMAVLINK_MSG_TRAJ_INFO_PAYLOAD_LEN_MAX,
        FASTMAVLINK_MSG_TRAJ_INFO_CRCEXTRA,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_info_encode_to_frame_buf(
    uint8_t* _buf,
    uint8_t sysid,
    uint8_t compid,
    const fmav_traj_info_t* _payload,
    fmav_status_t* _status)
{
    return fmav_msg_traj_info_pack_to_frame_buf(
        _buf, sysid, compid,
        _payload->k, _payload->n, _payload->k_dim1, _payload->k_dim2,
        _status);
}


#ifdef FASTMAVLINK_SERIAL_WRITE_CHAR

FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_info_pack_to_serial(
    uint8_t sysid,
    uint8_t compid,
    uint16_t k, uint32_t n, uint16_t k_dim1, uint16_t k_dim2,
    fmav_status_t* _status)
{
    fmav_traj_info_t _payload;

    _payload.n = n;
    _payload.k = k;
    _payload.k_dim1 = k_dim1;
    _payload.k_dim2 = k_dim2;


    return fmav_finalize_serial(
        sysid,
        compid,
        (uint8_t*)&_payload,
        FASTMAVLINK_MSG_ID_TRAJ_INFO,
        FASTMAVLINK_MSG_TRAJ_INFO_PAYLOAD_LEN_MAX,
        FASTMAVLINK_MSG_TRAJ_INFO_CRCEXTRA,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_info_encode_to_serial(
    uint8_t sysid,
    uint8_t compid,
    const fmav_traj_info_t* _payload,
    fmav_status_t* _status)
{
    return fmav_finalize_serial(
        sysid,
        compid,
        (uint8_t*)_payload,
        FASTMAVLINK_MSG_ID_TRAJ_INFO,
        FASTMAVLINK_MSG_TRAJ_INFO_PAYLOAD_LEN_MAX,
        FASTMAVLINK_MSG_TRAJ_INFO_CRCEXTRA,
        _status);
}
#endif


//----------------------------------------
//-- Message TRAJ_INFO decode routines, for receiving
//----------------------------------------
// For these functions to work correctly, the msg payload must be zero filled.
// Call the helper fmav_msg_zerofill() if needed, or set FASTMAVLINK_ALWAYS_ZEROFILL to 1
// Note that the parse functions do zerofill the msg payload, but that message generator functions
// do not. This means that for the msg obtained from parsing the below functions can safely be used,
// but that this is not so for the msg obtained from pack/encode functions.

FASTMAVLINK_FUNCTION_DECORATOR void fmav_msg_traj_info_decode(fmav_traj_info_t* payload, const fmav_message_t* msg)
{
#if FASTMAVLINK_ALWAYS_ZEROFILL
    if (msg->len < FASTMAVLINK_MSG_TRAJ_INFO_PAYLOAD_LEN_MAX) {
        memcpy(payload, msg->payload, msg->len);
        // ensure that returned payload is zero filled
        memset(&(((uint8_t*)payload)[msg->len]), 0, FASTMAVLINK_MSG_TRAJ_INFO_PAYLOAD_LEN_MAX - msg->len);
    } else {
        // note: msg->len can be larger than PAYLOAD_LEN_MAX if the message has unknown extensions
        memcpy(payload, msg->payload, FASTMAVLINK_MSG_TRAJ_INFO_PAYLOAD_LEN_MAX);
    }
#else
    // this requires that msg payload had been zero filled before
    memcpy(payload, msg->payload, FASTMAVLINK_MSG_TRAJ_INFO_PAYLOAD_LEN_MAX);
#endif
}


FASTMAVLINK_FUNCTION_DECORATOR uint32_t fmav_msg_traj_info_get_field_n(const fmav_message_t* msg)
{
    uint32_t r;
    memcpy(&r, &(msg->payload[0]), sizeof(uint32_t));
    return r;
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_info_get_field_k(const fmav_message_t* msg)
{
    uint16_t r;
    memcpy(&r, &(msg->payload[4]), sizeof(uint16_t));
    return r;
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_info_get_field_k_dim1(const fmav_message_t* msg)
{
    uint16_t r;
    memcpy(&r, &(msg->payload[6]), sizeof(uint16_t));
    return r;
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_info_get_field_k_dim2(const fmav_message_t* msg)
{
    uint16_t r;
    memcpy(&r, &(msg->payload[8]), sizeof(uint16_t));
    return r;
}





//----------------------------------------
//-- Pymavlink wrappers
//----------------------------------------
#ifdef FASTMAVLINK_PYMAVLINK_ENABLED

#define MAVLINK_MSG_ID_TRAJ_INFO  51810

#define mavlink_traj_info_t  fmav_traj_info_t

#define MAVLINK_MSG_ID_TRAJ_INFO_LEN  10
#define MAVLINK_MSG_ID_TRAJ_INFO_MIN_LEN  10
#define MAVLINK_MSG_ID_51810_LEN  10
#define MAVLINK_MSG_ID_51810_MIN_LEN  10

#define MAVLINK_MSG_ID_TRAJ_INFO_CRC  227
#define MAVLINK_MSG_ID_51810_CRC  227




#if MAVLINK_COMM_NUM_BUFFERS > 0

FASTMAVLINK_FUNCTION_DECORATOR uint16_t mavlink_msg_traj_info_pack(
    uint8_t sysid,
    uint8_t compid,
    mavlink_message_t* _msg,
    uint16_t k, uint32_t n, uint16_t k_dim1, uint16_t k_dim2)
{
    fmav_status_t* _status = mavlink_get_channel_status(MAVLINK_COMM_0);
    return fmav_msg_traj_info_pack(
        _msg, sysid, compid,
        k, n, k_dim1, k_dim2,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t mavlink_msg_traj_info_encode(
    uint8_t sysid,
    uint8_t compid,
    mavlink_message_t* _msg,
    const mavlink_traj_info_t* _payload)
{
    return mavlink_msg_traj_info_pack(
        sysid,
        compid,
        _msg,
        _payload->k, _payload->n, _payload->k_dim1, _payload->k_dim2);
}

#endif


FASTMAVLINK_FUNCTION_DECORATOR uint16_t mavlink_msg_traj_info_pack_txbuf(
    char* _buf,
    fmav_status_t* _status,
    uint8_t sysid,
    uint8_t compid,
    uint16_t k, uint32_t n, uint16_t k_dim1, uint16_t k_dim2)
{
    return fmav_msg_traj_info_pack_to_frame_buf(
        (uint8_t*)_buf,
        sysid,
        compid,
        k, n, k_dim1, k_dim2,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR void mavlink_msg_traj_info_decode(const mavlink_message_t* msg, mavlink_traj_info_t* payload)
{
    fmav_msg_traj_info_decode(payload, msg);
}

#endif // FASTMAVLINK_PYMAVLINK_ENABLED


#endif // FASTMAVLINK_MSG_TRAJ_INFO_H
