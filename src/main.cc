
#include <stdint.h>
#include <mbed.h>
#include "USBHID.h"
#include "usbif.h"

DigitalOut led1(PD_13);
bool led1_on = false;

DigitalOut led2(PD_12);
bool led2_on = false;

DigitalOut led3(PD_14);
DigitalOut led4(PD_15);


HID_REPORT send_report;
HID_REPORT recv_report;


int main()
{
    ubot::UsbIf hid(64, 64);
    send_report.length = 64;

    uint8_t data = 123;

    do {
        // Await usb connection
    } while (!hid.configured());

    led3 = 1;

    while (1) {
        //try to read a msg
        if(hid.read(&recv_report)) {
            if (recv_report.length > 0) {
                led1 = led1_on ? 0 : 1;
                led1_on != led1_on;

                data = recv_report.data[0];
            }

            //Fill the report
            for (int i = 0; i < send_report.length; i++) {
                send_report.data[i] = data;
            }

            //Send the report
            if (hid.send(&send_report)) {
                // led2 = led2_on ? 0 : 1;
                // led2_on != led2_on;
            }
        }
    }

    return 0;
}
