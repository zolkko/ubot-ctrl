
#ifndef __motor_defs_h__
#define __motor_defs_h__

namespace ubot
{

typedef enum {
    MOTOR_INDEX_UNDEFINED   = 0,

    MOTOR_INDEX_LEFT_FRONT  = 1,
    MOTOR_INDEX_LEFT_MID    = 2,
    MOTOR_INDEX_LEFT_BACK   = 3,

    MOTOR_INDEX_RIGHT_FRONT = 4,
    MOTOR_INDEX_RIGHT_MID   = 5,
    MOTOR_INDEX_RIGHT_BACK  = 6
} motor_index_t;

}

#endif

