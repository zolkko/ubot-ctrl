
#ifndef __pwm_h__
#define __pwm_h__

namespace ubot
{

class Pwm : mbed::PwmOut
{
public:
    Pwm(PinName pin);

    void set_dc(float value);
};

}

#endif

