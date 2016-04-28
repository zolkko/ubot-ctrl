
#include <stdint.h>
#include <mbed.h>
#include "USBHID.h"
#include "usbif.h"

DigitalOut led1(PD_13);

DigitalOut led2(PD_12);
bool led2_on = false;

DigitalOut led3(PD_14);
bool led3_on = false;

HID_REPORT send_report;
HID_REPORT recv_report;

#define SIZE 2

static uint8_t data[SIZE];

int main()
{
    ubot::UsbIf hid(SIZE, SIZE);
    send_report.length = SIZE;

    do {
        // Await usb connection
    } while (!hid.configured());

    led1 = 1;

    while (1) {
        //try to read a msg
        if(hid.read(&recv_report)) {
            if (recv_report.length > 0) {
                led2 = led2_on ? 0 : 1;
                led2_on != led2_on;

                for (uint32_t i = 0; i < recv_report.length; i++) {
                    data[i] = recv_report.data[i];
                }
            }

            //Fill the report
            for (uint32_t i = 0; i < send_report.length; i++) {
                send_report.data[i] = data[i];
            }

            //Send the report
            if (hid.send(&send_report)) {
                // led3 = led3_on ? 0 : 1;
                // led3_on != led3_on;
            }
        }
    }

    return 0;
}
