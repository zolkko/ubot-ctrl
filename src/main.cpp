
#include <stdint.h>
#include "sys.h"
#include "mbed.h"
#include "Thread.h"

#include "app_conf.h"
#include "app.h"
#include "qei.h"


#define CMD_RESET (0)

#define CMD_M1 (0x01)
#define CMD_M2 (0x02)
#define CMD_M3 (0x03)
#define CMD_M4 (0x04)
#define CMD_M5 (0x05)
#define CMD_M6 (0x06)


DigitalOut led1(LED3);
DigitalOut led2(LED4);
DigitalOut led3(LED5);
DigitalOut led4(LED6);

SPISlave command_spi(PA_7, PA_6, PA_5, PA_4);

/*
Queue<uint8_t, 16> command_queue;
MemoryPool<uint8_t, 16> command_pool;
 */


/*ISR_HANDLER_SECTION
void spi_interrupt_handler (void)
{
    if (command_spi.receive()) {
        uint8_t data = command_spi.read();

        uint8_t * data_ptr = command_pool.alloc();

        if (data_ptr) {
            *data_ptr = data;
            if (command_queue.put(data_ptr) != osOK) {
                command_pool.free(data_ptr);
            }
        }
    }
}*/

/*
void command_acceptor(void const *args)
{
    osEvent evt;

    do {
        evt = command_queue.get();
        if (evt.status == osEventMessage) {
            uint8_t command = ((uint8_t *)evt.value.p);
            if (evt.value.p) {
                command_pool.free(command);
                evt.value.p = NULL;
            }
        }
    } while (true);
}*/


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


void debug_toggle(void)
{
    uint32_t value = led4.read();
    if (value) {
        led4.write(0);
    } else {
        led4.write(1);
    }
}


int main()
{
    rtos::Thread led1_thread(led_thread_func, static_cast<void *>(&led1));
    rtos::Thread led2_thread(led_thread_func, static_cast<void *>(&led2));
    rtos::Thread led3_thread(led_thread_func, static_cast<void *>(&led3));

    command_spi.reply(0xaa);
    command_spi.format(8);

    do {
        if (command_spi.receive()) {
            command_spi.read();
            debug_toggle();
        }
    } while (true);

    led3_thread.terminate();
    led2_thread.terminate();
    led1_thread.terminate();

    return 0;
}

