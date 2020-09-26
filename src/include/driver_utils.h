#ifndef __DRIVER_UTILS_H__
#define __DRIVER_UTILS_H__

#include <stdlib.h>
#include <stdint.h>
#include <commands.h>

uint8_t *to_bytes_MSB(void *x, size_t size);
uint8_t *to_bytes_LSB(void *x, size_t size);
uint16_t checksum(uint8_t *data, int32_t start, int32_t end);
uint32_t get_command_pkg_len(CommandType type);

#endif