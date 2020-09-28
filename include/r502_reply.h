#ifndef __R502_REPLY_H__
#define __R502_REPLY_H__

#include <r502_types.h>

/* Reply package lengths */
#define READ_SYS_PARA_REPLY_LEN 28
#define VFY_PWD_REPLY_LEN       12
#define GEN_IMG_REPLY_LEN       12
#define IMG2TZ_REPLY_LEN        12
#define SEARCH_REPLY_LEN        16
#define LOAD_CHAR_REPLY_LEN     12
#define MATCH_REPLY_LEN         14
#define TEMPLATE_NUM_REPLY_LEN  14
#define REG_MODEL_REPLY_LEN     12
#define STORE_REPLY_LEN         12
#define DELETE_CHAR_REPLY_LEN   12

/* Build a Reply structure from raw data received from the serial port */
int32_t parse_reply(CommandType type, uint8_t *data, Reply *reply);

/* Build a ReadSysPara Reply */
static int32_t read_sys_para_reply(uint8_t *data, Reply *reply);

/* Build a Search Reply */
static int32_t search_reply(uint8_t *data, Reply *reply);

/* Build a Match Reply */
static int32_t match_reply(uint8_t *data, Reply *reply);

/* Build a TemplateNum Reply */
static int32_t template_num_reply(uint8_t *data, Reply *reply);

/* Other commands' replies contain just the confirmation code */

#endif
