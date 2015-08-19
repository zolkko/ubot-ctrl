
#include <stdint.h>
#include "spislave_ext.h"


void ubot::SPISlaveExt::enable_it(uint32_t flags)
{
    ((SPI_TypeDef *)(_spi.spi))->CR2 |= flags;
}


void ubot::SPISlaveExt::disable_it(uint32_t flags)
{
    ((SPI_TypeDef *)(_spi.spi))->CR2 &= ~flags;
}


bool ubot::SPISlaveExt::is_flag_set(uint32_t flag)
{
    SPI_TypeDef * s = (SPI_TypeDef *)(_spi.spi);

    return ((s->SR) & flag) == flag;
}


void ubot::SPISlaveExt::clear_freflag(void)
{
    SPI_TypeDef * s = (SPI_TypeDef *)(_spi.spi);

    do {
        __IO uint32_t tmpreg;
        tmpreg = s->SR;
        UNUSED(tmpreg);
    } while (0);
}


void ubot::SPISlaveExt::clear_modflag(void)
{
    SPI_TypeDef * s = (SPI_TypeDef *)(_spi.spi);

    do {
        __IO uint32_t tmpreg;
        tmpreg = s->SR;
        s->CR1 &= (~SPI_CR1_SPE);
        UNUSED(tmpreg);
    } while (0);
}


void ubot::SPISlaveExt::clear_ovrflag(void)
{
    SPI_TypeDef * s = (SPI_TypeDef *)(_spi.spi);

    do {
        __IO uint32_t tmpreg;
        tmpreg = s->DR;
        tmpreg = s->SR;
        UNUSED(tmpreg);
    } while (0);
}

