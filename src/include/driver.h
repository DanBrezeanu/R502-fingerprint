#ifndef __DRIVER_H__
#define __DRIVER_H__

#include <libserialport.h>
#include <stdint.h>
#include <error_codes.h>
#include <stdlib.h>
#include <stdarg.h>

#define DEFAULT_BAUD_RATE 57600
#define DEVICE_ADDRESS_SIZE 4

typedef struct Driver {
    uint32_t address;
    struct sp_port *sp;
    int8_t *cmd_buf;
    int32_t cmd_buf_len;
    int8_t recv_buf[1024];
} Driver;

int32_t init_driver(int8_t *port_name, int32_t address, Driver **driver);

#endif