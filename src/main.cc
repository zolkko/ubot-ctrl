#include <stdint.h>
#include <mbed.h>

#include <usbd_core.h>
#include <usbd_cdc_if.h>

#include "usbd_desc.h"


USBD_HandleTypeDef USBD_Device;


DigitalOut led1(PD_13);
DigitalOut led2(PD_12);
DigitalOut led3(PD_14);
DigitalOut led4(PD_15);


static uint8_t data[10];
static uint16_t sz = 0;


int main()
{
    USBD_Init(&USBD_Device, &VCP_Desc, 0);
    USBD_RegisterClass(&USBD_Device, USBD_CDC_CLASS);
    USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_Template_fops);
    USBD_Start(&USBD_Device);

    data[0] = 't';
    data[1] = 'e';
    data[2] = 's';
    data[3] = 't';
    data[4] = '\r';
    data[5] = '\n';
    sz = 6;

    while (true) {
        led1 = !led1;
        wait(1);
        led2 = !led2;
        wait(1);

        // TODO: check that usb is connected
        USBD_CDC_SetTxBuffer(&USBD_Device, (uint8_t *) &data, sz);
        if (USBD_CDC_TransmitPacket(&USBD_Device) == USBD_OK) {
            data[0] = 'Y';
            data[1] = 'e';
            data[2] = 's';
            data[3] = '\r';
            data[4] = '\n';
            sz = 5;
        } else {
            data[0] = 'N';
            data[1] = 'o';
            data[2] = '\r';
            data[3] = '\n';
            sz = 4;
        }
    }

    return 0;
}
