#include <r502_driver.h>
#include <unistd.h>

int32_t init_driver(uint8_t *port_name, int32_t address, Driver **driver) {
    if (port_name == NULL)
        return INIT_ARG;

    struct sp_port *port = NULL;
    enum sp_return result;

    /* Get port using the port name provided */
    result = sp_get_port_by_name((const char *)port_name, &port);
    if (result != SP_OK)
        return INIT_FAIL;

    /* Open port for both reading and writing */
    result = sp_open(port, SP_MODE_READ_WRITE);
    if (result != SP_OK)
        goto error;

    /* Set baud rate */
    result = sp_set_baudrate(port, DEFAULT_BAUD_RATE);
    if (result != SP_OK)
        goto error;

    /* Set data bits*/
    result = sp_set_bits(port, 8);
    if (result != SP_OK)
        goto error;

    /* Set parity bits */
    result = sp_set_parity(port, SP_PARITY_NONE);
    if (result != SP_OK)
        goto error;

    /* Set stop bits */
    result = sp_set_stopbits(port, 1);
    if (result != SP_OK)
        goto error;

    /* Set flow control */
    result = sp_set_flowcontrol(port, SP_FLOWCONTROL_NONE);
    if (result != SP_OK)
        goto error;

    /* Save the new port */
    *driver = calloc(1, sizeof(Driver));
    (*driver)->address = address;
    (*driver)->sp = port;

    return SUCCESS;

error:
    sp_free_port(port);

    return INIT_FAIL;
}

int32_t destroy_driver(Driver *driver) {
    if (driver == NULL)
        return SUCCESS;

    free(driver->cmd_buf);
    driver->cmd_buf = NULL;

    sp_free_port(driver->sp);
    driver->sp = NULL;

    free(driver);

    return SUCCESS;
}

int32_t call_cmd(Driver *driver, CommandType type, Reply *reply, int32_t arg_num, ...) {
    Command cmd = {0};
    int32_t err = SUCCESS;
    int32_t result;
    va_list ap;

    cmd.command_type = type;

    va_start(ap, arg_num);

    /* Build package from arguments */
    err = get_command_package(driver, cmd, arg_num, ap);
    if (err != SUCCESS)
        goto error;

    /* Send the raw data to the device */
    result = sp_blocking_write(driver->sp, driver->cmd_buf, driver->cmd_buf_len, TIMEOUT);

    /* Check whether we sent all of the data. */
    if (result != driver->cmd_buf_len) {
        err = ERR_SEND;
        goto error;
    }

    /* Allocate a buffer to receive data. */
    int32_t reply_len = get_command_reply_len(type);

    /* Try to receive the data */
    result = sp_blocking_read(driver->sp, driver->recv_buf, reply_len, TIMEOUT);

    /* Check whether we received the number of bytes we wanted. */
    if (result != reply_len){
        err = ERR_RECV;
        goto error;
    }

    /* Parse the raw data into a Reply structure */
    err = parse_reply(type, driver->recv_buf, reply);
    if (err != SUCCESS || reply->conf_code != SUCCESS) {
        if (reply->conf_code != SUCCESS)
            err = reply->conf_code;

        goto error;
    }

    err = SUCCESS;

error:
    va_end(ap);

    return err;
}
