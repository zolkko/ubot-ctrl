
#ifndef __ctrl_fsm_h__
#define __ctrl_fsm_h__


#include <rtos.h>
#include "motor_defs.h"
#include "crc8.h"


namespace ubot
{

namespace control
{

#ifndef CTRL_QUEUE_SIZE
#define CTRL_QUEUE_SIZE 16
#endif


typedef enum {
    STATE_INDEX_CMD_READ = 0,
    STATE_INDEX_CMD_MSB  = 1,
    STATE_INDEX_CMD_LSB  = 2,
    STATE_INDEX_CMD_CRC  = 3,
    STATE_INDEX_MFL_LSB  = 4,
    STATE_INDEX_MFL_CRC  = 5
} state_index_t;


typedef enum {
    MOTOR_FRONT_LEFT_VELOCITY = 1,
    MOTOR_FRONT_LEFT_READ_SPEED = 2,
} command_t;


typedef enum {
    MSG_UNKNOWN = 0,
    MSG_MOTOR_VELOCITY = 1
} message_t;


typedef struct {
    message_t type;
    uint8_t crc;
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
          _event(nullptr),
          _crc_out(0)
    {
        _speed[0] = 0x22bb;
        _speed[1] = 0;
        _speed[2] = 0;
        _speed[3] = 0;
        _speed[4] = 0;
        _speed[5] = 0;
    }

    uint8_t put(const uint8_t input_data);

    void set_speed(motor_index_t index, int16_t value)
    {
        _speed[index - 1] = value;
    }

    osStatus get(event_t& event);

protected:
    state_index_t _state;
    event_t * _event;

    EventPool _pool;
    EventQueue _queue;

    int16_t _speed[6];
    uint8_t _crc_out;
};

}

}

#endif

