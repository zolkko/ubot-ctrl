
#include <stdint.h>
#include <mbed.h>
#include <rtos.h>
#include <map>

#include "eventloop.h"
#include "control.h"
#include "pwm.h"
#include "enc.h"
#include "wheel.h"


namespace ubot
{

class MotorController
{
private:
    uint8_t _index;
    Enc * _enc;
    Wheel * _wheel;
    Control * _ctrl;

public:
    MotorController(uint8_t index, Enc * enc, Wheel * wheel, Control * ctrl)
        : _index(index),
          _enc(enc),
          _wheel(wheel),
          _ctrl(ctrl) {
    }

    void step(void) {
        int16_t velocity = _enc->get_velocity();
        _wheel->step(velocity);
        _ctrl->set_velocity(_index, velocity);
    }

    void on_velocity(const event_t * event) {
        _wheel->set_velocity(event->payload.velocity);
    }
};


void motor_feedback(const void * data)
{
    MotorController * feedback = reinterpret_cast<MotorController *>(const_cast<void *>(data));
    do {
        rtos::Thread::wait(100);
        feedback->step();
    } while (true);
}

}


int main()
{
    ubot::EventLoop event_loop;
    ubot::Control ctrl(PA_7, PA_6, PA_5, PA_4, &event_loop);

    ubot::Enc enc1(PA_0);
    ubot::Wheel wheel1(PA_1, PB_1, PB_2);
    ubot::MotorController motor1_feedback(0, &enc1, &wheel1, &ctrl);
    ubot::event_callback_t motor1_callback(&motor1_feedback, &ubot::MotorController::on_velocity);
    event_loop.attach(ubot::ET_MOTOR1_VELOCITY, &motor1_callback);

    enc1.enable_irq();
    ctrl.enable_irq();

    rtos::Thread motor_feedback_thread(ubot::motor_feedback, &motor1_feedback);

    do {
        event_loop.iteration();
    } while (true);

    motor_feedback_thread.terminate();

    ctrl.disable_irq();
    enc1.disable_irq();

    return 0;
}
