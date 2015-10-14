
#ifndef UBOT_WHEEL_H
#define UBOT_WHEEL_H

#include <mbed.h>
#include <arm_math.h>

namespace ubot
{

class Wheel
{
private:
    ubot::Pwm _pwm;
    mbed::DigitalOut _ina;
    mbed::DigitalOut _inb;

    int16_t _velocity;
    arm_pid_instance_f32 _pid;

public:
    Wheel(const PinName pwm_pin, const PinName ina_pin, const PinName inb_pin);

    void set_velocity(int16_t value);

    void step(int16_t velocity);
};

} // namespace ubot

#endif // UBOT_WHEEL_H
