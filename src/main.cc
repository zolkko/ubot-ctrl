
#include <stdint.h>
#include <mbed.h>
#include <rtos.h>

#include "spislave_ext.h"
#include "pwm.h"


DigitalOut          led1(LED3);
DigitalOut          led4(LED6);
ubot::SPISlaveExt   command_spi(PA_7, PA_6, PA_5, PA_4);


void led_thread_func(void const *args)
{
    DigitalOut * out = (DigitalOut *) args;
    while (true) {
        rtos::Thread::wait(500);
        int value = out->read();
        if (value) { out->write(0); } else { out->write(1); }
    }
}


void debug_toggle(void)
{
    uint32_t value = led4.read();
    if (value) { led4.write(0); } else { led4.write(1); }
}


static volatile uint8_t spi_d;
rtos::Queue<uint8_t, 1> spi_q;


extern "C" void spi1_isr_handler (void)
{
    if (command_spi.receive()) {
        spi_d = command_spi.read();
        spi_q.put((unsigned char *)&spi_d);
    }
}


void spi_thread_func(void const * args)
{
    ubot::Pwm mpwm(PE_5);
    osEvent e;

    do {
        e = spi_q.get();
        if (e.status == osEventMessage) {
            uint8_t * d = (uint8_t *)e.value.p;
            debug_toggle();
        } else {
            error("Failed to get data form spi_q");
        }
    } while (true) ;
}


int main()
{
    rtos::Thread led1_thread(led_thread_func, static_cast<void *>(&led1));
    rtos::Thread spi_thread(spi_thread_func, nullptr, osPriorityNormal, 2048, nullptr);

    command_spi.reply(0x00);
    command_spi.format(8, 1);
    command_spi.enable_it(SPI_IT_RXNE);
    NVIC_EnableIRQ(SPI1_IRQn);

    do {
        rtos::Thread::yield();
    } while (true);

    spi_thread.terminate();
    led1_thread.terminate();

    return 0;
}

