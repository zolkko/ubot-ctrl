
#ifndef __controlif_h__
#define __controlif_h__

namespace ubot
{

class Control : public SPISlave
{
public:
    ControlIf(PinName mosi, PinName miso, PinName clk, PinName cs);

    void enable_irq(void);

    void disable_irq(void);

    void handle_irq(void);

private:

    class SPIControl : public SPISlave
    {
    public:
        SPI_TypeDef operator*(void) {
            SPI_TypeDef * s = (SPI_TypeDef *)(_spi.spi);
            return s;
        }
    };

    SPIControl _spi;
    ubot::control::Fsm _fsm;
};

}

#endif

