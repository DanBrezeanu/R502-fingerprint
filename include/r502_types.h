#ifndef __R502_TYPES__
#define __R502_TYPES__

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <r502_error_codes.h>

#define FINGERPRINT_SIZE 1536

/* Driver for R502 device communication via USART */
typedef struct Driver {
    /* Address of the device (default 0xFFFFFFFF) */
    uint32_t address;

    /* Serial port used for communication */
    struct sp_port *sp;

    /* Send buffer */
    uint8_t *cmd_buf;

    /* Send buffer length */
    int32_t cmd_buf_len;

    /* Receive buffer */
    uint8_t recv_buf[1024];
} Driver;

/* The commands supported so far by this driver */
/* NOTE: Do not modify the order of the commands. They are used as indices. */
typedef enum CommandType {
    /* Read system parameters for extra information about the device */
    ReadSysPara = 0,

    /* Verify if the provided password is correct */
    VfyPwd,

    /* Generate an image buffer from the fingerprint sensor */
    GenImg,

    /* Transcode the image buffer into a character buffer */
    Img2Tz,

    /* Search for a match between one character buffer and the fingerprint library */
    Search,

    /* Loads a fingerprint into one of the two character buffers */
    LoadChar,

    /* Performs a match between the two character buffers */
    Match,

    /* Provides the next available index in the fingerprint library */
    TemplateNum,

    /* Combines the two character buffers into a fingeprint template */
    /* The template is returned into both character buffers          */
    RegModel,

    /* Stores a fingerprint template from one of the character buffers */
    Store,

    /* Deletes stores fingerprints from the library starting from an index */
    DeleteChar,
    
    /* Upload character file from one of the buffers to upper computer */
    UpChar

} CommandType;

/* Represents the command to be sent to the device. */
/* NOTE: One single command can be used at a time using this structure */
typedef struct Command {
    CommandType command_type;

    union CommandBody {
        struct ReadSysPara {} read_sys_para;

        struct VfyPwd {
            /* Password to be checked against */
            uint32_t passw;
        } vfy_pwd;

        struct GenImg {} gen_img;

        struct Img2Tz {
            /* Character buffer number to store the image */
            /* NOTE: Available options are 1 and 2. Any other value defaults to 2 */
            uint8_t buf;
        } img2tz;

        struct Search {
            /* Character buffer number to be searched */
            /* NOTE: Available options are 1 and 2. Any other value defaults to 2 */
            uint8_t buf;

            /* Start index of the search. 0-based */
            uint16_t start;

            /* End index of the search. Use 0xFFFF to scan until the very end */
            uint16_t end;
        } search;

        struct LoadChar {
            /* Character buffer number to have the fingerprint loaded in */
            /* NOTE: Available options are 1 and 2. Any other value defaults to 2 */
            uint8_t buf;

            /* Index of the fingerprint to be loaded. 0-based */
            uint16_t index;
        } load_char;

        struct Match {} match;

        struct TemplateNum {} template_num;

        struct RegModel {} reg_model;

        struct Store {
            /* Character buffer number to be stored */
            /* NOTE: Available options are 1 and 2. Any other value defaults to 2 */
            uint8_t buf;

            /* Index library to store the fingerprint */
            /* NOTE: This can overwrite existing fingerprints. Use `TemplateNum` to acquire an index */
            uint16_t index;
        } store;

        struct DeleteChar {
            /* Start index of the removal. 0-based */
            uint16_t start;

            /* Number of fingerprints to delete, starting from `index` */
            uint16_t count;
        } delete_char;

        struct UpChar {
            /* Character buffer number to be stored */
            /* NOTE: Available options are 1 and 2. Any other value defaults to 2 */
            uint8_t buf;
        } up_char;

    } body;

} Command;


/* Status register bits */
#define SR_BUSY(x)       ((x) & (1 << 0))
#define SR_FING_MATCH(x) ((x) & (1 << 1))
#define SR_PWD_OK(x)     ((x) & (1 << 2))
#define SR_IMG_VALID(x)  ((x) & (1 << 3))

/* Represents the reply received from the device. */
/* NOTE: One single reply can be used at a time using this structure */
typedef struct Reply {
    CommandType command_type;

    /* Confirmation code received */
    /* For all available codes check `r502_error_codes.h` */
    uint8_t conf_code;

    union ReplyBody {
        struct ReadSysParaReply {
            /* Status information */
            uint16_t status_register;

            /* System identifier code. Fixed value 0x0009 */
            uint16_t sys_id_code;

            /* Fingerprint library size (maximum number, not enrolled) */
            uint16_t library_size;

            /* Security level [1-5] */
            uint16_t security_level;

            /* Device address. Default is 0xFFFFFFFF */
            uint32_t device_address;

            /* Packet size [0-3]
            * 0: 32 bytes
            * 1: 64 bytes
            * 2: 128 bytes (default)
            * 3: 256 bytes
            */
            uint16_t packet_size;

            /* Baud rate setting. Multiply by 9600 to get actual value */
            /* Default is 6 (57600) */
            uint16_t baud_setting;
        } read_sys_para;

        struct VfyPwdReply {} vfy_pwd;

        struct GenImgReply {} gen_img;

        struct Img2TzReply {} img2tz;

        struct SearchReply {
            /* Index of the best match in the library */
            uint16_t index;

            /* Match score */
            /* NOTE: a match_score of 0, means no match (error FINGER_NOT_FOUND (0x09)) */
            uint16_t match_score;
        } search;

        struct LoadCharReply {} load_char;

        struct MatchReply {
            /* Match score */
            uint16_t match_score;
        } match;

        struct TemplateNumReply {
            /* Next available index in the library */
            uint16_t index;
        } template_num;

        struct RegModelReply {} reg_model;

        struct StoreReply {} store;

        struct DeleteCharReply {} delete_char;

        struct UpCharReply {
            /* Fingerprint data */
            uint8_t fingerprint[FINGERPRINT_SIZE];
        } up_char;

    } body;

} Reply;


#endif
