//------------------------------
// The fastMavlink library
// (c) OlliW, OlliW42, www.olliw.eu
//------------------------------

#pragma once
#ifndef FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_H
#define FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_H


//----------------------------------------
//-- Message TRAJ_LOAD_SD_CARD
//----------------------------------------

// fields are ordered, as they appear on the wire
FASTMAVLINK_PACK(
typedef struct _fmav_traj_load_sd_card_t {
    char filename[63];
}) fmav_traj_load_sd_card_t;


#define FASTMAVLINK_MSG_ID_TRAJ_LOAD_SD_CARD  51809

#define FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_PAYLOAD_LEN_MAX  63
#define FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_CRCEXTRA  220

#define FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_FLAGS  0
#define FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_TARGET_SYSTEM_OFS  0
#define FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_TARGET_COMPONENT_OFS  0

#define FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_FRAME_LEN_MAX  88

#define FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_FIELD_FILENAME_NUM  63 // number of elements in array
#define FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_FIELD_FILENAME_LEN  63 // length of array = number of bytes

#define FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_FIELD_FILENAME_OFS  0


//----------------------------------------
//-- Message TRAJ_LOAD_SD_CARD pack,encode routines, for sending
//----------------------------------------

FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_load_sd_card_pack(
    fmav_message_t* _msg,
    uint8_t sysid,
    uint8_t compid,
    const char* filename,
    fmav_status_t* _status)
{
    fmav_traj_load_sd_card_t* _payload = (fmav_traj_load_sd_card_t*)_msg->payload;


    memcpy(&(_payload->filename), filename, sizeof(char)*63);

    _msg->sysid = sysid;
    _msg->compid = compid;
    _msg->msgid = FASTMAVLINK_MSG_ID_TRAJ_LOAD_SD_CARD;
    _msg->target_sysid = 0;
    _msg->target_compid = 0;
    _msg->crc_extra = FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_CRCEXTRA;
    _msg->payload_max_len = FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_PAYLOAD_LEN_MAX;

    return fmav_finalize_msg(_msg, _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_load_sd_card_encode(
    fmav_message_t* _msg,
    uint8_t sysid,
    uint8_t compid,
    const fmav_traj_load_sd_card_t* _payload,
    fmav_status_t* _status)
{
    return fmav_msg_traj_load_sd_card_pack(
        _msg, sysid, compid,
        _payload->filename,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_load_sd_card_pack_to_frame_buf(
    uint8_t* _buf,
    uint8_t sysid,
    uint8_t compid,
    const char* filename,
    fmav_status_t* _status)
{
    fmav_traj_load_sd_card_t* _payload = (fmav_traj_load_sd_card_t*)(&_buf[FASTMAVLINK_HEADER_V2_LEN]);


    memcpy(&(_payload->filename), filename, sizeof(char)*63);

    _buf[5] = sysid;
    _buf[6] = compid;
    _buf[7] = (uint8_t)FASTMAVLINK_MSG_ID_TRAJ_LOAD_SD_CARD;
    _buf[8] = ((uint32_t)FASTMAVLINK_MSG_ID_TRAJ_LOAD_SD_CARD >> 8);
    _buf[9] = ((uint32_t)FASTMAVLINK_MSG_ID_TRAJ_LOAD_SD_CARD >> 16);

    return fmav_finalize_frame_buf(
        _buf,
        FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_PAYLOAD_LEN_MAX,
        FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_CRCEXTRA,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_load_sd_card_encode_to_frame_buf(
    uint8_t* _buf,
    uint8_t sysid,
    uint8_t compid,
    const fmav_traj_load_sd_card_t* _payload,
    fmav_status_t* _status)
{
    return fmav_msg_traj_load_sd_card_pack_to_frame_buf(
        _buf, sysid, compid,
        _payload->filename,
        _status);
}


#ifdef FASTMAVLINK_SERIAL_WRITE_CHAR

FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_load_sd_card_pack_to_serial(
    uint8_t sysid,
    uint8_t compid,
    const char* filename,
    fmav_status_t* _status)
{
    fmav_traj_load_sd_card_t _payload;


    memcpy(&(_payload.filename), filename, sizeof(char)*63);

    return fmav_finalize_serial(
        sysid,
        compid,
        (uint8_t*)&_payload,
        FASTMAVLINK_MSG_ID_TRAJ_LOAD_SD_CARD,
        FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_PAYLOAD_LEN_MAX,
        FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_CRCEXTRA,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t fmav_msg_traj_load_sd_card_encode_to_serial(
    uint8_t sysid,
    uint8_t compid,
    const fmav_traj_load_sd_card_t* _payload,
    fmav_status_t* _status)
{
    return fmav_finalize_serial(
        sysid,
        compid,
        (uint8_t*)_payload,
        FASTMAVLINK_MSG_ID_TRAJ_LOAD_SD_CARD,
        FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_PAYLOAD_LEN_MAX,
        FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_CRCEXTRA,
        _status);
}
#endif


//----------------------------------------
//-- Message TRAJ_LOAD_SD_CARD decode routines, for receiving
//----------------------------------------
// For these functions to work correctly, the msg payload must be zero filled.
// Call the helper fmav_msg_zerofill() if needed, or set FASTMAVLINK_ALWAYS_ZEROFILL to 1
// Note that the parse functions do zerofill the msg payload, but that message generator functions
// do not. This means that for the msg obtained from parsing the below functions can safely be used,
// but that this is not so for the msg obtained from pack/encode functions.

FASTMAVLINK_FUNCTION_DECORATOR void fmav_msg_traj_load_sd_card_decode(fmav_traj_load_sd_card_t* payload, const fmav_message_t* msg)
{
#if FASTMAVLINK_ALWAYS_ZEROFILL
    if (msg->len < FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_PAYLOAD_LEN_MAX) {
        memcpy(payload, msg->payload, msg->len);
        // ensure that returned payload is zero filled
        memset(&(((uint8_t*)payload)[msg->len]), 0, FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_PAYLOAD_LEN_MAX - msg->len);
    } else {
        // note: msg->len can be larger than PAYLOAD_LEN_MAX if the message has unknown extensions
        memcpy(payload, msg->payload, FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_PAYLOAD_LEN_MAX);
    }
#else
    // this requires that msg payload had been zero filled before
    memcpy(payload, msg->payload, FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_PAYLOAD_LEN_MAX);
#endif
}





FASTMAVLINK_FUNCTION_DECORATOR char* fmav_msg_traj_load_sd_card_get_field_filename_ptr(const fmav_message_t* msg)
{
    return (char*)&(msg->payload[0]);
}


FASTMAVLINK_FUNCTION_DECORATOR char fmav_msg_traj_load_sd_card_get_field_filename(uint16_t index, const fmav_message_t* msg)
{
    if (index >= FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_FIELD_FILENAME_NUM) return 0;
    return ((char*)&(msg->payload[0]))[index];
}


//----------------------------------------
//-- Pymavlink wrappers
//----------------------------------------
#ifdef FASTMAVLINK_PYMAVLINK_ENABLED

#define MAVLINK_MSG_ID_TRAJ_LOAD_SD_CARD  51809

#define mavlink_traj_load_sd_card_t  fmav_traj_load_sd_card_t

#define MAVLINK_MSG_ID_TRAJ_LOAD_SD_CARD_LEN  63
#define MAVLINK_MSG_ID_TRAJ_LOAD_SD_CARD_MIN_LEN  63
#define MAVLINK_MSG_ID_51809_LEN  63
#define MAVLINK_MSG_ID_51809_MIN_LEN  63

#define MAVLINK_MSG_ID_TRAJ_LOAD_SD_CARD_CRC  220
#define MAVLINK_MSG_ID_51809_CRC  220

#define MAVLINK_MSG_TRAJ_LOAD_SD_CARD_FIELD_FILENAME_LEN 63


#if MAVLINK_COMM_NUM_BUFFERS > 0

FASTMAVLINK_FUNCTION_DECORATOR uint16_t mavlink_msg_traj_load_sd_card_pack(
    uint8_t sysid,
    uint8_t compid,
    mavlink_message_t* _msg,
    const char* filename)
{
    fmav_status_t* _status = mavlink_get_channel_status(MAVLINK_COMM_0);
    return fmav_msg_traj_load_sd_card_pack(
        _msg, sysid, compid,
        filename,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR uint16_t mavlink_msg_traj_load_sd_card_encode(
    uint8_t sysid,
    uint8_t compid,
    mavlink_message_t* _msg,
    const mavlink_traj_load_sd_card_t* _payload)
{
    return mavlink_msg_traj_load_sd_card_pack(
        sysid,
        compid,
        _msg,
        _payload->filename);
}

#endif


FASTMAVLINK_FUNCTION_DECORATOR uint16_t mavlink_msg_traj_load_sd_card_pack_txbuf(
    char* _buf,
    fmav_status_t* _status,
    uint8_t sysid,
    uint8_t compid,
    const char* filename)
{
    return fmav_msg_traj_load_sd_card_pack_to_frame_buf(
        (uint8_t*)_buf,
        sysid,
        compid,
        filename,
        _status);
}


FASTMAVLINK_FUNCTION_DECORATOR void mavlink_msg_traj_load_sd_card_decode(const mavlink_message_t* msg, mavlink_traj_load_sd_card_t* payload)
{
    fmav_msg_traj_load_sd_card_decode(payload, msg);
}

#endif // FASTMAVLINK_PYMAVLINK_ENABLED


#endif // FASTMAVLINK_MSG_TRAJ_LOAD_SD_CARD_H
