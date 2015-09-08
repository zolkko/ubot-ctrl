
#include <stdint.h>
#include <stdlib.h>
#include <rtos.h>
#include <mbed_error.h>
#include "ctrl_fsm.h"
#include "crc8.h"


extern "C" void debug_toggle(void);


namespace ubot
{

namespace control
{


typedef struct {
    state_index_t next;
    state_index_t error;
} state_t;


static const state_t fsm[] = {
    {STATE_INDEX_CMD_MSB,  STATE_INDEX_CMD_READ},
    {STATE_INDEX_CMD_LSB,  STATE_INDEX_CMD_READ},
    {STATE_INDEX_CMD_CRC,  STATE_INDEX_CMD_READ},
    {STATE_INDEX_CMD_READ, STATE_INDEX_CMD_READ},
    {STATE_INDEX_MFL_CRC,  STATE_INDEX_CMD_READ}, // STATE_INDEX_MFL_LSB
    {STATE_INDEX_CMD_READ, STATE_INDEX_CMD_READ}  // STATE_INDEX_MFL_CRC
};


uint8_t Fsm::put(uint8_t input_data)
{
    const state_t * curr = &fsm[_state];
    uint8_t result = 0;

    switch (_state) {
        case STATE_INDEX_CMD_READ:
            {
                switch (static_cast<command_t>(input_data)) {
                    case MOTOR_FRONT_LEFT_VELOCITY:
                        _event = _pool.alloc();
                        if (_event) {
                            _event->type = MSG_MOTOR_VELOCITY;
                            _event->crc = ubot::crc8(CRC8_INITIAL, input_data);
                            _event->vel.index = ubot::MOTOR_INDEX_FRONT_LEFT;
                            _event->vel.value = 0;

                            _state = STATE_INDEX_CMD_MSB;
                        } else {
                            error("Failed to allocate memory for new control event");
                            _state = curr->error;
                        }
                        return result;

                    case MOTOR_FRONT_LEFT_READ_SPEED:
                        result = static_cast<uint8_t>((_speed[ubot::MOTOR_INDEX_FRONT_LEFT - 1] & 0xff00) >> 8);
                        _crc_out = ubot::crc8(CRC8_INITIAL, result);
                        _state = STATE_INDEX_MFL_LSB;
                        return result;

                    default:
                        _state = curr->error;
                        return result;
                }
            }
            break;

        case STATE_INDEX_CMD_MSB:
            _event->crc = ubot::crc8(_event->crc, input_data);
            _event->vel.value |= static_cast<int16_t>(input_data << 8);
            break;

        case STATE_INDEX_CMD_LSB:
            {
                _event->crc = ubot::crc8(_event->crc, input_data);
                _event->vel.value |= static_cast<int16_t>(input_data);
            }
            break;

        case STATE_INDEX_CMD_CRC:
            {
                _event->crc = ubot::crc8(_event->crc, input_data);

                osStatus status = _queue.put(_event);
                if (osOK != status) {
                    _pool.free(_event);
                    _event = nullptr;

                    error("Failed to send new control event. Reason: %d", status);
                    _state = curr->error;

                    return result;
                }

                _event = nullptr;
            }
            break;

        case STATE_INDEX_MFL_LSB:
            {
                result = (uint8_t)(_speed[ubot::MOTOR_INDEX_FRONT_LEFT - 1] & 0xff);
                _crc_out = ubot::crc8(_crc_out, result);
            }
            break;

        case STATE_INDEX_MFL_CRC:
            {
                result = _crc_out;
                debug_toggle();
            }
            break;

        default:
            _state = curr->error;
            return result;
    }

    _state = curr->next;

    return result;
}


osStatus Fsm::get(event_t& event)
{
    osEvent e = _queue.get(osWaitForever);

    if (osEventMessage == e.status) {
        event_t * event_ptr = reinterpret_cast<event_t *>(e.value.p);

        if (event_ptr) {
            memcpy(&event, event_ptr, sizeof(event));

            if (osOK != _pool.free(event_ptr)) {
                error("Failed to release memory allocated for control message");
            }

            if (event.crc != 0x00) {
                return osErrorOS;
            }
        }
    }

    return e.status;
}

} // namespace control

} // namespace ubot

