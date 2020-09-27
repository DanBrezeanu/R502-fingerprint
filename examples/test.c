#include <r502.h>

int main() {
    Driver *driver;
    Reply reply;
    int32_t err = SUCCESS;

    err = init_driver("/dev/ttyS0", 0xFFFFFFFF, &driver);
    if (err != SUCCESS)
        goto error;

    err = call_cmd(driver, TemplateNum, &reply, 0);
    if (err != SUCCESS)
        goto error;

    printf("%.2X %d\n", reply.conf_code, reply.body.template_num.index);

    uint16_t new_index = reply.body.template_num.index;

    call_cmd(driver, GenImg, &reply, 0);
    if (err != SUCCESS)
        goto error;
    printf("%.2X\n", reply.conf_code);

    call_cmd(driver, Img2Tz, &reply, 1, 1);
    if (err != SUCCESS)
        goto error;
    printf("%.2X\n", reply.conf_code);

    sleep(2);

    call_cmd(driver, GenImg, &reply, 0);
    if (err != SUCCESS)
        goto error;

    printf("%.2X\n", reply.conf_code);

    call_cmd(driver, Img2Tz, &reply, 1, 2);
    if (err != SUCCESS)
        goto error;
    printf("%.2X\n", reply.conf_code);

    call_cmd(driver, RegModel, &reply, 0);
    if (err != SUCCESS)
        goto error;
    printf("%.2X\n", reply.conf_code);

    call_cmd(driver, Store, &reply, 2, 1, new_index);
    if (err != SUCCESS)
        goto error;

    printf("%.2X\n", reply.conf_code);

error:
    printf("0x%.2X\n", err);
    return 0;
}
