
#include <stdint.h>
#include <mbed.h>
#include <rtos.h>

#include "spislave_ext.h"
#include "pwm.h"
#include "wheel.h"
#include "ctrl_fsm.h"


DigitalOut          led1(LED3);
DigitalOut          led4(LED6);
ubot::SPISlaveExt   command_spi(PA_7, PA_6, PA_5, PA_4);
ubot::control::Fsm  control_fsm;


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


extern "C" void spi1_isr_handler (void)
{
    if (command_spi.receive()) {
        control_fsm.put(command_spi.read());
    }
}


int main()
{
    rtos::Thread led1_thread(led_thread_func, static_cast<void *>(&led1));

    ubot::control::event_t evt;

    mbed::DigitalOut ina(PB_1);
    mbed::DigitalOut inb(PB_2);
    ubot::Pwm pwm(PA_0);

    osStatus status;

    command_spi.reply(0x00);
    command_spi.format(8, 1);
    command_spi.enable_it(SPI_IT_RXNE);
    NVIC_EnableIRQ(SPI1_IRQn);

    ubot::Wheel wheel_left_front(pwm, ina, inb);

    do {
        status = control_fsm.get(evt);
        if (osEventMessage == status) {
            debug_toggle();

            if (evt.type == ubot::control::MSG_MOTOR_VELOCITY) {
                if (evt.vel.index == ubot::MOTOR_INDEX_FRONT_LEFT) {
                    wheel_left_front.set_velocity(evt.vel.value);
                }
            }

        } else {
            error("Failed to get control event. Reason: %d", status);
        }
    } while (true);

    led1_thread.terminate();

    return 0;
}

