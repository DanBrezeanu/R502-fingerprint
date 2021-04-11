#include <r502.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>


int main(int argc, char *argv[]) {
    Driver *driver;
    Reply reply;
    int32_t err = SUCCESS;

    if (argc == 2 && strncmp(argv[1], "--help", 6) == 0) {
        printf("Usage: %s <port> <control> <speed> <color> <times>\n", argv[0]);
        printf("port - your port, ex: \"/dev/ttyS0\" if you're using UART on RaspberryPi\n");
        printf("control   1: breathing light\n");
        printf("          2: flashing light\n");
        printf("          3: light always on\n");
        printf("          4: light always off\n");
        printf("          5: light gradually on\n");
        printf("          6: light gradually off\n");
        printf("speed     0-255 It is effective for breathing lamp and flashing lamp,Light gradually on,Light gradually off\n");
        printf("color     1: red\n");
        printf("          2: blue\n");
        printf("          3: purple\n");
        printf("times     1-255, 0 for infinite\n");
        return 0;
    }

    if (argc != 6) {
        printf("Usage: %s <port> <control> <speed> <color> <times>\nUse %s --help for help\n", argv[0], argv[0]);
        return -1;
    }

    char *controlPointer;
    char *speedPointer;
    char *colorPointer;
    char *timesPointer;

    int control;
    int speed;
    int color;
    int times;

    errno = 0;
    long controlConv = strtol(argv[2], &controlPointer, 10);

    // Check for errors: e.g., the string does not represent an integer
    // or the integer is larger than int
    if (errno != 0 || *controlPointer != '\0' || controlConv > INT_MAX || controlConv < INT_MIN) {
        printf("[ERROR] The option for the control is not an integer!\n");
        return -1;
    } else {
        control = controlConv;
    }

    errno = 0;
    long speedConv   = strtol(argv[3], &speedPointer, 10);

    if (errno != 0 || *speedPointer != '\0' || speedConv > INT_MAX || speedConv < INT_MIN) {
        printf("[ERROR] The option for the speed is not an integer!\n");
        return -1;
    } else {
        speed = speedConv;
    }

    errno = 0;
    long colorConv   = strtol(argv[4], &colorPointer, 10);

    if (errno != 0 || *colorPointer != '\0' || colorConv > INT_MAX || colorConv < INT_MIN) {
        printf("[ERROR] The option for the color is not an integer!\n");
        return -1;
    } else {
        color = colorConv;
    }

    errno = 0;
    long timesConv   = strtol(argv[5], &timesPointer, 10);

    if (errno != 0 || *timesPointer != '\0' || timesConv > INT_MAX || timesConv < INT_MIN) {
        printf("[ERROR] The option for times is not an integer!\n");
        return -1;
    } else {
        times = timesConv;
    }

    // Checking control variable
    if (control != 1 && control != 2 && control != 3 && control != 4 && control != 5 && control != 6) {
        printf("Control variable not set properly see %s --help for help\n", argv[0]);
        return -1;
    }

    // Checking speed variable
    if (speed > 255 || speed < 0) {
        printf("Speed variable not set properly see %s --help for help\n", argv[0]);
        return -1;
    }
    
    //Checking color variable
    if (color != 1 && color != 2 && color != 3) {
        printf("Color variable not set properly see %s --help for help\n", argv[0]);
        return -1;
    }

    // Checking times variable
    if (times > 255 || times < 0) {
        printf("Times variable not set properly see %s --help for help\n", argv[0]);
        return -1;
    }

    printf("[INFO]  Using port %s\n", argv[1]);
    printf("[INFO]  Initializing device communication\n");
    err = init_driver(argv[1], 0xFFFFFFFF, &driver);
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

    printf("[INFO]  Executing command with the following parameters: %d %d %d %d\n", control, speed, color, times);
    err = call_cmd(driver, AuraLedConfig, &reply, 4, control, speed, color, times);
    if (err != SUCCESS)
        goto error;

error:
    printf("[ERROR] Error raised: 0x%.2X\n", err);
    return err;
}