
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

}

#endif

