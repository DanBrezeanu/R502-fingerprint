#ifndef __R502_TYPES__
#define __R502_TYPES__

#include <stdlib.h>
#include <stdint.h>
#include <r502_error_codes.h>

typedef struct Driver {
    uint32_t address;
    struct sp_port *sp;
    uint8_t *cmd_buf;
    int32_t cmd_buf_len;
    int8_t recv_buf[1024];
} Driver;

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

typedef struct Reply {
    CommandType command_type;
    uint8_t conf_code;

    union ReplyBody {
        struct ReadSysParaReply {
            uint16_t status_register;
            uint16_t sys_id_code;
            uint16_t library_size;
            uint16_t security_level;
            uint32_t device_address;
            uint16_t packet_size;
            uint16_t baud_setting;
        } read_sys_para;
        
        struct VfyPwdReply {} vfy_pwd;
        
        struct GenImgReply {} gen_img;
        
        struct Img2TzReply {} img2tz;

        struct SearchReply {
            uint16_t index;
            uint16_t match_score;
        } search;

        struct LoadCharReply {} load_char;
        
        struct MatchReply {
            uint16_t match_score;
        } match;

        struct TemplateNumReply {
            uint16_t index;
        } template_num;

        struct RegModelReply {} reg_model;

        struct StoreReply {} store;

        struct DeleteCharReply {} delete_char;

    } body;

} Reply;


#endif


