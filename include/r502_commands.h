#ifndef __R502_COMMANDS_H__
#define __R502_COMMANDS_H__

#include <r502_types.h>
#include <stdarg.h>

#define BASIC_HEADER_LEN  7
#define CHECKSUM_LEN  2

#define READ_SYS_PARA_LEN 12
#define VFY_PWD_LEN       16
#define GEN_IMG_LEN       12
#define IMG2TZ_LEN        13
#define SEARCH_LEN        17
#define LOAD_CHAR_LEN     15
#define MATCH_LEN         12
#define TEMPLATE_NUM_LEN  12
#define REG_MODEL_LEN     12
#define STORE_LEN         15
#define DELETE_CHAR_LEN   16

int32_t get_command_package(Driver *driver, Command command, int32_t arg_num, va_list ap);
int32_t get_basic_header(Driver *driver, uint8_t **basic_header);
int32_t populate_command_args(Command *command, int32_t arg_num, va_list ap);

static int32_t read_sys_para_pkg(uint8_t *pkg, Command command, int32_t pkg_len);
static int32_t vfy_pwd_pkg(uint8_t *pkg, Command command, int32_t pkg_len);
static int32_t gen_img_pkg(uint8_t *pkg, Command command, int32_t pkg_len);
static int32_t img2tz_pkg(uint8_t *pkg, Command command, int32_t pkg_len);
static int32_t search_pkg(uint8_t *pkg, Command command, int32_t pkg_len);
static int32_t load_char_pkg(uint8_t *pkg, Command command, int32_t pkg_len);
static int32_t match_pkg(uint8_t *pkg, Command command, int32_t pkg_len);
static int32_t template_num_pkg(uint8_t *pkg, Command command, int32_t pkg_len);
static int32_t reg_model_pkg(uint8_t *pkg, Command command, int32_t pkg_len);
static int32_t store_pkg(uint8_t *pkg, Command command, int32_t pkg_len);
static int32_t delete_char_pkg(uint8_t *pkg, Command command, int32_t pkg_len);


#endif