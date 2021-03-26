#ifndef __R502_REPLY_H__
#define __R502_REPLY_H__

#include <r502_types.h>

#define MAX_PACKAGE_LENGTH  139
#define MAX_DATA_LENGTH 128

/* Reply package lengths */
#define READ_SYS_PARA_REPLY_LEN  28
#define VFY_PWD_REPLY_LEN        12
#define GEN_IMG_REPLY_LEN        12
#define IMG2TZ_REPLY_LEN         12
#define SEARCH_REPLY_LEN         16
#define LOAD_CHAR_REPLY_LEN      12
#define MATCH_REPLY_LEN          14
#define TEMPLATE_NUM_REPLY_LEN   14
#define REG_MODEL_REPLY_LEN      12
#define STORE_REPLY_LEN          12
#define DELETE_CHAR_REPLY_LEN    12
#define UP_CHAR_REPLY_LEN        12
#define WRITE_NOTEPAD_REPLY_LEN  12
#define READ_NOTEPAD_REPLY_LEN   44

/* Additional reply package lengths */
#define UP_CHAR_ADDITIONAL_REPLY_LEN  1536

/* Build a Reply structure from raw data received from the serial port */
int32_t parse_reply(CommandType type, uint8_t *data, Reply *reply);

/* Receive and parse additional packages from the serial port. */
int32_t parse_additional_packages(Driver *driver, CommandType type, Reply *reply);

/* Build a ReadSysPara Reply */
static int32_t read_sys_para_reply(uint8_t *data, Reply *reply);

/* Build a Search Reply */
static int32_t search_reply(uint8_t *data, Reply *reply);

/* Build a Match Reply */
static int32_t match_reply(uint8_t *data, Reply *reply);

/* Build a TemplateNum Reply */
static int32_t template_num_reply(uint8_t *data, Reply *reply);

/* Build a ReadNotepad reply */
static int32_t read_notepad_reply(uint8_t *data, Reply *reply);

/* Build a UpChar additional reply */
static int32_t upchar_additional_reply(Driver *driver, Reply *reply);


/* Other commands' replies contain just the confirmation code */

#endif
