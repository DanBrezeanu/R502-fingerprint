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

    printf("[INFO]  HandShaking device\n");
    err = call_cmd(driver, HandShake, &reply, 0);
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

    printf("[INFO]  Enrolling fingerprint to index %u\n", index);

    printf("[INFO]  [1/2] Place finger on reader\n");
    sleep(2);
    err = call_cmd(driver, GenImg, &reply, 0);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  [1/2] Processing image into a buffer\n");
    err = call_cmd(driver, Img2Tz, &reply, 1, 1);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  [2/2] Place finger on reader\n");
    sleep(2);
    err = call_cmd(driver, GenImg, &reply, 0);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  [2/2] Processing image into a buffer\n");
    err = call_cmd(driver, Img2Tz, &reply, 1, 2);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  Processing buffers to generate fingerprint template\n");
    err = call_cmd(driver, RegModel, &reply, 0);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  Saving the fingerprint template\n");
    err = call_cmd(driver, Store, &reply, 2, 1, index);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  Fingerprint enrollment complete\n");
    destroy_driver(driver);

    return 0;

error:
    printf("[ERROR] Error raised: 0x%.2X\n", err);
    return err;
}
