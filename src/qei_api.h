
#ifndef __qei_api_h__
#define __qei_api_h__

#include "device.h"

#if DEVICE_QEI

#ifdef __cplusplus
extern "C" {
#endif


typedef struct qei_s {
    //
} qei_t;


typedef qei_dir_e {
    BACK,
    FORTH
} qei_dir_t;


/**
 * For STM32F4 only TIM1, TIM8, TIM2, TIM3, TIM4, TIM5
 * timers can work in encoder mode.
 */
void qei_init(qei_t* obj);
void qei_free(qei_t* obj);

/**
 * Get current encoder count.
 * TIM_X->CNT;
 */
uint32_t qei_read(qei_t* obj);
uint32_t qei_reset(qei_t* obj);


#ifdef __cplusplus
}
#endif

#endif // DEVICE_QEI

#endif // __qei_api_h__

