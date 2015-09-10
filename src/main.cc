
#include <stdint.h>
#include <mbed.h>
#include <rtos.h>

#include "spislave_ext.h"


DigitalOut          led4(LED6);


void led_thread_func(void const *args)
{
    DigitalOut * out = (DigitalOut *) args;
    while (true) {
        rtos::Thread::wait(500);
        int value = out->read();
        if (value) { out->write(0); } else { out->write(1); }
    }
}


extern "C" void debug_toggle(void)
{
    uint32_t value = led4.read();
    if (value) {
        led4.write(0);
    } else {
        led4.write(1);
    }
}


/*extern "C" void tim2_isr_handler(void)
{
    enc.handle_it();
}*/


int main()
{
    DigitalOut led1(LED3);
    rtos::Thread led1_thread(led_thread_func, static_cast<void *>(&led1));

    osStatus status;
    ubot::control::event_t evt;
    ubot::Control ctrl(PA_7, PA_6, PA_5, PA_4);

    /*
    mbed::DigitalOut ina(PB_1);
    mbed::DigitalOut inb(PB_2);
    ubot::Pwm pwm(PA_0);

    ubot::Wheel wheel_left_front(pwm, ina, inb, enc);

    command_spi.enable_it(SPI_IT_RXNE);

    enc.enable_it();
    NVIC_EnableIRQ(TIM2_IRQn);
    */

    ctrl.enable_irq();

    do {
        status = ctrl.get_event(evt);
        if (osEventMessage == status) {
            if (evt.type == ubot::control::MSG_MOTOR_VELOCITY) {
                if (evt.vel.index == ubot::MOTOR_INDEX_FRONT_LEFT) {
                    // wheel_left_front.set_velocity(evt.vel.value);
                }
            }
        } else {
            error("Failed to get control event. Reason: %d", status);
        }
    } while (true);

    ctrl.disable_irq();

    led1_thread.terminate();

    return 0;
}

