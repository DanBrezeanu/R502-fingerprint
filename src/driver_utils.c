#include <driver_utils.h>
#include <commands.h>

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

uint16_t checksum(uint8_t *data, int32_t start, int32_t end) {
    uint16_t chksum = 0;

    for (int32_t i = start; i < end; ++i) {
        chksum += (uint16_t) data[i];
    }

    return chksum;
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
    }

    return len;
}