
#include <stdint.h>
#include <mbed.h>
#include <InterruptManager.h>
#include <rtos.h>

#include "ctrl_fsm.h"
#include "spislave_ext.h"



ubot::Control::Control(PinName mosi, PinName miso, PinName clk, PinName cs)
    : _spi(mosi, miso, clk, cs)
{
    _spi.format(8, 0);
   _spi->DR = 0;
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
    NVIC_EnableIRQ(SPI1_IRQn);
    _spi->CR2 &= ~SPI_IT_RXNE;
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
        _spi->DR = _fsm.put(_spi->DR);
    }
}


osStatus ubot::Control::get_event(ubot::control::event_t& event)
{
    return _fsm.get(event);
}

