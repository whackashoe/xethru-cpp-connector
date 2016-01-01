#ifndef XETHRU_CONST_H
#define XETHRU_CONST_H

#include <cstdint>

namespace xethru {

// Constants for the XeThru Serial Protocol

// Device Specific
constexpr uint32_t XETHRU_BAUD = 115200;

// Protocol flags
constexpr uint8_t XETHRU_START = 0x7d;
constexpr uint8_t XETHRU_END = 0x7e;
constexpr uint8_t XETHRU_ESC = 0x7f;

// Identifiers
constexpr uint32_t XTS_ID_APP_RESP = 0x1423a2d6;
constexpr uint32_t XTS_ID_APP_PRESENCE = 0x00288912;
constexpr uint32_t XTS_ID_RESP_STATUS = 0x2375fe26;
constexpr uint32_t XTS_ID_PRESENCE_STATUS = 0x991a52be;
constexpr uint32_t XTS_ID_DETECTION_ZONE = 0x96a10a1c;

// Command codes
constexpr uint8_t XTS_SPC_APPCOMMAND = 0x10;
constexpr uint8_t XTS_SPC_MOD_SETMODE = 0x20;
constexpr uint8_t XTS_SPC_MOD_LOADAPP = 0x21;
constexpr uint8_t XTS_SPC_MOD_RESET = 0x22;

constexpr uint8_t XTS_SPC_MOD_SETLEDCONTROL = 0x24;

constexpr uint8_t XTS_SPCA_SET = 0x10;

constexpr uint8_t XT_UI_LED_MODE_OFF = 0;
constexpr uint8_t XT_UI_LED_MODE_SIMPLE = 1;
constexpr uint8_t XT_UI_LED_MODE_FULL = 2;

constexpr uint8_t XTS_SM_RUN = 0x01;
constexpr uint8_t XTS_SM_IDLE = 0x11;

// Response codes
constexpr uint8_t XTS_SPR_ACK = 0x10;
constexpr uint8_t XTS_SPR_ERROR = 0x20;
constexpr uint8_t XTS_SPR_SYSTEM = 0x30;
constexpr uint8_t XTS_SPR_APPDATA = 0x50;
constexpr uint8_t XTS_SPRS_BOOTING = 0x10;
constexpr uint8_t XTS_SPRS_READY = 0x11;

// Range limits
constexpr float XETHRU_RESP_MIN = 0.5f;
constexpr float XETHRU_RESP_MAX = 2.5f;
constexpr float XETHRU_RESP_SPAN_MIN = 0.2f;
constexpr float XETHRU_RESP_SPAN_MAX = 0.7f;

constexpr float XETHRU_PRES_MIN = 0.5f;
constexpr float XETHRU_PRES_MAX = 4.5f;
constexpr float XETHRU_PRES_SPAN_MIN = 0.8f;
constexpr float XETHRU_PRES_SPAN_MAX = 2.8f;

// Presence states
constexpr uint8_t XETHRU_PRES_NODETECT = 0;
constexpr uint8_t XETHRU_PRES_DETECTED = 1;
constexpr uint8_t XETHRU_PRES_UNKNOWN  = 1;

// Respiration application codes
constexpr uint8_t XTS_VAL_RESP_STATE_BREATHING = 0;
constexpr uint8_t XTS_VAL_RESP_STATE_MOVEMENT = 1;
constexpr uint8_t XTS_VAL_RESP_STATE_MOVEMENT_TRACKING = 2;
constexpr uint8_t XTS_VAL_RESP_STATE_NO_MOVEMENT = 3;
constexpr uint8_t XTS_VAL_RESP_STATE_INTIALIZING = 4;
constexpr uint8_t XTS_VAL_RESP_STATE_RESERVED = 5;
constexpr uint8_t XTS_VAL_RESP_STATE_UNKNOWN = 6;

// Verbosity level
enum verbosity {
    NONE   = 1 << 0
  , ERR    = 1 << 1
  , WARN   = 1 << 2
  , STATUS = 1 << 3
  , DEBUG  = 1 << 4
};

}

#endif

