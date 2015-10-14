
#ifndef EVENTLOOP_H
#define EVENTLOOP_H


#include <map>
#include <rtos.h>
#include <mbed.h>


namespace ubot
{

#ifndef CTRL_QUEUE_SIZE
#define CTRL_QUEUE_SIZE 16
#endif


typedef enum {
    ET_UNKNOWN = 0,
    ET_MOTOR1_VELOCITY = 1,
    ET_MOTOR1_ENCODER_UPD = 2
} event_type_t;


typedef union {
    int16_t velocity;
} event_payload_t;


typedef struct {
    event_type_t type;
    event_payload_t payload;
} event_t;


class EventPool
    : public rtos::MemoryPool<event_t, CTRL_QUEUE_SIZE>
{
};


class EventQueue
    : public rtos::Queue<event_t, CTRL_QUEUE_SIZE>
{
};


typedef FunctionPointerArg1<void, const event_t *> event_callback_t;


class EventLoop
{
private:
    EventPool _pool;
    EventQueue _queue;
    std::map<event_type_t, event_callback_t *> _callbacks;

public:
    EventLoop() {
    }

    void attach(event_type_t type, event_callback_t * callback);

    bool publish(const event_t * e);

    void iteration(void);
};

}

#endif // EVENTLOOP

