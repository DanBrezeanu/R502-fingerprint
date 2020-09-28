#ifndef __R502_ERROR_CODES_H__
#define __R502_ERROR_CODES_H__


#define SUCCESS           0x00

/* Device response error codes */
#define ERR_RECV          0x01
#define NO_FINGER         0x02
#define FAIL_ENROLL       0x03
#define FAIL_GENERATE     0x06
#define FAIL_GENERATE_    0x07
#define NO_MATCH          0x08
#define FINGER_NOT_FOUND  0x09
#define FAIL_COMBINE_CH   0x0A
#define ID_TOO_BIG        0x0B
#define ERR_READ_TEMPL    0x0C
#define ERR_UPLOAD_TEMPL  0x0D
#define ERR_RECV_PACKG    0x0E
#define ERR_UPLOAD_IMG    0x0F
#define ERR_DELETE_TEMPL  0x10
#define ERR_CLEAR_TEMPL   0x11
#define WRONG_PASSW       0x13
#define FAIL_GEN_IMAGE    0x15
#define ERR_WRITE_FLASH   0x18
#define UNKNOWN_ERR       0x19
#define INV_REGISTER_NO   0x1A
#define INV_REGISTER_CONF 0x1B
#define WRONG_NOTEPAD_PG  0x1C
#define FAIL_COMM_PORT    0x1D


/* Driver-defined error codes */
#define INIT_ARG                 0x20
#define INIT_FAIL                0x21
#define HEADER_FAIL              0x22
#define CMD_PKG_FAIL             0x23
#define POP_COMM_ARG             0x24
#define POP_COMM_NOT_ENOUGH_ARGS 0x25
#define CHKSUM_FAIL              0x26
#define REPLY_FAIL               0x27
#define ERR_SEND                 0x28


#endif
