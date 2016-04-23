
#include <stdint.h>
#include <mbed.h>
#include <USBSerial.h>


DigitalOut led1(PD_13);
DigitalOut led2(PD_12);
DigitalOut led3(PD_14);
DigitalOut led4(PD_15);

int main()
{
    USBSerial serial;

    while (true) {
        led1 = 1;
        led2 = 0;
        led3 = 1;
        led4 = 1;

        wait(0.5f);

        led1 = 0;
        led2 = 1;
        led3 = 0;
        led4 = 0;

        wait(0.5f);

        serial.printf("MBED usb-serial is working...\n");
    }

    return 0;
}
