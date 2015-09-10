
#ifndef __wheel_h__
#define __wheel_h__


namespace ubot
{

typedef enum {
    DIRECTION_NONE,
    DIRECTION_BACK,
    DIRECTION_FORTH
} direction_t;


class Wheel
{
public:
    Wheel(const ubot::Pwm& pwm,
          const mbed::DigitalOut& ina,
          const mbed::DigitalOut& inb,
          const ubot::Enc& enc);

    void set_velocity(int16_t value);

    int16_t get_velocity(void);

private:

    void set_direction(int16_t value);

    Pwm _pwm;
    mbed::DigitalOut _ina;
    mbed::DigitalOut _inb;

    Enc _enc;

    direction_t _direction;
};

} // namespace ubot

#endif // __wheel_h__

