#include <r502.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    Driver *driver;
    Reply reply;
    int32_t err = SUCCESS;

    if (argc < 3 || argc > 3) {
        printf("Usage: %s <page_num> <port> \n", argv[0]);
        return -1;
    }

    printf("[INFO]  Using port %s\n", argv[2]);
    printf("[INFO]  Initializing device communication\n");
    err = init_driver((uint8_t *) argv[2], 0xFFFFFFFF, &driver);
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

    printf("[INFO]  Reading data from page %d\n", page_num);
    err = call_cmd(driver, ReadNotepad, &reply, 1, page_num);
    if (err != SUCCESS)
        goto error;
  
    printf("[INFO]  Data read succesfully!\n");
    printf("[INFO]  Data hexdump:\n");
    for (uint32_t i = 0; i < PAGE_SIZE; ++i) {
        if (i % 16 == 0 && i)
            printf("\n");

        printf("%.2X ", reply.body.read_notepad.data[i]);
    }
    printf("\n");

    return 0;

error:
    printf("[ERROR] Error raised: 0x%.2X\n", err);
    return err;
}
