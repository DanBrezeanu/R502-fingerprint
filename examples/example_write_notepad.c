#include <r502.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    Driver *driver;
    Reply reply;
    int32_t err = SUCCESS;

    if (argc < 4 || argc > 4) {
        printf("Usage: %s <page_num> <data> <port> \n", argv[0]);
        return -1;
    }

    printf("[INFO]  Using port %s\n", argv[3]);
    printf("[INFO]  Initializing device communication\n");
    err = init_driver(argv[3], 0xFFFFFFFF, &driver);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  Checking device status\n");
    err = call_cmd(driver, ReadSysPara, &reply, 0);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  Verifying password (default: 0x00000000)\n");
    err = call_cmd(driver, VfyPwd, &reply, 1, 0x00000000);
    if (err != SUCCESS)
        goto error;

    uint8_t page_num = atoi(argv[1]);

    printf("[INFO]  Writing data to page %d\n", page_num);
    err = call_cmd(driver, WriteNotepad, &reply, 2, page_num, argv[2]);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  Data written succesfully!\n");

    return 0;

error:
    printf("[ERROR] Error raised: 0x%.2X\n", err);
    return err;
}