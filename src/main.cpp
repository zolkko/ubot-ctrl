
#include <stdint.h>
#include "mbed.h"
#include "Thread.h"

uint16_t do_work(uint16_t value)
{
    return value + 1;
}


void thread1_func(void const *args)
{
    uint16_t i = 0;

    while (true) {
        if (i > 100) {
            i = 0;
        } else {
            i = do_work(i);
        }

        rtos::Thread::yield();
    }
}


void thread2_func(void const *args)
{
    uint16_t j = 0;
    while (true) {
        if (j > 100) {
            j = 0;
        } else {
            j = do_work(j);
        }

        rtos::Thread::yield();
    }
}


int main()
{
    rtos::Thread thread1(thread1_func);
    rtos::Thread thread2(thread2_func);

    while (true) {
    }

    thread2.terminate();
    thread1.terminate();

    return 0;
}

