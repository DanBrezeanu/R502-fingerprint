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

/*!
  * Initialize the driver with the necessary information for communication

  @param[in]    port_name     Name of the serial device
  @param[in]    address       Address of the device (default: 0xFFFFFFFF)
  @param[out]   driver        The to be initialised driver
  @param[out]   err           Error code
*/
int32_t init_driver(uint8_t *port_name, int32_t address, Driver **driver);

/*!
  * Frees the memory allocated for the driver

  @param[in]    driver        Driver to be freed
  @param[out]   err           Error code
*/
int32_t destroy_driver(Driver *driver);

/*!
  * Call command of type `type`. Driver must be initialized and reply will be
  * stored in `reply`

  * Command specific arguments must be provided in order (according to r502_commands.h)

  @param[in]    driver      The initialized driver
  @param[in]    type        The command to be called
  @param[in]    arg_num     Number of command specific arguments
  @param[in]    ...         The arguments provided in the required order
  @param[out]   reply       Reply structure
  @param[out]   err         Error code
*/
int32_t call_cmd(Driver *driver, CommandType type, Reply *reply, int32_t arg_num, ...);

#endif
