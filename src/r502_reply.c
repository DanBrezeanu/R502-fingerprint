#include <r502_reply.h>
#include <r502_driver_utils.h>

int32_t parse_reply(CommandType type, uint8_t *data, Reply *reply) {
    int32_t len = get_command_reply_len(type);
    if (!check_checksum(data, len))
        return CHKSUM_FAIL;

    for(int i = 0; i < 12; ++i)
        printf("%.2X ", data[i]);
    printf("\n");

    reply->command_type = type;
    reply->conf_code = data[9];

    int32_t err = SUCCESS;

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
    }

    return SUCCESS;
}


static int32_t read_sys_para_reply(uint8_t *data, Reply *reply) {
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
    reply->body.search.index = from_bytes_MSB(data, 10, 12);
    reply->body.search.match_score = from_bytes_MSB(data, 12, 14);

    return SUCCESS;
}

static int32_t match_reply(uint8_t *data, Reply *reply) {
    reply->body.match.match_score = from_bytes_MSB(data, 10, 12);

    return SUCCESS;
}

static int32_t template_num_reply(uint8_t *data, Reply *reply) {
    reply->body.template_num.index = from_bytes_MSB(data, 10, 12);

    return SUCCESS;
}



