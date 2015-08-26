
#ifndef __crc8_h__
#define __crc8_h__

namespace ubot
{

#define CRC8_INITIAL (0x00)

uint8_t crc8(uint8_t crc, uint8_t data);
uint8_t crc8(uint8_t * data, size_t data_len, uint8_t initial_crc);

}

#endif

