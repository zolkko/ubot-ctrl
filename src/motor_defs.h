
#ifndef __motor_defs_h__
#define __motor_defs_h__

namespace ubot
{

typedef enum {
    MOTOR_INDEX_UNDEFINED   = 0,

    MOTOR_INDEX_FRONT_LEFT  = 1,
    MOTOR_INDEX_MID_LEFT    = 2,
    MOTOR_INDEX_BACK_LEFT   = 3,

    MOTOR_INDEX_FRONT_RIGHT = 4,
    MOTOR_INDEX_MID_RIGHT   = 5,
    MOTOR_INDEX_BACK_RIGHT  = 6
} motor_index_t;


/*  @breif 15kHz motor PWM freq because of limitations of motor-driver IC */
#ifndef MOTOR_PWM_PERIOD
#define MOTOR_PWM_PERIOD_US  (67)
#endif

/* @breif defines maximum motor speed in mm/s. This is a limitation of used motors */
#ifndef MOTOR_MAX_SPEED
#define MOTOR_MAX_SPEED      (2000)
#endif


#ifndef ENC_PRESCALER_FREQ
#define ENC_PRESCALER_FREQ   (15000)
#endif


/*
 * The desired frequency must be multiplied by factor of two
 * because SYSCLK (depending on configuration) that clocks a timer
 * slower than PLL by two.
 */
#define ENC_TIMER_DIVIDER    (ENC_PRESCALER_FREQ * 2)


#define ENC_STEP_DISTANCE    (0.6944)
#define ENC_MAX_OVERFLOW     (3)


}

#endif

