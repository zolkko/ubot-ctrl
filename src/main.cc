
#include <stdint.h>
#include "sys.h"

#include <mbed.h>
#include <InterruptManager.h>
#include <rtos.h>

#include "app_conf.h"
#include "app.h"
#include "spislave_ext.h"
#include "qei.h"


DigitalOut led1(LED3);
DigitalOut led2(LED4);
DigitalOut led3(LED5);
DigitalOut led4(LED6);


ubot::SPISlaveExt command_spi(PA_7, PA_6, PA_5, PA_4);
rtos::Queue<uint8_t, 16> command_queue;
rtos::MemoryPool<uint8_t, 16> command_pool;


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


void command_acceptor(void const *args)
{
    osEvent evt;

    do {
        evt = command_queue.get();
        if (evt.status == osEventMessage) {
            uint8_t * command_ptr = (uint8_t *)(evt.value.p);
            volatile uint8_t command = *command_ptr;

            if (command_ptr) {
                command_pool.free(command_ptr);
                evt.value.p = NULL;
            }

            debug_toggle();
        }
    } while (true);
}


extern "C" void spi1_isr_handler (void)
{
    if (command_spi.receive()) {
        volatile uint8_t data = command_spi.read();

        uint8_t * data_ptr = command_pool.alloc();
        if (data_ptr) {
            *data_ptr = data;
            if (command_queue.put(data_ptr) != osOK) {
                command_pool.free(data_ptr);
            }
        }
    }
}


int main()
{
    rtos::Thread led1_thread(led_thread_func, static_cast<void *>(&led1));
    rtos::Thread led2_thread(led_thread_func, static_cast<void *>(&led2));
    rtos::Thread led3_thread(led_thread_func, static_cast<void *>(&led3));
    rtos::Thread cmd_thread(command_acceptor, nullptr);

    command_spi.reply(0x00);
    command_spi.format(8, 1);
    command_spi.enable_it(SPI_IT_RXNE);

    // InterruptManager::get()->add_handler(command_spi_handler, SPI1_IRQn);
    // NVIC_SetVector(SPI1_IRQn, (uint32_t) command_spi_handler);
    NVIC_SetPriority(SPI1_IRQn, 2);
    NVIC_EnableIRQ(SPI1_IRQn);

    do {
        //
    } while (true);

    cmd_thread.terminate();
    led3_thread.terminate();
    led2_thread.terminate();
    led1_thread.terminate();

    return 0;
}

