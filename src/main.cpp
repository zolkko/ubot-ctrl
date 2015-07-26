
#include <stdint.h>
#include "mbed.h"
#include "Thread.h"

#include "app_conf.h"
#include "app.h"


DigitalOut led1(LED3);
DigitalOut led2(LED4);
DigitalOut led3(LED5);
DigitalOut led4(LED6);


void led_thread_func(void const *args)
{
    DigitalOut * out = (DigitalOut *) args;

    while (true) {
        if (out == (&led1)) {
            rtos::Thread::wait(1000);
        } else if (out == (&led2)) {
            rtos::Thread::wait(2000);
        } else if (out == (&led3)) {
            rtos::Thread::wait(3000);
        } else {
            rtos::Thread::wait(4000);
        }

        int value = out->read();
        if (value) {
            out->write(0);
        } else {
            out->write(1);
        }

        rtos::Thread::yield();
    }
}


int main()
{
    rtos::Thread led1_thread(led_thread_func, static_cast<void *>(&led1));
    rtos::Thread led2_thread(led_thread_func, static_cast<void *>(&led2));
    rtos::Thread led3_thread(led_thread_func, static_cast<void *>(&led3));
    rtos::Thread led4_thread(led_thread_func, static_cast<void *>(&led4));

    ubot::App app (
        MOTOR_LEFT_1_PIN, MOTOR_LEFT_2_PIN, MOTOR_LEFT_3_PIN, 
        MOTOR_RIGHT_1_PIN, MOTOR_RIGHT_2_PIN, MOTOR_RIGHT_3_PIN
    );
    app.run();

    while (true) {
    }

    led4_thread.terminate();
    led3_thread.terminate();
    led2_thread.terminate();
    led1_thread.terminate();

    return 0;
}

