#include <r502.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    Driver *driver;
    Reply reply;
    int32_t err = SUCCESS;

    if (argc < 2 || argc > 2) {
        printf("Usage: %s <port> \n", argv[0]);
        return -1;
    }

    printf("[INFO]  Using port %s\n", argv[1]);
    printf("[INFO]  Initializing device communication\n");
    err = init_driver((uint8_t *) argv[1], 0xFFFFFFFF, &driver);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  HandShaking device\n");
    err = call_cmd(driver, HandShake, &reply, 0);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  Reading System Parameters:\n");
    err = call_cmd(driver, ReadSysPara, &reply, 0);
    if (err != SUCCESS)
        goto error;

    uint16_t status_register = reply.body.read_sys_para.status_register;
    uint16_t sys_id_code = reply.body.read_sys_para.sys_id_code;
    uint16_t library_size = reply.body.read_sys_para.library_size;
    uint16_t security_level = reply.body.read_sys_para.security_level;
    uint16_t device_address = reply.body.read_sys_para.device_address;
    uint16_t packet_size = reply.body.read_sys_para.packet_size;
    uint16_t baud_setting = reply.body.read_sys_para.baud_setting;

    printf("\t*-----------------------------------------------*\n");
    printf("\t|   .Status Register:           %u\t\t|\n", status_register);
    printf("\t|   .System identifier code:    %u\t\t|\n", sys_id_code);
    printf("\t|   .Finger library size:       %u\t\t|\n", library_size);
    printf("\t|   .Security level:            %u\t\t|\n", security_level);
    printf("\t|   .Device address:            %u\t\t|\n", device_address);
    printf("\t|   .Data packet size:          %u\t\t|\n", packet_size);
    printf("\t|   .Baud settings:             %i bps (%u)\t|\n", baud_setting*9600, baud_setting);
    printf("\t*-----------------------------------------------*\n");
    return 0;

error:
    printf("[ERROR] Error raised: 0x%.2X\n", err);
    return err;
}
