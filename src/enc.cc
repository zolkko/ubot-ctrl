
#include <cmsis.h>
#include <mbed.h>
#include <PinNames.h>
#include "pinmap.h"
#include "enc.h"


typedef enum {
#if defined(TIM8_BASE)
    ENC_8  = (int)TIM8_BASE,
#endif

#if defined(TIM12_BASE)
    ENC_12 = (int)TIM12_BASE,
#endif

#if defined(TIM13_BASE)
    ENC_13 = (int)TIM13_BASE,
#endif

#if defined(TIM14_BASE)
    ENC_14 = (int)TIM14_BASE,
#endif

    ENC_1  = (int)TIM1_BASE,
    ENC_2  = (int)TIM2_BASE,
    ENC_3  = (int)TIM3_BASE,
    ENC_4  = (int)TIM4_BASE,

    ENC_9  = (int)TIM9_BASE,
    ENC_10 = (int)TIM10_BASE,
    ENC_11 = (int)TIM11_BASE

} ENCName;


static const PinMap PinMap_ENC[] = {
    {PA_0,  ENC_2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2,  1, 0)},
    // {PA_0,  ENC_5,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM5,  1, 0)},
    // {PA_0,  ENC_8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8,  0, 0)}, // TIM8_ETR
    {NC,    NC,    0}
};


ubot::Enc::Enc(const PinName pin)
    : _channel(0)
{
    ENCName enc = static_cast<ENCName>(pinmap_peripheral(pin, PinMap_ENC));
    MBED_ASSERT(enc != (ENCName)NC);

    // Configure and enable GPIO
    pinmap_pinout(pin, PinMap_ENC);

    // Enable TIM clock
    if (enc == ENC_1) __TIM1_CLK_ENABLE();
    if (enc == ENC_2) __TIM2_CLK_ENABLE();
    if (enc == ENC_3) __TIM3_CLK_ENABLE();
    if (enc == ENC_4) __TIM4_CLK_ENABLE();

#if defined(TIM8_BASE)
    if (enc == ENC_8) __TIM8_CLK_ENABLE();
#endif

    if (enc == ENC_9) __TIM9_CLK_ENABLE();

    if (enc == ENC_10) __TIM10_CLK_ENABLE();
    if (enc == ENC_11) __TIM11_CLK_ENABLE();

#if defined(TIM12_BASE)
    if (enc == ENC_12) __TIM12_CLK_ENABLE();
#endif

#if defined(TIM13_BASE)
    if (enc == ENC_13) __TIM13_CLK_ENABLE();
#endif

#if defined(TIM14_BASE)
    if (enc == ENC_14) __TIM14_CLK_ENABLE();
#endif

    uint32_t function = pinmap_function(pin, PinMap_ENC);
    _channel = STM_PIN_CHANNEL(function);

    _tim.Instance = reinterpret_cast<TIM_TypeDef *>(enc);
    _tim.Init.Period = 0xffff;
    _tim.Init.Prescaler = 0;
    _tim.Init.CounterMode = TIM_COUNTERMODE_UP;
    _tim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    _tim.Init.RepetitionCounter = 0;

    // Initialize encoder interface
    TIM_Encoder_InitTypeDef enc_init;

    enc_init.IC1Polarity  = TIM_INPUTCHANNELPOLARITY_RISING;
    enc_init.IC2Polarity  = TIM_INPUTCHANNELPOLARITY_RISING;
    enc_init.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    enc_init.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    enc_init.IC1Prescaler = TIM_ICPSC_DIV1;
    enc_init.IC2Prescaler = TIM_ICPSC_DIV1;
    enc_init.IC1Filter    = 0;
    enc_init.IC2Filter    = 0;

    switch (_channel) {
        case 1:
            enc_init.EncoderMode = TIM_ENCODERMODE_TI1;
            break;

        case 2:
            enc_init.EncoderMode = TIM_ENCODERMODE_TI2;
            break;

        default:
            enc_init.EncoderMode = TIM_ENCODERMODE_TI12;
            break;
    }

    HAL_TIM_Encoder_Init(&_tim, &enc_init);
    __HAL_TIM_SetCounter(&_tim, 0);

    switch (_channel) {
        case 1:
            HAL_TIM_Encoder_Start(&_tim, TIM_CHANNEL_1);
            break;

        case 2:
            HAL_TIM_Encoder_Start(&_tim, TIM_CHANNEL_2);
            break;

        default:
            HAL_TIM_Encoder_Start(&_tim, TIM_CHANNEL_1 | TIM_CHANNEL_2);
            break;
    }
}


void ubot::Enc::set(const uint32_t value) {
    __HAL_TIM_SetCounter(&_tim, value);
}


uint32_t ubot::Enc::get(void) const {
    return __HAL_TIM_GetCounter(&_tim);
}

