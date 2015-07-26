#include "mbed.h"
#include "Thread.h"


void some_thread(void const *args)
{
    int i, j = 0;

    while (true) {
        for (i = 0; i < 100; i++) {
            j += i + 3;
        }
        i = j;
    }
}


int main()
{
    rtos::Thread thread(some_thread);

    while (true) {
    }

    thread.terminate();

    return 0;
}

