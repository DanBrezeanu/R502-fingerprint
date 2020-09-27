#ifndef __R502_DRIVER_UTILS_H__
#define __R502_DRIVER_UTILS_H__

#include <r502_types.h>

uint8_t *to_bytes_MSB(void *x, size_t size);
uint8_t *to_bytes_LSB(void *x, size_t size);
uint32_t from_bytes_MSB(uint8_t *data, int32_t start, int32_t end);
uint16_t checksum(uint8_t *data, int32_t start, int32_t end);
uint8_t check_checksum(uint8_t *data, uint32_t len);
uint32_t get_command_pkg_len(CommandType type);
uint32_t get_command_reply_len(CommandType type);

#endif