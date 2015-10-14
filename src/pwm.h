
#ifndef UBOT_PWM_H
#define UBOT_PWM_H

namespace ubot
{

class Pwm : mbed::PwmOut
{
public:
    Pwm(PinName pin);

    void set_dc(float value);
};

}

#endif // UBOT_PWM_H
