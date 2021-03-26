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


    printf("[INFO]  Place finger on reader\n");
    sleep(2);
    err = call_cmd(driver, GenImg, &reply, 0);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  Processing image into a buffer\n");
    err = call_cmd(driver, Img2Tz, &reply, 1, 1);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  Load fingerprint with index 0 into a buffer\n");
    err = call_cmd(driver, LoadChar, &reply, 2, 2, 0);
    if (err != SUCCESS)
        goto error;
    
    printf("[INFO]  Perform match between the two buffers\n");
    err = call_cmd(driver, Match, &reply, 0);
    if (err != SUCCESS)
        goto error;

    printf("[INFO]  Match Score = %u\n", 
        reply.body.match.match_score);
    
    destroy_driver(driver);

    return 0;

error:
    printf("[ERROR] Error raised: 0x%.2X\n", err);
    return err;
}
