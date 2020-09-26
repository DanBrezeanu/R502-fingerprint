#include <driver.h>
#include <commands.h>

int32_t init_driver(int8_t *port_name, int32_t address, Driver **driver) {
    if (port_name == NULL)
        return INIT_ARG;

    struct sp_port *port = NULL;
    enum sp_return result;

    result = sp_get_port_by_name(port_name, &port);
    if (result != SP_OK)
        return INIT_FAIL;

    result = sp_open(port, SP_MODE_READ_WRITE);
    if (result != SP_OK)
        goto error;

    result = sp_set_baudrate(port, DEFAULT_BAUD_RATE);
    if (result != SP_OK)
        goto error;

    result = sp_set_bits(port, 8);
    if (result != SP_OK)
        goto error;

    result = sp_set_parity(port, SP_PARITY_NONE);
    if (result != SP_OK)
        goto error;

    result = sp_set_stopbits(port, 1);
    if (result != SP_OK)
        goto error;

    result = sp_set_flowcontrol(port, SP_FLOWCONTROL_NONE);
    if (result != SP_OK)
        goto error;

    *driver = calloc(1, sizeof(Driver));
    (*driver)->address = address;
    (*driver)->sp = port;

    return SUCCESS;

error:
    sp_free_port(port);

    return INIT_FAIL;    
}

#include <stdio.h>

int32_t call_cmd(Driver *driver, CommandType type, int32_t arg_num, ...) {
    Command cmd = {0};
    int32_t err = SUCCESS;
    int32_t result;
    va_list ap;

    cmd.command_type = type;

    va_start(ap, arg_num);
    err = get_command_package(driver, cmd, arg_num, ap);
    va_end(ap);

    result = sp_blocking_write(driver->sp, driver->cmd_buf, driver->cmd_buf_len, 5000);
 
    /* Check whether we sent all of the data. */
    if (result == driver->cmd_buf_len)
            printf("Sent %d bytes successfully.\n", result);
    else
            printf("Timed out, %d/%d bytes sent.\n", result, driver->cmd_buf_len);

    /* Allocate a buffer to receive data. */
    char *buf = calloc(30, 1);

    /* Try to receive the data on the other port. */
    printf("Receiving %d bytes on port %s.\n",
                    12, sp_get_port_name(driver->sp));
    result = sp_blocking_read(driver->sp, buf, 12, 5000);

    /* Check whether we received the number of bytes we wanted. */
    if (result == 12)
            printf("Received %d bytes successfully.\n", 12);
    else
            printf("Timed out, %d/%d bytes received.\n", result, 12);

    /* Check if we received the same data we sent. */
    buf[result] = '\0';
    
    for (int i = 0; i < 12; ++i)
        printf("%.2X ", buf[i]);
    printf("\n");

    return SUCCESS;
}

int main() {
    Driver *driver;

    init_driver("/dev/ttyS0", 0xFFFFFFFF, &driver);
    call_cmd(driver, GenImg, 0);
}