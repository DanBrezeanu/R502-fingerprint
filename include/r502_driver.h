#ifndef __R502_DRIVER_H__
#define __R502_DRIVER_H__

#include <libserialport.h>
#include <r502_types.h>
#include <r502_commands.h>
#include <r502_driver_utils.h>
#include <r502_reply.h>
#include <stdarg.h>

#define DEFAULT_BAUD_RATE   57600
#define DEVICE_ADDRESS_SIZE 4
#define TIMEOUT             5000


int32_t init_driver(uint8_t *port_name, int32_t address, Driver **driver);
int32_t call_cmd(Driver *driver, CommandType type, Reply *reply, int32_t arg_num, ...);

#endif