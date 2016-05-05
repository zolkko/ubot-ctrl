#include <stdint.h>
#include <mbed.h>

#include <usbd_core.h>

#include "usbd_desc.h"
#include "hid_if.h"


USBD_HandleTypeDef USBD_Device;


DigitalOut led1(PD_13);
DigitalOut led2(PD_12);
DigitalOut led3(PD_14);
DigitalOut led4(PD_15);


static uint8_t data[25];


int main()
{
    USBD_Init(&USBD_Device, &VCP_Desc, 0);
    USBD_RegisterClass(&USBD_Device, &USBD_UBOT_HID);
    USBD_Start(&USBD_Device);

    // TODO: device must stay blocked unless connected

    int i = 0;

    while (true) {
        led1 = !led1;
        wait(1);
        led2 = !led2;
        wait(1);

        for (int j = 1; j < sizeof(data); j++) {
            data[j] = i;
        }

        // data[0] = 1;
        // USBD_UBOT_HID_SendReport(&USBD_Device, (uint8_t *)&data, 3);

        data[0] = 2;
        USBD_UBOT_HID_SendReport(&USBD_Device, (uint8_t *)&data, 25);

        if (i >= 100) {
            i = 0;
        } else {
            i++;
        }
    }

    return 0;
}
