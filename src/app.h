
#ifndef __app_h__
#define __app_h__


namespace ubot
{

class App
{
public:
    App(const PinName ml1p, const PinName ml2p, const PinName ml3p,
        const PinName mr1p, const PinName mr2p, const PinName mr3p) :

            motor_left_1(ml1p),
            motor_left_2(ml2p),
            motor_left_3(ml3p),

            motor_right_1(mr1p),
            motor_right_2(mr2p),
            motor_right_3(mr3p)
    {
    }

    inline void run(void) const {
        while (true) {
        }
    }

private:

    inline void init_pwm(void) {
        motor_left_1.period_us(PWM_PERIOD_US);
        motor_left_1.write(PWM_DEFAULT_DUTY_CYCLE);

        motor_left_2.period_us(PWM_PERIOD_US);
        motor_left_2.write(PWM_DEFAULT_DUTY_CYCLE);

        motor_left_3.period_us(PWM_PERIOD_US);
        motor_left_3.write(PWM_DEFAULT_DUTY_CYCLE);

        motor_right_1.period_us(PWM_PERIOD_US);
        motor_right_1.write(PWM_DEFAULT_DUTY_CYCLE);

        motor_right_2.period_us(PWM_PERIOD_US);
        motor_right_2.write(PWM_DEFAULT_DUTY_CYCLE);

        motor_right_3.period_us(PWM_PERIOD_US);
        motor_right_3.write(PWM_DEFAULT_DUTY_CYCLE);
    }

    PwmOut motor_left_1;
    PwmOut motor_left_2;
    PwmOut motor_left_3;

    PwmOut motor_right_1;
    PwmOut motor_right_2;
    PwmOut motor_right_3;
};

};

#endif

