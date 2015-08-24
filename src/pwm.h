
#ifndef __pwm_h__
#define __pwm_h__


#include <mbed.h>


namespace ubot
{

class Pwm : mbed::PwmOut
{
public:
    Pwm(PinName pin) : mbed::PwmOut(pin) {
    }

    void set_dc(float value);
};

}

#endif

