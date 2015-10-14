
#include <stdint.h>
#include <atomic>
#include <cmsis.h>
#include <mbed.h>
#include <InterruptManager.h>
#include <PinNames.h>
#include "pinmap.h"
#include "enc.h"
#include "motor_defs.h"



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
    : _channel(0),
      _ovf(0),
      _values_index(0)
{
    _values[0] = 0;
    _values[1] = 0;

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

    switch (STM_PIN_CHANNEL(function)) {
        case 1:
            _channel = TIM_CHANNEL_1;
            _cc_it = TIM_IT_CC1;
            _cc_flag = TIM_FLAG_CC1;
            _of_flag = TIM_FLAG_CC1OF;
            break;
        case 2:
            _channel = TIM_CHANNEL_2;
            _cc_it = TIM_IT_CC2;
            _cc_flag = TIM_FLAG_CC2;
            _of_flag = TIM_FLAG_CC2OF;
            break;
        case 3:
            _channel = TIM_CHANNEL_3;
            _cc_it = TIM_IT_CC3;
            _cc_flag = TIM_FLAG_CC3;
            _of_flag = TIM_FLAG_CC3OF;
            break;
        default:
            _channel = TIM_CHANNEL_4;
            _cc_it = TIM_IT_CC4;
            _cc_flag = TIM_FLAG_CC4;
            _of_flag = TIM_FLAG_CC4OF;
            break;
    }

    _tim.Instance = reinterpret_cast<TIM_TypeDef *>(enc);
    _tim.Init.Period = 0xffff;
    _tim.Init.Prescaler = SystemCoreClock / ENC_TIMER_DIVIDER;
    _tim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    _tim.Init.CounterMode = TIM_COUNTERMODE_UP;
    _tim.Init.RepetitionCounter = 0;

    if (HAL_OK != HAL_TIM_IC_Init(&_tim)) {
        error("Failed to initialize timer in input-capture mode");
    }

    TIM_IC_InitTypeDef enc_init;
    enc_init.ICPrescaler = TIM_ICPSC_DIV1;
	enc_init.ICFilter = 0x0;
	enc_init.ICPolarity = TIM_ICPOLARITY_RISING;
	enc_init.ICSelection = TIM_ICSELECTION_DIRECTTI;

	if (HAL_OK != HAL_TIM_IC_ConfigChannel(&_tim, &enc_init, _channel)) {
        error("Failed to initialize input-capture channel");
    }

    if (HAL_OK != HAL_TIM_IC_Start(&_tim, _channel)) {
        error("Failed to start input-caputre timer");
    }
}


void ubot::Enc::enable_irq(void)
{
    InterruptManager::get()->add_handler_front(this, &ubot::Enc::handle_irq, TIM2_IRQn);
    __HAL_TIM_ENABLE_IT(&_tim, _cc_it);
    NVIC_EnableIRQ(TIM2_IRQn);
}


void ubot::Enc::disable_irq(void)
{
    __HAL_TIM_DISABLE_IT(&_tim, _cc_it);
}


bool ubot::Enc::is_update(void)
{
    return __HAL_TIM_GET_FLAG(&_tim, TIM_FLAG_UPDATE) != RESET;
}


void ubot::Enc::clear_update(void)
{
    __HAL_TIM_CLEAR_FLAG(&_tim, TIM_FLAG_UPDATE);
}


uint32_t ubot::Enc::get_cc(void)
{
    return HAL_TIM_ReadCapturedValue(&_tim, _channel);
}


bool ubot::Enc::is_cc(void)
{
    return __HAL_TIM_GET_FLAG(&_tim, _cc_flag) != RESET;
}


void ubot::Enc::clear_cc(void)
{
    __HAL_TIM_CLEAR_FLAG(&_tim, _cc_flag);
}


bool ubot::Enc::is_of(void)
{
    return __HAL_TIM_GET_FLAG(&_tim, _of_flag) != RESET;
}


void ubot::Enc::clear_of(void)
{
    __HAL_TIM_CLEAR_FLAG(&_tim, _of_flag);
}


void ubot::Enc::handle_irq(void)
{
    if (is_update()) {
        clear_update();

       _ovf++;

        if (_ovf > ENC_MAX_OVERFLOW) {
            _values_index = 0;
            _values[0] = 0;
            _values[1] = 0;
            _ovf = 0;

            if (_speed != 0) {
                _speed = 0;
            }
        }
    }

    if (is_cc()) {
        _values[_values_index] = get_cc();
        _values_index++;

        if (_values_index > 1) {
            uint32_t diff = 0;

            if (_values[1] > _values[0]) {
                diff = _values[1] - _values[0];
            } else {
                diff = (0xffff - _values[0]) + _values[1];
            }

            if (_ovf > 1) {
                diff += 0xffff * (_ovf - 1);
            }

            int16_t speed = (ENC_STEP_DISTANCE * ENC_PRESCALER_FREQ) / diff;
            if (speed != _speed) {
                _speed.store(speed);
            }

            _values_index = 0;
        }

        clear_cc();
    }

    if (is_of()) {
        clear_of();
    }
}


int16_t ubot::Enc::get_velocity() const {
    return _speed.load();
}
