
#include <stdint.h>
#include <mbed.h>
#include "pwm.h"
#include "wheel.h"
#include "motor_defs.h"


ubot::Wheel::Wheel(const ubot::Pwm& pwm, const mbed::DigitalOut& ina, const mbed::DigitalOut& inb)
    : _pwm(pwm),
      _ina(ina),
      _inb(inb),
      _direction(DIRECTION_NONE)
{
}


void ubot::Wheel::set_velocity(int16_t value)
{
    set_direction(value);

    if (value > MOTOR_MAX_SPEED) {
        value = MOTOR_MAX_SPEED;
    } else if (value < -MOTOR_MAX_SPEED) {
        value = -MOTOR_MAX_SPEED;
    }

    float dc = (value < 0.0f ? -value : value) / static_cast<float>(MOTOR_MAX_SPEED);
    _pwm.set_dc(dc);
}


int16_t ubot::Wheel::get_velocity(void)
{
    int16_t velocity = 0;

    // TODO: read actual velocity value from quad-encoder interface.

    if (_direction == DIRECTION_BACK) {
        velocity = -velocity;
    }

    return velocity;
}


void ubot::Wheel::set_direction(int16_t value)
{
    if (value == 0 && _direction != DIRECTION_NONE) {
        _ina = 0;
        _inb = 0;
        _direction = DIRECTION_NONE;
    } else if (value > 0 && _direction != DIRECTION_FORTH) {
        _ina = 1;
        _inb = 0;
        _direction = DIRECTION_FORTH;
    } else if (value < 0 && _direction != DIRECTION_BACK) {
        _ina = 0;
        _inb = 1;
        _direction = DIRECTION_BACK;
    }
}

