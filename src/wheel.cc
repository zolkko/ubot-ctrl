
#include <stdint.h>
#include <mbed.h>
#include <arm_math.h>
#include <cmath>
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


ubot::Wheel::Wheel(const PinName pwm_pin, const PinName ina_pin, const PinName inb_pin)
    : _pwm(pwm_pin),
      _ina(ina_pin),
      _inb(inb_pin),
      _velocity(0)
{
    _pid.Kp = MOTOR_PID_PARAM_KP;
    _pid.Ki = MOTOR_PID_PARAM_KI;
    _pid.Kd = MOTOR_PID_PARAM_KD;

    arm_pid_init_f32(&_pid, 1);
}


void ubot::Wheel::set_velocity(int16_t value)
{
    if (value > MOTOR_MAX_SPEED) {
        value = MOTOR_MAX_SPEED;
    } else if (value < (-MOTOR_MAX_SPEED)) {
        value = -MOTOR_MAX_SPEED;
    }

    if (value == 0 && _velocity != 0) {
        _ina = 0;
        _inb = 0;
    } else if (value > 0 && _velocity <= 0) {
        _ina = 1;
        _inb = 0;
    } else if (value < 0 && _velocity >= 0) {
        _ina = 0;
        _inb = 1;
    }

    float dc = abs(value) / static_cast<float>(MOTOR_MAX_SPEED);
    _pwm.set_dc(dc);

    _velocity = value;

    arm_pid_reset_f32(&_pid);
}


void ubot::Wheel::step(int16_t velocity)
{
    float pid_error = abs(_velocity) - abs(velocity);
    float velocity_resp = arm_pid_f32(&_pid, pid_error);
    float duty;

    if (velocity_resp < 0.0f) {
        duty = 0.0f;
    } else if (velocity_resp >= static_cast<float>(MOTOR_MAX_SPEED)) {
        duty = 1.0f;
    } else {
        duty = velocity_resp / static_cast<float>(MOTOR_MAX_SPEED);
    }

    // _pwm.set_dc(duty);
}
