
#include <stdint.h>
#include <stdlib.h>
#include <mbed.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "usbd_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_core.h"
#include "usbd_cdc_vcp.h"

#ifdef __cplusplus
}
#endif

__ALIGN_BEGIN
USB_OTG_CORE_HANDLE USB_OTG_dev
__ALIGN_END;

DigitalOut led1(PD_13);
DigitalOut led2(PD_12);
DigitalOut led3(PD_14);

int main()
{
    __IO uint32_t i = 0;
    uint8_t buf[255];
    uint8_t len;

    USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_CDC_cb, &USR_cb);

    VCP_send_str( (uint8_t *) "USB serial DEMO\n");
    buf[0] = 't';
    buf[1] = 'e';
    buf[2] = 's';
    buf[3] = 't';
    buf[4] = 0;

    VCP_send_str(&buf[0]);
    buf[0] = 0;
    buf[1] = 0;
    buf[2] = 0;
    buf[3] = 0;
    buf[4] = 0;

    while(1) {
        len = VCP_get_string(&buf[0]);
        if(len) {
            VCP_send_str(&buf[0]);
        }

        if (i == 0x1000000) {
            VCP_put_char('b');
            led1 = 0;
            led2 = 0;
            led3 = 0;
        }

        if (i++ == 0x2000000) {
            i = 0;
            VCP_put_char('a');
            led1 = 1;
            led2 = 1;
            led3 = 1;
        }
    }

    return 0;
}
