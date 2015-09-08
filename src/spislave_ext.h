
#ifndef __spislave_ext_h__
#define __spislave_ext_h__

namespace ubot
{

class SPISlaveExt : public SPISlave
{
public:
    SPISlaveExt(PinName mosi, PinName miso, PinName clk, PinName cs)
        : SPISlave(mosi, miso, clk, cs)
    {
    }

    /*
     * SPI_IT_TXE: Tx buffer empty interrupt enable
     * SPI_IT_RXNE: RX buffer not empty interrupt enable
     * SPI_IT_ERR: Error interrupt enable
     */
    void enable_it(uint32_t flags);

    void disable_it(uint32_t flags);

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
    uint32_t get_flags(void);

    uint8_t get(void);

    void set(uint8_t data);
};

}



#endif

