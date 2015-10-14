
#include <stdint.h>
#include <rtos.h>
#include <stdlib.h>
#include <map>
#include "eventloop.h"


void ubot::EventLoop::attach(ubot::event_type_t type, ubot::event_callback_t * callback)
{
    _callbacks[type] = callback;
}


extern "C" void debug_evt(void) {
    volatile int a = 10;
    do {
        a += 13;
    } while (false) ;
}


bool ubot::EventLoop::publish(const ubot::event_t * source_event)
{
    debug_evt();

    ubot::event_t * event = _pool.alloc();

    if (event) {
        memcpy(event, source_event, sizeof(ubot::event_t));

        osStatus status = _queue.put(event);
        if (osOK == status) {
            return true;
        } else {
            _pool.free(event);
        }
    }
    return false;
}


void ubot::EventLoop::iteration()
{
    osEvent os_event = _queue.get(osWaitForever);

    debug_evt();

    if (osEventMessage == os_event.status) {
        event_t * event_ptr = reinterpret_cast<event_t *>(os_event.value.p);

        if (event_ptr) {
            auto callback_iter = _callbacks.find(event_ptr->type);

            if (callback_iter != _callbacks.end()) {
                callback_iter->second->call(const_cast<const event_t *>(event_ptr));
            }

            if (osOK != _pool.free(event_ptr)) {
                error("Failed to release memory allocated for control message");
            }
        }
    }
}
