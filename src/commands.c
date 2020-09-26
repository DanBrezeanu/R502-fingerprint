#include <commands.h>
#include <driver_utils.h>
#include <stdarg.h>

static int32_t (*cmd_func[])(uint8_t *, Command, int32_t) = {
    read_sys_para_pkg, vfy_pwd_pkg, gen_img_pkg, img2tz_pkg, search_pkg,
    load_char_pkg, match_pkg, template_num_pkg, reg_model_pkg, store_pkg,
    delete_char_pkg
};

int32_t get_basic_header(Driver *driver, uint8_t **basic_header) {
    if (driver == NULL)
        return HEADER_FAIL;

    uint8_t *header = malloc(BASIC_HEADER_LEN);
    if (header == NULL)
        return HEADER_FAIL;
    
    /* Header default bytes */
    header[0] = 0xEF;
    header[1] = 0x01;

    /* Device address */
    uint8_t *address_as_bytes = to_bytes_MSB(&(driver->address), DEVICE_ADDRESS_SIZE);
    for (int i = 0; i < DEVICE_ADDRESS_SIZE; ++i)   
        header[i + 2] = address_as_bytes[i];

    /* Package identifier */
    header[6] = 0x01;

    *basic_header = header;

    return SUCCESS;
}

int32_t get_command_package(Driver *driver, Command command, int32_t arg_num, va_list ap) {
    uint8_t *header = NULL;
    int32_t err = SUCCESS;

    err = populate_command_args(&command, arg_num, ap);
    if (err != SUCCESS)
        return err;

    err = get_basic_header(driver, &header);
    if (err != SUCCESS)
        return err;

    int32_t pkg_len = get_command_pkg_len(command.command_type);

    uint8_t *package = realloc(header, pkg_len);
    if (package == NULL) {
        goto error;
    }

    err = cmd_func[command.command_type](package, command, pkg_len);
    if (err != SUCCESS)
        goto error;

    driver->cmd_buf = package;
    driver->cmd_buf_len = pkg_len;
    
    return SUCCESS;

error:
    free(package);

    return CMD_PKG_FAIL;
}

int32_t populate_command_args(Command *command, int32_t arg_num, va_list ap) {
    if (command == NULL)
        return POP_COMM_ARG;

    switch (command->command_type) {
    case ReadSysPara:
        break;
    case VfyPwd:
        if (arg_num < 1)
            goto error;
        
        command->body.vfy_pwd.passw = va_arg(ap, uint32_t);

        break;
    case GenImg:
        break;
    case Img2Tz:
        if (arg_num < 1)
            goto error;
        
        command->body.img2tz.buf = va_arg(ap, uint32_t);

        break;
    case Search:
        if (arg_num < 3)
            goto error;
        
        command->body.search.buf = va_arg(ap, uint32_t);
        command->body.search.start = va_arg(ap, uint32_t);
        command->body.search.end = va_arg(ap, uint32_t);
        
        break;
    case LoadChar:
        if (arg_num < 2)
            goto error;
        
        command->body.load_char.buf = va_arg(ap, uint32_t);
        command->body.load_char.index = va_arg(ap, uint32_t);
        
        break;
    case Match:
        break;
    case TemplateNum:
        break;
    case RegModel:
        break;
    case Store:
        if (arg_num < 2)
            goto error;
        
        command->body.store.buf = va_arg(ap, uint32_t);
        command->body.store.index = va_arg(ap, uint32_t);
        
        break;
    case DeleteChar:
        if (arg_num < 2)
            goto error;
        
        command->body.delete_char.start = va_arg(ap, uint32_t);
        command->body.delete_char.count = va_arg(ap, uint32_t);
        
        break;
    }

    return SUCCESS;

error:
    return POP_COMM_NOT_ENOUGH_ARGS;
}

static int32_t read_sys_para_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    pkg[7] = 0x00;
    pkg[8] = 0x03;
    pkg[9] = 0x0F;

    uint16_t chk = checksum(pkg, 6, pkg_len - 2);
    uint8_t *chk_bytes = to_bytes_MSB(&chk, CHECKSUM_LEN);

    pkg[10] = chk_bytes[0];
    pkg[11] = chk_bytes[1];

    free(chk_bytes);

    return SUCCESS;
}

static int32_t vfy_pwd_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    pkg[7] = 0x00;
    pkg[8] = 0x07;
    pkg[9] = 0x13;

    size_t passw_size = sizeof(command.body.vfy_pwd.passw);
    uint8_t *passw_bytes = to_bytes_MSB(&(command.body.vfy_pwd.passw), passw_size);
    for (int i = 0; i < passw_size; ++i)
        pkg[i + 10] = passw_bytes[i];

    uint16_t chk = checksum(pkg, 6, pkg_len - 2);
    uint8_t *chk_bytes = to_bytes_MSB(&chk, CHECKSUM_LEN);

    pkg[14] = chk_bytes[0];
    pkg[15] = chk_bytes[1];

    free(chk_bytes);
    free(passw_bytes);

    return SUCCESS;
}

static int32_t gen_img_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    pkg[7] = 0x00;
    pkg[8] = 0x03;
    pkg[9] = 0x01;
    pkg[10] = 0x00;
    pkg[11] = 0x05;

    return SUCCESS;
}

static int32_t img2tz_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    pkg[7] = 0x00;
    pkg[8] = 0x04;
    pkg[9] = 0x02;
    pkg[10] = command.body.img2tz.buf;
    
    uint16_t chk = checksum(pkg, 6, pkg_len - 2);
    uint8_t *chk_bytes = to_bytes_MSB(&chk, CHECKSUM_LEN);

    pkg[11] = chk_bytes[0];
    pkg[12] = chk_bytes[1];

    free(chk_bytes);

    return SUCCESS;
}

static int32_t search_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    pkg[7] = 0x00;
    pkg[8] = 0x08;
    pkg[9] = 0x04;
    pkg[10] = command.body.search.buf;

    uint8_t *start_bytes = to_bytes_MSB(&(command.body.search.start), sizeof(uint16_t));
    pkg[11] = start_bytes[0];
    pkg[12] = start_bytes[1];

    uint8_t *end_bytes = to_bytes_MSB(&(command.body.search.end), sizeof(uint16_t));
    pkg[13] = end_bytes[0];
    pkg[14] = end_bytes[1];

    uint16_t chk = checksum(pkg, 6, pkg_len - 2);
    uint8_t *chk_bytes = to_bytes_MSB(&chk, CHECKSUM_LEN);
    pkg[15] = chk_bytes[0];
    pkg[16] = chk_bytes[1];

    free(chk_bytes);
    free(start_bytes);
    free(end_bytes);

    return SUCCESS;
}

static int32_t load_char_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    pkg[7] = 0x00;
    pkg[8] = 0x06;
    pkg[9] = 0x07;
    pkg[10] = command.body.load_char.buf;

    uint8_t *index_bytes = to_bytes_MSB(&(command.body.load_char.index), sizeof(uint16_t));
    pkg[11] = index_bytes[0];
    pkg[12] = index_bytes[1];

    uint16_t chk = checksum(pkg, 6, pkg_len - 2);
    uint8_t *chk_bytes = to_bytes_MSB(&chk, CHECKSUM_LEN);
    pkg[13] = chk_bytes[0];
    pkg[14] = chk_bytes[1];

    free(chk_bytes);
    free(index_bytes);

    return SUCCESS;
}

static int32_t match_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    pkg[7] = 0x00;
    pkg[8] = 0x03;
    pkg[9] = 0x03;
    pkg[10] = 0x00;
    pkg[11] = 0x07;
    
    return SUCCESS;
}

static int32_t template_num_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    pkg[7] = 0x00;
    pkg[8] = 0x03;
    pkg[9] = 0x1d;
    pkg[10] = 0x00;
    pkg[11] = 0x21;

    return SUCCESS;
}

static int32_t reg_model_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    pkg[7] = 0x00;
    pkg[8] = 0x03;
    pkg[9] = 0x05;
    pkg[10] = 0x00;
    pkg[11] = 0x09;

    return SUCCESS;
}

static int32_t store_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    pkg[7] = 0x00;
    pkg[8] = 0x06;
    pkg[9] = 0x06;
    pkg[10] = command.body.store.buf;

    uint8_t *index_bytes = to_bytes_MSB(&(command.body.store.index), sizeof(uint16_t));
    pkg[11] = index_bytes[0];
    pkg[12] = index_bytes[1];

    uint16_t chk = checksum(pkg, 6, pkg_len - 2);
    uint8_t *chk_bytes = to_bytes_MSB(&chk, CHECKSUM_LEN);
    pkg[13] = chk_bytes[0];
    pkg[14] = chk_bytes[1];

    return SUCCESS;
}

static int32_t delete_char_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    pkg[7] = 0x00;
    pkg[8] = 0x07;
    pkg[9] = 0x0c;

    uint8_t *start_bytes = to_bytes_MSB(&(command.body.delete_char.start), sizeof(uint16_t));
    pkg[10] = start_bytes[0];
    pkg[11] = start_bytes[1];

    uint8_t *count_bytes = to_bytes_MSB(&(command.body.delete_char.count), sizeof(uint16_t));
    pkg[12] = count_bytes[0];
    pkg[13] = count_bytes[1];

    uint16_t chk = checksum(pkg, 6, pkg_len - 2);
    uint8_t *chk_bytes = to_bytes_MSB(&chk, CHECKSUM_LEN);
    pkg[14] = chk_bytes[0];
    pkg[16] = chk_bytes[1];

    return SUCCESS;
}



