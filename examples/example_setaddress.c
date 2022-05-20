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

    printf("[INFO]  Verifying password (default: 0x00000000)\n");
    err = call_cmd(driver, VfyPwd, &reply, 1, 0x00000000);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  Setting new address (new: 0x12345678)\n");
    err = call_cmd(driver, SetAddr, &reply, 1, 0x12345678);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  Initializing device communication\n");
    err = init_driver((uint8_t *) argv[1], 0x12345678, &driver);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  HandShaking device\n");
    err = call_cmd(driver, HandShake, &reply, 0);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  Verifying password (default: 0x00000000)\n");
    err = call_cmd(driver, VfyPwd, &reply, 1, 0x00000000);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  Revert backup to default address (default: 0xFFFFFFFF)\n");
    err = call_cmd(driver, SetAddr, &reply, 1, 0xFFFFFFFF);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  Initializing device communication\n");
    err = init_driver((uint8_t *) argv[1], 0xFFFFFFFF, &driver);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  Verifying password (default: 0x00000000)\n");
    err = call_cmd(driver, VfyPwd, &reply, 1, 0x00000000);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  HandShaking device\n");
    err = call_cmd(driver, HandShake, &reply, 0);
    if (err != SUCCESS)
        goto error;

    return 0;

error:
    printf("[ERROR] Error raised: 0x%.2X\n", err);
    return err;
}
