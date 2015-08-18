
#ifndef __qie_h__
#define __qie_h__

#include "platform.h"

#include <stm32f4xx.h>
#include <stm32f4xx_hal_gpio.h>
#include <stm32f4xx_hal_rcc.h>
#include <stm32f4xx_hal_tim.h>


namespace ubot
{


typedef enum qei_direction
{
    FORWARD,
    BACKWARD
} qei_direction_t;


typedef enum qei_encoder_mode {
    TI1 = TIM_ENCODERMODE_TI1,
    TI2 = TIM_ENCODERMODE_TI2,
    TI12 = TIM_ENCODERMODE_TI12
} qei_encoder_mode_t;


class Qei
{
public:
    Qei() {
        init_gpio();

        __TIM2_CLK_ENABLE();

        tim.Instance = TIM2;
        tim.Init.CounterMode = TIM_COUNTERMODE_UP;
        tim.Init.Period = 0xffff;
        tim.Init.Prescaler = 0;
        tim.Init.RepetitionCounter = 0;
        tim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

        // init encoder
        enc.EncoderMode = TIM_ENCODERMODE_TI12;
 
        enc.IC1Filter = 0x0F;
        enc.IC1Polarity = TIM_INPUTCHANNELPOLARITY_RISING; // BOTHEDGE; // TIM_INPUTCHANNELPOLARITY_RISING;
        enc.IC1Prescaler = TIM_ICPSC_DIV1;
        enc.IC1Selection = TIM_ICSELECTION_DIRECTTI;
 
        enc.IC2Filter = 0x0F;
        enc.IC2Polarity = TIM_INPUTCHANNELPOLARITY_RISING; // TIM_INPUTCHANNELPOLARITY_RISING;
        enc.IC2Prescaler = TIM_ICPSC_DIV1;
        enc.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    }

    bool enable(void) {
        if (HAL_TIM_Encoder_Init(&tim, &enc) != HAL_OK) {
            return false;
        }
 
        // TIM_CHANNEL_1 | TIM_CHANNEL_2
        if (HAL_TIM_Encoder_Start(&tim, TIM_CHANNEL_ALL) != HAL_OK) {
            return false;
        }

        return true;
    }

    uint16_t count() const {
        return static_cast<uint16_t>(tim.Instance->CNT);
    }

    qei_direction_t direction() const {
        return (tim.Instance->CR1) & TIM_CR1_DIR ? FORWARD : BACKWARD;
    }

private:

    void init_gpio(void) const {
        GPIO_InitTypeDef GPIO_InitStructure;

        __GPIOA_CLK_ENABLE();

        GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;
        GPIO_InitStructure.Pull = GPIO_NOPULL;
        GPIO_InitStructure.Alternate = GPIO_AF1_TIM2;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
    }


    TIM_HandleTypeDef tim;
    TIM_Encoder_InitTypeDef enc;
};

}

#endif

