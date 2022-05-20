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


    printf("[INFO]  Place finger on reader\n");
    sleep(2);
    err = call_cmd(driver, GenImg, &reply, 0);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  Processing image into a buffer\n");
    err = call_cmd(driver, Img2Tz, &reply, 1, 1);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  Searching the library for a match\n");
    err = call_cmd(driver, Search, &reply, 3, 1, 0, 0xFF);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  Finger found. Index = %u    Match Score = %u\n",
        reply.body.search.index, reply.body.search.match_score);

    destroy_driver(driver);

    return 0;

error:
    printf("[ERROR] Error raised: 0x%.2X\n", err);
    return err;
}
