
#include <stdint.h>
#include <mbed.h>
#include <rtos.h>

#include "spislave_ext.h"
#include "pwm.h"
#include "enc.h"
#include "wheel.h"
#include "ctrl_fsm.h"


DigitalOut          led1(LED3);
DigitalOut          led4(LED6);

ubot::Enc enc(PA_0);


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


extern "C" void spi1_isr_handler (void)
{   
    uint32_t flags = command_spi.get_flags();

    if ((flags & SPI_FLAG_RXNE) == SPI_FLAG_RXNE) {
        uint8_t idata = command_spi.get();
        uint8_t odata = control_fsm.put(idata);
        command_spi.set(odata);
    }
}


extern "C" void tim2_isr_handler(void)
{
    enc.handle_it();
}


void encoder_read_thread(const void * params)
{
    while (true) {
        __disable_irq();
        control_fsm.set_speed(ubot::MOTOR_INDEX_FRONT_LEFT, enc.get_speed());
        __enable_irq();

        rtos::Thread::wait(10);
    }
}


namespace ubot
{

class App
{

public:
    void run(void) {
        do {
        } while (true);
    }

private:
    Wheel _lf;

};



}



int main()
{
    ubot::SPISlaveExt cmd_spi(PA_7, PA_6, PA_5, PA_4);

    mbed::DigitalOut ina(PB_1);
    mbed::DigitalOut inb(PB_2);
    ubot::Pwm pwm(PA_0);

    ubot::Wheel wheel_left_front(pwm, ina, inb, enc);

    ubot::control::event_t evt;
    osStatus status;


    rtos::Thread led1_thread(led_thread_func, static_cast<void *>(&led1));


    command_spi.enable_it(SPI_IT_RXNE);


    enc.enable_it();
    NVIC_EnableIRQ(TIM2_IRQn);


    do {
        status = control_fsm.get(evt);
        if (osEventMessage == status) {
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

