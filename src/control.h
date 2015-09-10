
#ifndef __control_h__
#define __control_h__

#include "ctrl_fsm.h"

namespace ubot
{

class Control
{
public:
    Control(PinName mosi, PinName miso, PinName clk, PinName cs);

    void enable_irq(void);

    void disable_irq(void);

    void handle_irq(void);

    osStatus get_event(ubot::control::event_t& event);

private:

    class SPIControl
        : public SPISlave
    {
    public:
        SPIControl(PinName mosi, PinName miso, PinName clk, PinName cs)
            : SPISlave(mosi, miso, clk, cs) {
        }

        inline SPI_TypeDef* operator->(void) {
            SPI_TypeDef * s = (SPI_TypeDef *)(_spi.spi);
            return s;
        }
    };

    SPIControl _spi;
    ubot::control::Fsm _fsm;
};

}

#endif

