
#include <mbed.h>
#include <USBHAL.h>
#include <USBEndpoints.h>
#include <USBRegs_STM32.h>

#include "usbif.h"


DigitalOut led4(PD_15);
static bool led4_on = false;


static uint32_t get_fifo_size(void) {
    volatile uint32_t status = OTG_FS->GREGS.GRXSTSP;
    volatile uint32_t endpoint = (status & 0xF) << 1;
    volatile uint32_t length = (status >> 4) & 0x7FF;
    volatile uint32_t type = (status >> 17) & 0xF;

    if (length > 0) {
        led4 = led4_on ? 0 : 1;
        led4_on = !led4_on;
    }

    return length;
}

bool ubot::UsbIf::EP1_OUT_callback() {
    uint32_t size = get_fifo_size();

    if (size) {
        return false;
    } else {
        return false;
    }
}


bool ubot::UsbIf::EP1_IN_callback() {
    uint32_t size = get_fifo_size();

    if (size) {
        return false;
    } else {
        return false;
    }
}
