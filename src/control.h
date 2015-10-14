
#ifndef __control_h__
#define __control_h__

#include <stdint.h>
#include <stdbool.h>
#include <atomic>
#include "eventloop.h"


namespace ubot
{

typedef enum {
    CS_READ_COMMAND            = 0,
    CS_READ_MOTOR_VELOCITY_MSB = 1,
    CS_READ_MOTOR_VELOCITY_LSB = 2,
    CS_READ_MOTOR_VELOCITY_CRC = 3
} control_state_t;


class Control
{
public:
    Control(PinName mosi, PinName miso, PinName clk, PinName cs, EventLoop * event_loop);

    void enable_irq(void);

    void disable_irq(void);

    void handle_irq(void);

    void set_velocity(uint8_t index, int16_t velocity);

private:
    uint8_t process(uint8_t input_data);

    uint8_t process_write_motor_velocity(uint8_t index, uint8_t input_data);

    uint8_t process_cmd_read(uint8_t input_data);

    uint8_t process_velocity_msb(uint8_t input_data);

    uint8_t process_velocity_lsb(uint8_t input_data);

    uint8_t process_velocity_crc(uint8_t input_data);

    class SPIControl
        : public SPISlave
    {
    public:
        SPIControl(PinName mosi, PinName miso, PinName clk, PinName cs)
            : SPISlave(mosi, miso, clk, cs)
        {
        }

        inline SPI_TypeDef* operator->(void) {
            SPI_TypeDef * s = (SPI_TypeDef *)(_spi.spi);
            return s;
        }
    };

    typedef struct {
        uint8_t index;
        uint8_t msb;
        uint8_t lsb;
        uint8_t crc;
        int16_t current_velocity;
    } state_data_t;

    SPIControl _spi;
    EventLoop * _event_loop;

    control_state_t _state;
    state_data_t _state_data;

    std::atomic_short _velocity[6];
};

}

#endif
