
#ifndef __ctrl_fsm_h__
#define __ctrl_fsm_h__


#include <rtos.h>
#include "motor_defs.h"


namespace ubot
{

namespace control
{

#ifndef CTRL_QUEUE_SIZE
#define CTRL_QUEUE_SIZE 16
#endif


typedef enum {
    STATE_INDEX_RESET    = 0,
    STATE_INDEX_CMD_READ = 1,
    STATE_INDEX_CMD_MSB  = 2,
    STATE_INDEX_CMD_LSB  = 3
} state_index_t;


typedef enum {
    MOTOR_FRONT_LEFT_VELOCITY = 1
} command_t;


typedef enum {
    MSG_UNKNOWN = 0,
    MSG_MOTOR_VELOCITY = 1
} message_t;


typedef struct {
    message_t type;
    struct {
        ubot::motor_index_t index;
        int16_t value;
    } vel;
} event_t;


class EventPool
    : public rtos::MemoryPool<event_t, CTRL_QUEUE_SIZE>
{
};


class EventQueue
    : public rtos::Queue<event_t, CTRL_QUEUE_SIZE>
{
};

class Fsm
{
public:
    Fsm()
        : _state(STATE_INDEX_CMD_READ),
          _event(nullptr)
    {
    }

    void put(const uint8_t input_data);

    osStatus get(event_t& event);

protected:
    state_index_t _state;
    event_t * _event;

    EventPool _pool;
    EventQueue _queue;
};

}

}

#endif

