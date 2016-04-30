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


int main()
{
    USBD_Init(&USBD_Device, &VCP_Desc, 0);
    USBD_RegisterClass(&USBD_Device, USBD_CDC_CLASS);
    USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_Template_fops);
    USBD_Start(&USBD_Device);

    NVIC_EnableIRQ(OTG_FS_IRQn);

    while (true) {
        led1 = !led1;
        wait(1);
        led2 = !led2;
        wait(1);
    }

    return 0;
}
