#ifndef __R502_DRIVER_UTILS_H__
#define __R502_DRIVER_UTILS_H__

#include <r502_types.h>

/* Get an array of bytes from a number `x` of size `size` (MSB first) */
uint8_t *to_bytes_MSB(void *x, size_t size);

/* Get an array of bytes from a number `x` of size `size` (LSB first) */
uint8_t *to_bytes_LSB(void *x, size_t size);

/* Get a 32-bit number from the bytes `data` [start-end) */
uint32_t from_bytes_MSB(uint8_t *data, int32_t start, int32_t end);

/* Compute the 2-byte checksum on `data` from `start` to `end` */
uint16_t checksum(uint8_t *data, int32_t start, int32_t end);

/* Check whether the checksum provided at the end is correct */
uint8_t check_checksum(uint8_t *data, uint32_t len);

/* Get package length based of `type` */
uint32_t get_command_pkg_len(CommandType type);

/* Get reply length based of `type` */
uint32_t get_command_reply_len(CommandType type);

/* Returns a non-zero value if command of type `type` needs to
   to receive additional packages */
uint8_t cmd_has_additional_packages(CommandType type);

/* Get additional reply length of `type` */
uint32_t get_command_additional_reply_len(CommandType type);

#endif
