#ifndef __R502_COMMANDS_H__
#define __R502_COMMANDS_H__

#include <r502_types.h>
#include <stdarg.h>

#define BASIC_HEADER_LEN        7
#define CHECKSUM_LEN            2

/* Command package lengths */
#define READ_SYS_PARA_LEN       12
#define VFY_PWD_LEN             16
#define GEN_IMG_LEN             12
#define IMG2TZ_LEN              13
#define SEARCH_LEN              17
#define LOAD_CHAR_LEN           15
#define MATCH_LEN               12
#define TEMPLATE_NUM_LEN        12
#define REG_MODEL_LEN           12
#define STORE_LEN               15
#define DELETE_CHAR_LEN         16
#define UP_CHAR_LEN             13
#define WRITE_NOTEPAD_LEN       45
#define READ_NOTEPAD_LEN        13
#define AURA_LED_CONFIG_LEN     16
#define SET_PWD_LEN             16
#define SET_ADDR_LEN            16
#define SET_SYS_PARA_LEN        14
#define CONTROL_LEN             13
#define READ_INDEX_TABLE_LEN    13
#define UP_IMAGE_LEN            12
#define DOWN_IMAGE_LEN          12
#define DOWN_CHAR_LEN           13
#define EMPTY_LEN               12
#define GET_IMAGE_EX_LEN        12
#define CANCEL_LEN              12
#define HANDSHAKE_LEN           12
#define CHECK_SENSOR_LEN        12
#define GET_ALG_VER_LEN         12
#define GET_FW_VER_LEN          12
#define READ_PRODUCT_INFO_LEN   12
#define SOFT_RESET_LEN          12
#define GET_RANDOM_CODE_LEN     12
#define READ_INF_PAGE_LEN       12

/* Build a package to send for command using the given arguments */
int32_t get_command_package(Driver *driver, Command command, int32_t arg_num, va_list ap);

/* Build the general 7-byte header that will be prepended to the package  */
int32_t get_basic_header(Driver *driver, uint8_t **basic_header);

/* Populate the appropiate body structure from the command with the given arguments  */
int32_t populate_command_args(Command *command, int32_t arg_num, va_list ap);

/* Build package for ReadSysPara command */
static int32_t read_sys_para_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for VfyPwd command */
static int32_t vfy_pwd_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for GenImg command */
static int32_t gen_img_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for Img2Tz command */
static int32_t img2tz_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for Search command */
static int32_t search_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for LoadChar command */
static int32_t load_char_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for Match command */
static int32_t match_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for TemplateNum command */
static int32_t template_num_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for RegModel command */
static int32_t reg_model_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for Store command */
static int32_t store_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for DeleteChar command */
static int32_t delete_char_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for UpChar command */
static int32_t up_char_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for WriteNotepad command */
static int32_t write_notepad_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for ReadNotepad command */
static int32_t read_notepad_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for AuraLedConfig command */
static int32_t aura_led_config_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for SetPwd command */
static int32_t set_pwd_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for SetAddr command */
static int32_t set_addr_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for SetSysPara command */
static int32_t set_sys_para_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for Control command */
static int32_t control_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for ReadIndexTable command */
static int32_t read_index_table_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for UpImage command */
static int32_t up_imgage_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for DownImage command */
static int32_t down_imgage_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for DownChar command */
static int32_t down_char_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for Empty command */
static int32_t empty_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for GetImageEx command */
static int32_t get_image_ex_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for Cancel command */
static int32_t cancel_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for Handshake command */
static int32_t handshake_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for CheckSensor command */
static int32_t check_sensor_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for GetAlgVer command */
static int32_t get_alg_ver_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for GetFwVer command */
static int32_t get_fw_ver_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for ReadProductInfo command */
static int32_t read_product_info_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for SoftReset command */
static int32_t soft_reset_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for GetReandomCode command */
static int32_t get_random_code_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

/* Build package for ReadInfPage command */
static int32_t read_inf_page_pkg(uint8_t *pkg, Command command, int32_t pkg_len);

#endif
