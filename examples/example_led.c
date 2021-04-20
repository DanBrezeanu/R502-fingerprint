#include <r502.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    Driver *driver;
    Reply reply;
    int32_t err = SUCCESS;

    if (argc == 2 && strncmp(argv[1], "--help", 6) == 0) {
        printf("Usage: %s <port> <control> <speed> <color> <times>\n"
        "port - UART communication device\n"
        "control:\n"
        "     1 - breathing light\n"
        "     2 - flashing light\n"
        "     3 - light always on\n"
        "     4 - light always off\n"
        "     5 - light gradually on\n"
        "     6 - light gradually off\n"
        "speed:\n"
        "     0-255 - It is effective for breathing lamp and flashing lamp,Light gradually on,Light gradually off\n"
        "color:\n"
        "     1 - red\n"
        "     2 - blue\n"
        "     3 - purple\n"
        "times:\n"
        "     1-255, 0 for infinite\n", argv[0]);
        return 0;
    }

    if (argc != 6) {
        printf("Usage: %s <port> <control> <speed> <color> <times>\nUse %s --help for help\n", argv[0], argv[0]);
        return -1;
    }

    char *endptr;

    int32_t control;
    int32_t speed;
    int32_t color;
    int32_t times;

    errno = 0;
    control = strtol(argv[2], &endptr, 10);

    // Check for errors: e.g., the string does not represent an integer
    if (errno != 0 || *endptr != '\0' || control < 1 || control > 6) {
        printf("[ERROR] The option for the control is not valid, see %s --help for help\n", argv[0]);
        return -1;
    }

    speed = strtol(argv[3], &endptr, 10);

    if (errno != 0 || *endptr != '\0' || speed > 255 || speed < 0) {
        printf("[ERROR] The option for the speed is not valid, see %s --help for help\n", argv[0]);
        return -1;
    }

    color = strtol(argv[4], &endptr, 10);

    if (errno != 0 || *endptr != '\0' || color < 1 || color > 3) {
        printf("[ERROR] The option for the color is not valid, see %s --help for help\n", argv[0]);
        return -1;
    }

    times = strtol(argv[5], &endptr, 10);

    if (errno != 0 || *endptr != '\0' || times > 255 || times < 0) {
        printf("[ERROR] The option for times is not valid, see %s --help for help\n", argv[0]);
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

    printf("[INFO]  Executing command: %s %d %d %d %d\n", argv[0], control, speed, color, times);
    err = call_cmd(driver, AuraLedConfig, &reply, 4, control, speed, color, times);
    if (err != SUCCESS)
        goto error;
    return 0;

error:
    printf("[ERROR] Error raised: 0x%.2X\n", err);
    return err;
}