#include <r502_commands.h>
#include <r502_driver.h>

/* Function pointer array for creating command packages */
/* NOTE: Do not change the order. Array is indexed by CommandType enum */
static int32_t (*cmd_func[])(uint8_t *, Command, int32_t) = {
    read_sys_para_pkg, vfy_pwd_pkg, gen_img_pkg, img2tz_pkg, search_pkg,
    load_char_pkg, match_pkg, template_num_pkg, reg_model_pkg, store_pkg,
    delete_char_pkg, up_char_pkg, write_notepad_pkg, read_notepad_pkg, aura_led_config_pkg,
    set_pwd_pkg, set_addr_pkg, handshake_pkg
};

int32_t get_basic_header(Driver *driver, uint8_t **basic_header) {
    // Basic header:
    // header | 0xEF 0x01      [2]
    // addr   | driver.address [4]
    // ident  | 0x01           [1]

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

    free(address_as_bytes);

    return SUCCESS;
}

int32_t get_command_package(Driver *driver, Command command, int32_t arg_num, va_list ap) {
    uint8_t *header = NULL;
    int32_t err = SUCCESS;

    /* Populate the command structure with the arguments provided */
    err = populate_command_args(&command, arg_num, ap);
    if (err != SUCCESS)
        return err;
    
    /* Get a general header */
    err = get_basic_header(driver, &header);
    if (err != SUCCESS)
        return err;
    
    /* Get the length of the command package */
    int32_t pkg_len = get_command_pkg_len(command.command_type);

    /* Extend the header to the necessary length */
    uint8_t *package = realloc(header, pkg_len);
    if (package == NULL) {
        goto error;
    }

    /* Populate the rest of the package with the actual information */
    err = cmd_func[command.command_type](package, command, pkg_len);
    if (err != SUCCESS)
        goto error;
    
    /* Save the command package in the driver structure */
    if (driver->cmd_buf != NULL)
        free(driver->cmd_buf);

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

    /* Extract command arguments from the provided va_list */
    /* NOTE: Arguments must be provided in the exact order they were declared */

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
    case UpChar:
        if (arg_num < 1)
            goto error;

        command->body.up_char.buf = va_arg(ap, uint32_t);

        break;
    case WriteNotepad:
        if (arg_num < 2)
            goto error;

        command->body.write_notepad.page_num = va_arg(ap, uint32_t);
        memcpy(command->body.write_notepad.data, va_arg(ap, uint8_t *), PAGE_SIZE);

        break;
    case ReadNotepad:
        if (arg_num < 1)
            goto error;

        command->body.read_notepad.page_num = va_arg(ap, uint32_t);

        break;
    case AuraLedConfig:
        if (arg_num < 4)
            goto error;
        
        command->body.aura_led_config.ctrl = va_arg(ap, uint32_t);
        command->body.aura_led_config.speed = va_arg(ap, uint32_t);
        command->body.aura_led_config.color = va_arg(ap, uint32_t);
        command->body.aura_led_config.times = va_arg(ap, uint32_t);

        break;
    case SetPwd:
        if (arg_num < 1)
            goto error;

        command->body.set_pwd.passw = va_arg(ap, uint32_t);

        break;
    case SetAddr:
        if (arg_num < 1)
            goto error;

        command->body.set_addr.addr = va_arg(ap, uint32_t);

        break;
    case HandShake:
        break;
    
    default:
        goto error;
    }


    return SUCCESS;

error:
    return POP_COMM_NOT_ENOUGH_ARGS;
}

static int32_t read_sys_para_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    // Required packet:
    // basic_header        [7]
    // length | 0x00 0x03  [2]
    // instr  | 0x0F       [1]
    // chksum | checksum   [2]

    /* Package length */
    pkg[7] = 0x00;
    pkg[8] = 0x03;

    /* Instruction code */
    pkg[9] = 0x0F;

    /* Checksum */
    uint16_t chk = checksum(pkg, 6, pkg_len - 2);
    uint8_t *chk_bytes = to_bytes_MSB(&chk, CHECKSUM_LEN);

    pkg[10] = chk_bytes[0];
    pkg[11] = chk_bytes[1];

    free(chk_bytes);

    return SUCCESS;
}

static int32_t vfy_pwd_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    // Required packet:
    // basic_header           [7]
    // length | 0x00 0x07     [2]
    // instr  | 0x13          [1]
    // passw  | vfy_pwd.passw [4]
    // chksum | checksum      [2]

    /* Package length */
    pkg[7] = 0x00;
    pkg[8] = 0x07;

    /* Instruction code */
    pkg[9] = 0x13;

    /* Password */
    size_t passw_size = sizeof(command.body.vfy_pwd.passw);
    uint8_t *passw_bytes = to_bytes_MSB(&(command.body.vfy_pwd.passw), passw_size);
    for (int i = 0; i < passw_size; ++i)
        pkg[i + 10] = passw_bytes[i];

    /* Checksum */
    uint16_t chk = checksum(pkg, 6, pkg_len - 2);
    uint8_t *chk_bytes = to_bytes_MSB(&chk, CHECKSUM_LEN);

    pkg[14] = chk_bytes[0];
    pkg[15] = chk_bytes[1];

    free(chk_bytes);
    free(passw_bytes);

    return SUCCESS;
}

static int32_t gen_img_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    // Required packet:
    // basic_header       [7]
    // length | 0x00 0x03 [2]
    // instr  | 0x01      [1]
    // chksum | 0x00 0x05 [2]

    /* Package length */
    pkg[7] = 0x00;
    pkg[8] = 0x03;

    /* Instruction code */
    pkg[9] = 0x01;

    /* Checksum */
    pkg[10] = 0x00;
    pkg[11] = 0x05;

    return SUCCESS;
}

static int32_t img2tz_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    // Required packet:
    // basic_header        [7]
    // length | 0x00 0x04  [2]
    // instr  | 0x02       [1]
    // bufid  | img2tz.buf [1]
    // chksum | checksum   [2]

    /* Package length */
    pkg[7] = 0x00;
    pkg[8] = 0x04;

    /* Instruction code */
    pkg[9] = 0x02;

    /* Buffer number */
    pkg[10] = command.body.img2tz.buf;

    /* Checksum */
    uint16_t chk = checksum(pkg, 6, pkg_len - 2);
    uint8_t *chk_bytes = to_bytes_MSB(&chk, CHECKSUM_LEN);

    pkg[11] = chk_bytes[0];
    pkg[12] = chk_bytes[1];

    free(chk_bytes);

    return SUCCESS;
}

static int32_t search_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    // Required packet:
    // basic_header          [7]
    // length | 0x00 0x08    [2]
    // instr  | 0x04         [1]
    // bufid  | search.buf   [1]
    // start  | search.start [2]
    // end    | search.end   [2]
    // chksum | checksum     [2]

    /* Package length */
    pkg[7] = 0x00;
    pkg[8] = 0x08;

    /* Instruction code */
    pkg[9] = 0x04;

    /* Buffer number */
    pkg[10] = command.body.search.buf;

    /* Start index */
    uint8_t *start_bytes = to_bytes_MSB(&(command.body.search.start), sizeof(uint16_t));
    pkg[11] = start_bytes[0];
    pkg[12] = start_bytes[1];

    /* End index */
    uint8_t *end_bytes = to_bytes_MSB(&(command.body.search.end), sizeof(uint16_t));
    pkg[13] = end_bytes[0];
    pkg[14] = end_bytes[1];

    /* Checksum */
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
    // Required packet:
    // basic_header             [7]
    // length | 0x00 0x06       [2]
    // instr  | 0x07            [1]
    // bufid  | load_char.buf   [1]
    // index  | load_char.index [1]
    // chksum | checksum        [2]

    /* Package length */
    pkg[7] = 0x00;
    pkg[8] = 0x06;

    /* Instruction code */
    pkg[9] = 0x07;

    /* Buffer number */
    pkg[10] = command.body.load_char.buf;

    /* Index */
    uint8_t *index_bytes = to_bytes_MSB(&(command.body.load_char.index), sizeof(uint16_t));
    pkg[11] = index_bytes[0];
    pkg[12] = index_bytes[1];

    /* Checksum */
    uint16_t chk = checksum(pkg, 6, pkg_len - 2);
    uint8_t *chk_bytes = to_bytes_MSB(&chk, CHECKSUM_LEN);
    pkg[13] = chk_bytes[0];
    pkg[14] = chk_bytes[1];

    free(chk_bytes);
    free(index_bytes);

    return SUCCESS;
}

static int32_t match_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    // Required packet:
    // basic_header             [7]
    // length | 0x00 0x03       [2]
    // instr  | 0x03            [1]
    // chksum | 0x00 0x07       [2]

    /* Package length */
    pkg[7] = 0x00;
    pkg[8] = 0x03;

    /* Instruction code */
    pkg[9] = 0x03;

    /* Checksum */
    pkg[10] = 0x00;
    pkg[11] = 0x07;

    return SUCCESS;
}

static int32_t template_num_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    // Required packet:
    // basic_header             [7]
    // length | 0x00 0x03       [2]
    // instr  | 0x1d            [1]
    // chksum | 0x00 0x21       [2]

    /* Package length */
    pkg[7] = 0x00;
    pkg[8] = 0x03;

    /* Instruction code */
    pkg[9] = 0x1d;

    /* Checksum */
    pkg[10] = 0x00;
    pkg[11] = 0x21;

    return SUCCESS;
}

static int32_t reg_model_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    // Required packet:
    // basic_header             [7]
    // length | 0x00 0x03       [2]
    // instr  | 0x05            [1]
    // chksum | 0x00 0x09       [2]

    /* Package length */
    pkg[7] = 0x00;
    pkg[8] = 0x03;

    /* Instruction code */
    pkg[9] = 0x05;

    /* Checksum */
    pkg[10] = 0x00;
    pkg[11] = 0x09;

    return SUCCESS;
}

static int32_t store_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    // Required packet:
    // basic_header          [7]
    // length | 0x00 0x06    [2]
    // instr  | 0x06         [1]
    // bufid  | store.buf    [1]
    // index  | store.index  [2]
    // chksum | checksum     [2]

    /* Package length */
    pkg[7] = 0x00;
    pkg[8] = 0x06;

    /* Instruction code */
    pkg[9] = 0x06;

    /* Buffer number */
    pkg[10] = command.body.store.buf;

    /* Index */
    uint8_t *index_bytes = to_bytes_MSB(&(command.body.store.index), sizeof(uint16_t));
    pkg[11] = index_bytes[0];
    pkg[12] = index_bytes[1];

    /* Checksum */
    uint16_t chk = checksum(pkg, 6, pkg_len - 2);
    uint8_t *chk_bytes = to_bytes_MSB(&chk, CHECKSUM_LEN);
    pkg[13] = chk_bytes[0];
    pkg[14] = chk_bytes[1];

    free(index_bytes);
    free(chk_bytes);

    return SUCCESS;
}

static int32_t delete_char_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    // Required packet:
    // basic_header                [7]
    // length  | 0x00 0x07         [2]
    // instr   | 0x0c              [1]
    // index   | delete_char.start [2]
    // ndelete | delete_char.count [2]
    // chksum  | checksum          [2]

    /* Package length */
    pkg[7] = 0x00;
    pkg[8] = 0x07;

    /* Instruction code */
    pkg[9] = 0x0c;

    /* Start index */
    uint8_t *start_bytes = to_bytes_MSB(&(command.body.delete_char.start), sizeof(uint16_t));
    pkg[10] = start_bytes[0];
    pkg[11] = start_bytes[1];

    /* Number of deleted templates */
    uint8_t *count_bytes = to_bytes_MSB(&(command.body.delete_char.count), sizeof(uint16_t));
    pkg[12] = count_bytes[0];
    pkg[13] = count_bytes[1];

    /* Checksum */
    uint16_t chk = checksum(pkg, 6, pkg_len - 2);
    uint8_t *chk_bytes = to_bytes_MSB(&chk, CHECKSUM_LEN);
    pkg[14] = chk_bytes[0];
    pkg[15] = chk_bytes[1];

    free(start_bytes);
    free(count_bytes);
    free(chk_bytes);

    return SUCCESS;
}

static int32_t up_char_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    // Required packet:
    // basic_header         [7]
    // length | 0x00 0x04   [2]
    // instr  | 0x08        [1]
    // bufid  | up_char.buf [1]
    // chksum | checksum    [2]

    /* Package length */
    pkg[7] = 0x00;
    pkg[8] = 0x04;

    /* Instruction code */
    pkg[9] = 0x08;

    /* Buffer number */
    pkg[10] = command.body.up_char.buf;

    /* Checksum */
    uint16_t chk = checksum(pkg, 6, pkg_len - 2);
    uint8_t *chk_bytes = to_bytes_MSB(&chk, CHECKSUM_LEN);

    pkg[11] = chk_bytes[0];
    pkg[12] = chk_bytes[1];

    free(chk_bytes);

    return SUCCESS;
}

static int32_t write_notepad_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    // Required packet:
    // basic_header                      [7]
    // length   | 0x00 0x24              [2]
    // instr    | 0x18                   [1]
    // pagenum  | write_notepad.page_num [1]
    // data     | write_notepad.data     [32]
    // chksum   | checksum               [2]

    /* Package length */
    pkg[7] = 0x00;
    pkg[8] = 0x24;

    /* Instruction code */
    pkg[9] = 0x18;

    /* Page number of flash */
    pkg[10] = command.body.write_notepad.page_num;

    /* Data to store */
    memcpy(pkg + 11, command.body.write_notepad.data, PAGE_SIZE);

    /* Checksum */
    uint16_t chk = checksum(pkg, 6, pkg_len - 2);
    uint8_t *chk_bytes = to_bytes_MSB(&chk, CHECKSUM_LEN);

    pkg[43] = chk_bytes[0];
    pkg[44] = chk_bytes[1];

    free(chk_bytes);

    return SUCCESS;
}

static int32_t read_notepad_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    // Required packet:
    // basic_header                      [7]
    // length   | 0x00 0x4               [2]
    // instr    | 0x19                   [1]
    // pagenum  | read_notepad.page_num  [1]
    // chksum   | checksum               [2]

    /* Package length */
    pkg[7] = 0x00;
    pkg[8] = 0x04;

    /* Instruction code */
    pkg[9] = 0x19;

    /* Buffer number */
    pkg[10] = command.body.read_notepad.page_num;

    /* Checksum */
    uint16_t chk = checksum(pkg, 6, pkg_len - 2);
    uint8_t *chk_bytes = to_bytes_MSB(&chk, CHECKSUM_LEN);

    pkg[11] = chk_bytes[0];
    pkg[12] = chk_bytes[1];

    free(chk_bytes);

    return SUCCESS;
}

static int32_t aura_led_config_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    // Required packet:
    // basic_header                      [7]
    // length  | 0x00 0x07               [2]
    // instr   | 0x35                    [1]
    // control | aura_led_config.control [1]
    // speed   | aura_led_config.speed   [1]
    // color   | aura_led_config.color   [1]
    // times   | aura_led_config.times   [1]
    // chksum  | checksum                [2]

    /* Package length */
    pkg[7] = 0x00;
    pkg[8] = 0x07;

    /* Instruction code */
    pkg[9] = 0x35;

    /* Control Code */
    pkg[10] = command.body.aura_led_config.ctrl;

    /* Speed */
    pkg[11] = command.body.aura_led_config.speed;

    /* Color */
    pkg[12] = command.body.aura_led_config.color;

    /* Times */
    pkg[13] = command.body.aura_led_config.times;

    /* Checksum */
    uint16_t chk = checksum(pkg, 6, pkg_len - 2);
    uint8_t *chk_bytes = to_bytes_MSB(&chk, CHECKSUM_LEN);

    pkg[14] = chk_bytes[0];
    pkg[15] = chk_bytes[1];

    free(chk_bytes);

    return SUCCESS;
}

static int32_t set_pwd_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    // Required packet:
    // basic_header           [7]
    // length | 0x00 0x07     [2]
    // instr  | 0x12          [1]
    // passw  | set_pwd.passw [4]
    // chksum | checksum      [2]

    /* Package length */
    pkg[7] = 0x00;
    pkg[8] = 0x07;

    /* Instruction code */
    pkg[9] = 0x12;

    /* Password */
    size_t passw_size = sizeof(command.body.set_pwd.passw);
    uint8_t *passw_bytes = to_bytes_MSB(&(command.body.set_pwd.passw), passw_size);
    for (int i = 0; i < passw_size; ++i)
        pkg[i + 10] = passw_bytes[i];

    /* Checksum */
    uint16_t chk = checksum(pkg, 6, pkg_len - 2);
    uint8_t *chk_bytes = to_bytes_MSB(&chk, CHECKSUM_LEN);

    pkg[14] = chk_bytes[0];
    pkg[15] = chk_bytes[1];

    free(chk_bytes);
    free(passw_bytes);

    return SUCCESS;
}

static int32_t set_addr_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    // Required packet:
    // basic_header           [7]
    // length | 0x00 0x07     [2]
    // instr  | 0x12          [1]
    // passw  | set_addr.addr [4]
    // chksum | checksum      [2]

    /* Package length */
    pkg[7] = 0x00;
    pkg[8] = 0x07;

    /* Instruction code */
    pkg[9] = 0x15;

    /* Address */
    size_t addr_size = sizeof(command.body.set_addr.addr);
    uint8_t *addr_bytes = to_bytes_MSB(&(command.body.set_addr.addr), addr_size);
    for (int i = 0; i < addr_size; ++i)
        pkg[i + 10] = addr_bytes[i];

    /* Checksum */
    uint16_t chk = checksum(pkg, 6, pkg_len - 2);
    uint8_t *chk_bytes = to_bytes_MSB(&chk, CHECKSUM_LEN);

    pkg[14] = chk_bytes[0];
    pkg[15] = chk_bytes[1];

    free(chk_bytes);
    free(addr_bytes);

    return SUCCESS;
}

static int32_t handshake_pkg(uint8_t *pkg, Command command, int32_t pkg_len) {
    // Required packet:
    // basic_header             [7]
    // length | 0x00 0x03       [2]
    // instr  | 0x40            [1]
    // chksum | 0x00 0x07       [2]

    /* Package length */
    pkg[7] = 0x00;
    pkg[8] = 0x03;

    /* Instruction code */
    pkg[9] = 0x40;

    /* Checksum */
    pkg[10] = 0x00;
    pkg[11] = 0x44;

    return SUCCESS;
}