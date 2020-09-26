#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <driver.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>

typedef enum CommandType {
    ReadSysPara = 0, VfyPwd, GenImg, Img2Tz, Search, LoadChar,
    Match, TemplateNum, RegModel, Store, DeleteChar
} CommandType;

typedef struct Command {
    CommandType command_type;

    union CommandBody {
        struct ReadSysPara {} read_sys_para;
        
        struct VfyPwd { 
            uint32_t passw;
        } vfy_pwd;
        
        struct GenImg {} gen_img;
        
        struct Img2Tz {
            uint8_t buf;
        } img2tz;

        struct Search {
            uint8_t buf;
            uint16_t start;
            uint16_t end;
        } search;

        struct LoadChar {
            uint8_t buf;
            uint16_t index;
        } load_char;
        
        struct Match {} match;

        struct TemplateNum {} template_num;

        struct RegModel {} reg_model;

        struct Store {
            uint8_t buf;
            uint16_t index;
        } store;

        struct DeleteChar {
            uint16_t start;
            uint16_t count;
        } delete_char;

    } body;

} Command;

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