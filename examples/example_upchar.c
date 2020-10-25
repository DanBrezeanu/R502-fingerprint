#include <r502.h>
#include <stdio.h>
#include <unistd.h>

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

    printf("[INFO]  Checking device status\n");
    err = call_cmd(driver, ReadSysPara, &reply, 0);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  Verifying password (default: 0x00000000)\n");
    err = call_cmd(driver, VfyPwd, &reply, 1, 0x00000000);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  Checking next valid fingerprint index\n");
    err = call_cmd(driver, TemplateNum, &reply, 0);
    if (err != SUCCESS)
        goto error;

    uint16_t index = reply.body.template_num.index;

    printf("[INFO]  Uploading last fingerprint (index %d)\n", index - 1);

    err = call_cmd(driver, LoadChar, &reply, 2, 1, (index - 1));
    if (err != SUCCESS)
        goto error;

    err = call_cmd(driver, UpChar, &reply, 1, 1);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  Fingerprint raw data:\n");

    for (int i = 0; i < FINGERPRINT_SIZE; ++i) {
        printf("%.2X ", reply.body.up_char.fingerprint[i]);
    }
    printf("\n");

    printf("[INFO]  Fingerprint upload complete\n");
    destroy_driver(driver);

    return 0;

error:
    printf("[ERROR] Error raised: 0x%.2X\n", err);
    return err;
}
