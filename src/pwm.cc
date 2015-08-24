
#include "pwm.h"


void ubot::Pwm::set_dc(float value)
{
    uint32_t dc;
    TIM_TypeDef * tim = (TIM_TypeDef *)(_pwm.pwm);

    if (value > 1.0f) {
        dc = _pwm.period;
    } else if (value < 0.0f) {
        dc = 0;
    } else {
        dc = (uint32_t)(_pwm.period * value);
    }

    switch (_pwm.channel) {
        case 1:
            tim->CCR1 = dc;
            break;

        case 2:
            tim->CCR2 = dc;
            break;

        case 3:
            tim->CCR3 = dc;
            break;

        default:
            tim->CCR4 = dc;
            break;
    }
}

