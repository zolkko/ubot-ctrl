
#include <stdint.h>
#include <mbed.h>
#include "spislave_ext.h"


void ubot::SPISlaveExt::enable_it(uint32_t flags)
{
    ((SPI_TypeDef *)(_spi.spi))->CR2 |= flags;
}


void ubot::SPISlaveExt::disable_it(uint32_t flags)
{
    ((SPI_TypeDef *)(_spi.spi))->CR2 &= ~flags;
}


uint32_t ubot::SPISlaveExt::get_flags(void)
{
    SPI_TypeDef * s = (SPI_TypeDef *)(_spi.spi);
    return s->SR;
}


uint8_t ubot::SPISlaveExt::get(void)
{
    SPI_TypeDef * s = (SPI_TypeDef *)(_spi.spi);
    return (uint8_t)(s->DR);
}


void ubot::SPISlaveExt::set(uint8_t data)
{
    SPI_TypeDef * s = (SPI_TypeDef *)(_spi.spi);
    s->DR = data;
}

