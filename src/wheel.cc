
#include <stdint.h>
#include <mbed.h>
#include <arm_math.h>
#include "pwm.h"
#include "enc.h"
#include "wheel.h"
#include "motor_defs.h"


#ifndef MOTOR_PID_PARAM_KP
#error  MOTOR_PID_PARAM_KP proportional coeficient must be set
#endif

#ifndef MOTOR_PID_PARAM_KI
#error  MOTOR_PID_PARAM_KI integral coeficient must be set
#endif


#ifndef MOTOR_PID_PARAM_KD
#error  MOTOR_PID_PARAM_KD derivative coeficient must be set
#endif


ubot::Wheel::Wheel(const ubot::Pwm& pwm,
        const mbed::DigitalOut& ina,
        const mbed::DigitalOut& inb,
        const ubot::Enc& enc)
    : _pwm(pwm),
      _ina(ina),
      _inb(inb),
      _enc(enc),
      _direction(DIRECTION_NONE)
{
    _pid.Kp = MOTOR_PID_PARAM_KP;
    _pid.Ki = MOTOR_PID_PARAM_KI;
    _pid.Kd = MOTOR_PID_PARAM_KD;

    arm_pid_init_f32(&_pid, 1);
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
    int16_t velocity = 0; // _enc.get();

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


void ubot::Wheel::step(void)
{
    float pid_error = 0.0f;
    arm_pid_f32(&_pid, pid_error);
}

