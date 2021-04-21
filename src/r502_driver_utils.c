#include <r502_driver_utils.h>
#include <r502_commands.h>
#include <r502_reply.h>

uint8_t *to_bytes_MSB(void *x, size_t size) {
    uint8_t *bytes = calloc(size, 1);

    for (int32_t i = size - 1; i >= 0; --i) {
        bytes[size - 1 - i] = *((uint8_t *) x + i);
    }

    return bytes;
}

uint8_t *to_bytes_LSB(void *x, size_t size) {
    uint8_t *bytes = calloc(size, 1);

    for (int32_t i = 0; i < size; ++i) {
        bytes[i] = *((uint8_t *) x + i);
    }

    return bytes;
}

uint32_t from_bytes_MSB(uint8_t *data, int32_t start, int32_t end) {
    uint32_t num = 0;

    for (int32_t i = end - 1; i >= start; --i)
        num += (data[i] << (8 * (end - 1 - i)));

    return num;
}

uint16_t checksum(uint8_t *data, int32_t start, int32_t end) {
    uint16_t chksum = 0;

    for (int32_t i = start; i < end; ++i) {
        chksum += (uint16_t) data[i];
    }

    return chksum;
}

uint8_t check_checksum(uint8_t *data, uint32_t len) {
    uint16_t chksum_to_check = 0;
    uint16_t chksum = 0;

    chksum_to_check = data[len - 1] + (data[len - 2] << 8);

    for (int32_t i = 6; i < len - 2; ++i)
        chksum += data[i];

    return (chksum_to_check == chksum);
}

uint32_t get_command_pkg_len(CommandType type) {
    uint32_t len = 0;

    switch (type) {
    case ReadSysPara:
        len = READ_SYS_PARA_LEN;
        break;
    case VfyPwd:
        len = VFY_PWD_LEN;
        break;
    case GenImg:
        len = GEN_IMG_LEN;
        break;
    case Img2Tz:
        len = IMG2TZ_LEN;
        break;
    case Search:
        len = SEARCH_LEN;
        break;
    case LoadChar:
        len = LOAD_CHAR_LEN;
        break;
    case Match:
        len = MATCH_LEN;
        break;
    case TemplateNum:
        len = TEMPLATE_NUM_LEN;
        break;
    case RegModel:
        len = REG_MODEL_LEN;
        break;
    case Store:
        len = STORE_LEN;
        break;
    case DeleteChar:
        len = DELETE_CHAR_LEN;
        break;
    case UpChar:
        len = UP_CHAR_LEN;
        break;    
    case WriteNotepad:
        len = WRITE_NOTEPAD_LEN;
        break;
    case ReadNotepad:
        len = READ_NOTEPAD_LEN;
        break;
    case AuraLedConfig:
        len = AURA_LED_CONFIG_LEN;
        break;
    case SetPwd:
        len = SET_PWD_LEN;
        break;
    case SetAddr:
        len = SET_ADDR_LEN;
        break;
    case HandShake:
        len = HANDSHAKE_LEN;
        break;
    }

    return len;
}

uint32_t get_command_reply_len(CommandType type) {
    uint32_t len = 0;

    switch (type) {
    case ReadSysPara:
        len = READ_SYS_PARA_REPLY_LEN;
        break;
    case VfyPwd:
        len = VFY_PWD_REPLY_LEN;
        break;
    case GenImg:
        len = GEN_IMG_REPLY_LEN;
        break;
    case Img2Tz:
        len = IMG2TZ_REPLY_LEN;
        break;
    case Search:
        len = SEARCH_REPLY_LEN;
        break;
    case LoadChar:
        len = LOAD_CHAR_REPLY_LEN;
        break;
    case Match:
        len = MATCH_REPLY_LEN;
        break;
    case TemplateNum:
        len = TEMPLATE_NUM_REPLY_LEN;
        break;
    case RegModel:
        len = REG_MODEL_REPLY_LEN;
        break;
    case Store:
        len = STORE_REPLY_LEN;
        break;
    case DeleteChar:
        len = DELETE_CHAR_REPLY_LEN;
        break;
    case UpChar:
        len = UP_CHAR_REPLY_LEN;
        break;
    case WriteNotepad:
        len = WRITE_NOTEPAD_REPLY_LEN;
        break;
    case ReadNotepad:
        len = READ_NOTEPAD_REPLY_LEN;
        break;
    case AuraLedConfig:
        len = AURA_LED_CONFIG_REPLY_LEN;
        break;
    case SetPwd:
        len = SET_PWD_REPLY_LEN;
        break;
    case SetAddr:
        len = SET_ADDR_REPLY_LEN;
        break;
    case HandShake:
        len = HANDSHAKE_REPLY_LEN;
        break;
    }

    return len;
}

uint8_t cmd_has_additional_packages(CommandType type) {
    switch (type) {
    case UpChar:
        return 1;
    default:
        return 0;
    }

    return 0;
}

uint32_t get_command_additional_reply_len(CommandType type) {
    switch (type) {
    case UpChar:
        return UP_CHAR_ADDITIONAL_REPLY_LEN;
    default:
        return 0;
    }

    return 0;
}