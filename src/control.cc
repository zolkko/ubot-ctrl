
#include <stdint.h>
#include <mbed.h>
#include <InterruptManager.h>
#include <rtos.h>
#include <string.h>

#include "eventloop.h"
#include "control.h"
#include "crc8.h"


typedef enum {
    IC_MOTOR1_WRITE_VELOCITY = 1,
    IC_MOTOR1_READ_VELOCITY  = 2,

    IC_MOTOR2_WRITE_VELOCITY = 3,
    IC_MOTOR2_READ_VELOCITY  = 4,

    IC_MOTOR3_WRITE_VELOCITY = 5,
    IC_MOTOR3_READ_VELOCITY  = 6,

    IC_MOTOR4_WRITE_VELOCITY = 7,
    IC_MOTOR4_READ_VELOCITY  = 8,

    IC_MOTOR5_WRITE_VELOCITY = 9,
    IC_MOTOR5_READ_VELOCITY  = 10,

    IC_MOTOR6_WRITE_VELOCITY = 11,
    IC_MOTOR6_READ_VELOCITY  = 12
} input_command_t;


ubot::Control::Control(PinName mosi, PinName miso, PinName clk, PinName cs, EventLoop * event_loop)
    : _spi(mosi, miso, clk, cs),
      _event_loop(event_loop)
{
    _spi.format(8, 0);
   _spi->DR = 0;

   memset(_velocity, 0, sizeof(_velocity));
}


/*
 * SPI_IT_TXE: Tx buffer empty interrupt enable
 * SPI_IT_RXNE: RX buffer not empty interrupt enable
 * SPI_IT_ERR: Error interrupt enable
 */
void ubot::Control::enable_irq()
{
    InterruptManager::get()->add_handler_front(this, &ubot::Control::handle_irq, SPI1_IRQn);
    _spi->CR2 |= SPI_IT_RXNE;
    NVIC_EnableIRQ(SPI1_IRQn);
}


void ubot::Control::disable_irq()
{
    NVIC_DisableIRQ(SPI1_IRQn);
    _spi->CR2 &= ~SPI_IT_RXNE;
}

extern "C" void debug_isr(void) {
    volatile int a = 10;

    do {
        a += 3;
    } while (false);
}


/*
 * SPI_FLAG_RXNE: Receive buffer not empty flag
 * SPI_FLAG_TXE: Transmit buffer empty flag
 * SPI_FLAG_CRCERR: CRC error flag
 * SPI_FLAG_MODF: Mode fault flag
 * SPI_FLAG_OVR: Overrun flag
 * SPI_FLAG_BSY: Busy flag
 * SPI_FLAG_FRE: Frame format error flag
 * SPI_FLAG_FTLVL: SPI fifo transmission level
 * SPI_FLAG_FRLVL: SPI fifo reception level
 */
void ubot::Control::handle_irq(void)
{
    uint32_t flags = _spi->SR;
    if ((flags & SPI_FLAG_RXNE) == SPI_FLAG_RXNE) {
        _spi->DR = this->process(_spi->DR);
    }
}


uint8_t ubot::Control::process_cmd_read(uint8_t input_data)
{
    memset(&_state_data, 0, sizeof(_state_data));

    switch (static_cast<input_command_t>(input_data)) {
        case IC_MOTOR1_WRITE_VELOCITY:
            return this->process_write_motor_velocity(0, input_data);

        case IC_MOTOR2_WRITE_VELOCITY:
            return this->process_write_motor_velocity(1, input_data);

        case IC_MOTOR3_WRITE_VELOCITY:
            return this->process_write_motor_velocity(2, input_data);

        case IC_MOTOR4_WRITE_VELOCITY:
            return this->process_write_motor_velocity(3, input_data);

        case IC_MOTOR5_WRITE_VELOCITY:
            return this->process_write_motor_velocity(4, input_data);

        case IC_MOTOR6_WRITE_VELOCITY:
            return this->process_write_motor_velocity(6, input_data);

        default:
            _state = CS_READ_COMMAND;
            return 0;
    }
}


uint8_t ubot::Control::process_write_motor_velocity(uint8_t index, uint8_t input_data)
{
    int16_t current_velocity = _velocity[index].load();

    _state_data.index = index;
    _state_data.crc = ubot::crc8(CRC8_INITIAL, input_data);
    _state_data.current_velocity = current_velocity;
    _state = CS_READ_MOTOR_VELOCITY_MSB;

    uint8_t current_velocity_msb = static_cast<uint8_t>((_state_data.current_velocity & 0xff00) >> 8);

    return current_velocity_msb;
}


inline uint8_t ubot::Control::process_velocity_msb(uint8_t input_data)
{
    uint8_t current_velocity_lsb = static_cast<uint8_t>(_state_data.current_velocity & 0xff);

    _state_data.msb = input_data;
    _state_data.crc = ubot::crc8(_state_data.crc, input_data);
    _state = CS_READ_MOTOR_VELOCITY_LSB;

    return current_velocity_lsb;
}


uint8_t ubot::Control::process_velocity_lsb(uint8_t input_data)
{
    _state_data.lsb = input_data;
    _state_data.crc = ubot::crc8(_state_data.crc, input_data);
    _state = CS_READ_MOTOR_VELOCITY_CRC;

    uint8_t crc = ubot::crc8(CRC8_INITIAL, static_cast<uint8_t>((_velocity[_state_data.index] & 0xff00) >> 8));
    crc = ubot::crc8(crc, static_cast<uint8_t>(_velocity[_state_data.index] & 0xff));

    return crc;
}


uint8_t ubot::Control::process_velocity_crc(uint8_t input_data)
{
    uint8_t crc = ubot::crc8(_state_data.crc, input_data);

    if (crc == 0) {
        event_t event;
        switch (_state_data.index) {
            case 0:
                event.type = ET_MOTOR1_VELOCITY;
                break;

            default:
                event.type = ET_MOTOR1_VELOCITY;
                break;
        }
        event.payload.velocity = static_cast<int16_t>((_state_data.msb << 8) | _state_data.lsb);
        _event_loop->publish(&event);
    }

    _state = CS_READ_COMMAND;

    return crc;
}


uint8_t ubot::Control::process(uint8_t input_data)
{
    switch (_state) {
        case CS_READ_COMMAND:
            return this->process_cmd_read(input_data);

        case CS_READ_MOTOR_VELOCITY_MSB:
            return this->process_velocity_msb(input_data);

        case CS_READ_MOTOR_VELOCITY_LSB:
            return this->process_velocity_lsb(input_data);

        case CS_READ_MOTOR_VELOCITY_CRC:
            return this->process_velocity_crc(input_data);

        default:
            _state = CS_READ_COMMAND;
            return 0;
    }
}


void ubot::Control::set_velocity(uint8_t index, int16_t velocity)
{
    _velocity[index].store(velocity);
}
