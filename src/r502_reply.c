#include <r502_reply.h>
#include <r502_driver_utils.h>
#include <r502_driver.h>

int32_t parse_reply(CommandType type, uint8_t *data, Reply *reply) {
    int32_t len = get_command_reply_len(type);
    if (!check_checksum(data, len))
        return CHKSUM_FAIL;

    reply->command_type = type;

    /* Get confirmation code from raw data */
    reply->conf_code = data[9];

    int32_t err = SUCCESS;

    /* If the response had any other parameters, retreive them */
    switch (type) {
    case ReadSysPara:
        err = read_sys_para_reply(data, reply);
        if (err != SUCCESS)
            return REPLY_FAIL;
        break;
    case Search:
        err = search_reply(data, reply);
        if (err != SUCCESS)
            return REPLY_FAIL;
        break;
    case Match:
        err = match_reply(data, reply);
        if (err != SUCCESS)
            return REPLY_FAIL;
        break;
    case TemplateNum:
        err = template_num_reply(data, reply);
        if (err != SUCCESS)
            return REPLY_FAIL;
        break;
    case ReadNotepad:
        err = read_notepad_reply(data, reply);
        if (err != SUCCESS)
            return REPLY_FAIL;
        break;
    default:
        break;
    }

    return SUCCESS;
}

int32_t parse_additional_packages(Driver *driver, CommandType type, Reply *reply) {
    int32_t err;

    switch (type) {
    case UpChar:
        err = upchar_additional_reply(driver, reply);
        if (err != SUCCESS)
            return REPLY_FAIL;
        break;
    default:
        break;
    }

    return SUCCESS;
}

static int32_t read_sys_para_reply(uint8_t *data, Reply *reply) {
    // Expected packet:
    // headr       | 0xEF 0x01                     [2]
    // addr        | device.address                [4]
    // ident       | 0x01                          [1]
    // length      | 0x00 0x03                     [2] (3 + 16)
    // confrm      | reply.conf_code               [1]
    // status_reg  | read_sys_para.status_register [2]
    // sys_id_code | read_sys_para.sys_id_code     [2]
    // lib_size    | read_sys_para.library_size    [2]
    // sec_level   | read_sys_para.security_level  [2]
    // addr        | device.address                [4]
    // pkt_size    | read_sys_para.packet_size     [2]
    // baud        | read_sys_para.baud_setting    [2]
    // chksum      | checksum                      [2]

    reply->body.read_sys_para.status_register = from_bytes_MSB(data, 10, 12);
    reply->body.read_sys_para.sys_id_code = from_bytes_MSB(data, 12, 14);
    reply->body.read_sys_para.library_size = from_bytes_MSB(data, 14, 16);
    reply->body.read_sys_para.security_level = from_bytes_MSB(data, 16, 18);
    reply->body.read_sys_para.device_address = from_bytes_MSB(data, 18, 22);
    reply->body.read_sys_para.packet_size = from_bytes_MSB(data, 22, 24);
    reply->body.read_sys_para.baud_setting = from_bytes_MSB(data, 24, 26);

    return SUCCESS;
}

static int32_t search_reply(uint8_t *data, Reply *reply) {
    // Expected packet:
    // headr       | 0xEF 0x01                     [2]
    // addr        | device.address                [4]
    // ident       | 0x01                          [1]
    // length      | 0x00 0x07                     [2]
    // confrm      | reply.conf_code               [1]
    // index       | search.index                  [2]
    // match       | search.match_score            [2]
    // chksum      | checksum                      [2]

    reply->body.search.index = from_bytes_MSB(data, 10, 12);
    reply->body.search.match_score = from_bytes_MSB(data, 12, 14);

    return SUCCESS;
}

static int32_t match_reply(uint8_t *data, Reply *reply) {
    // Expected packet:
    // headr       | 0xEF 0x01                     [2]
    // addr        | device.address                [4]
    // ident       | 0x01                          [1]
    // length      | 0x00 0x05                     [2]
    // confrm      | reply.conf_code               [1]
    // match       | match.match_score             [2]
    // chksum      | checksum                      [2]

    reply->body.match.match_score = from_bytes_MSB(data, 10, 12);

    return SUCCESS;
}

static int32_t template_num_reply(uint8_t *data, Reply *reply) {
    // Expected packet:
    // headr       | 0xEF 0x01                     [2]
    // addr        | device.address                [4]
    // ident       | 0x01                          [1]
    // length      | 0x00 0x05                     [2]
    // confrm      | reply.conf_code               [1]
    // index       | template_num.index            [2]
    // chksum      | checksum                      [2]

    reply->body.template_num.index = from_bytes_MSB(data, 10, 12);

    return SUCCESS;
}

static int32_t read_notepad_reply(uint8_t *data, Reply *reply) {
    // Expected packet:
    // headr       | 0xEF 0x01                     [2]
    // addr        | device.address                [4]
    // ident       | 0x07                          [1]
    // length      | 0x00 0x23                     [2]
    // confrm      | reply.conf_code               [1]
    // data        | read_notepad.data             [32]
    // chksum      | checksum                      [2]

    memcpy(reply->body.read_notepad.data, data + 10, PAGE_SIZE);

    return SUCCESS;
}

static int32_t upchar_additional_reply(Driver *driver, Reply *reply) {
    // Expected packets:
    // headr       | 0xEF 0x01                     [2]
    // addr        | device.address                [4]
    // ident       | 0x02 | 0x08                   [1]
    // length      | 0x00 0x82                     [2]
    // data        | up_char.fingerprint           [128]
    // chksum      | checksum                      [2]

    int32_t len = get_command_additional_reply_len(UpChar);
    uint8_t buf[MAX_PACKAGE_LENGTH] = {0};
    int32_t len_recvd = 0;
    int32_t result;

    while (len_recvd < len) {
        result = sp_blocking_read(driver->sp, buf, MAX_PACKAGE_LENGTH, TIMEOUT);
        if (result < MAX_PACKAGE_LENGTH)
            return REPLY_FAIL;

        if (!check_checksum(buf, MAX_PACKAGE_LENGTH))
            return CHKSUM_FAIL;

        memcpy(reply->body.up_char.fingerprint + len_recvd, buf + 9, MAX_DATA_LENGTH);
        len_recvd += MAX_DATA_LENGTH;
    }

    return SUCCESS;
}