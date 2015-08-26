#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import struct
from pyBusPirateLite.SPI import *


crc8_lut = (
    0,   49,  98,  83,  196, 245, 166, 151, 185, 136, 219, 234, 125, 76,  31,  46,
    67,  114, 33,  16,  135, 182, 229, 212, 250, 203, 152, 169, 62,  15,  92,  109,
    134, 183, 228, 213, 66,  115, 32,  17,  63,  14,  93,  108, 251, 202, 153, 168,
    197, 244, 167, 150, 1,   48,  99,  82,  124, 77,  30,  47,  184, 137, 218, 235,
    61,  12,  95,  110, 249, 200, 155, 170, 132, 181, 230, 215, 64,  113, 34,  19,
    126, 79,  28,  45,  186, 139, 216, 233, 199, 246, 165, 148, 3,   50,  97,  80,
    187, 138, 217, 232, 127, 78,  29,  44,  2,   51,  96,  81,  198, 247, 164, 149,
    248, 201, 154, 171, 60,  13,  94,  111, 65,  112, 35,  18,  133, 180, 231, 214,
    122, 75,  24,  41,  190, 143, 220, 237, 195, 242, 161, 144, 7,   54,  101, 84,
    57,  8,   91,  106, 253, 204, 159, 174, 128, 177, 226, 211, 68,  117, 38,  23,
    252, 205, 158, 175, 56,  9,   90,  107, 69,  116, 39,  22,  129, 176, 227, 210,
    191, 142, 221, 236, 123, 74,  25,  40,  6,   55,  100, 85,  194, 243, 160, 145,
    71,  118, 37,  20,  131, 178, 225, 208, 254, 207, 156, 173, 58,  11,  88,  105,
    4,   53,  102, 87,  192, 241, 162, 147, 189, 140, 223, 238, 121, 72,  27,  42,
    193, 240, 163, 146, 5,   52,  103, 86,  120, 73,  26,  43,  188, 141, 222, 239,
    130, 179, 224, 209, 70,  119, 36,  21,  59,  10,  89,  104, 255, 206, 157, 172
)

def crc8(data, initial_crc):
    global crc8_lut
    crc = initial_crc
    for b in data:
        i = (crc ^ b) & 0xff
        crc = crc8_lut[i]
    return crc


if __name__ == '__main__':
    #crc = crc8([1,2,3], 0x00)
    #print 'CRC = ', crc
    #print 'TESTED = ', crc8([1,2,3, crc], 0x00)
    # sys.exit()

    data = ""

    spi = SPI("/dev/tty.usbserial-A600eHG0", 115200)

    print "Entering binmode: ",
    if spi.BBmode():
        print "OK."
    else:
        print "failed."
        sys.exit()

    print "Entering raw SPI mode: ",
    if spi.enter_SPI():
        print "OK."
    else:
        print "failed."
        sys.exit()

    if spi.set_speed(SPISpeed._30KHZ) != 0x01:
        print "Failed to set SPI Speed."
        sys.exit()

    spi.timeout(0.2)

    if spi.cfg_spi(SPICfg.OUT_TYPE | SPICfg.CLK_EDGE) != 0x01:
        print "Failed to set SPI configuration.";
        sys.exit()

    spi.timeout(0.2)

    print "Configuring SPI."
    if spi.cfg_pins(PinCfg.POWER | PinCfg.CS | PinCfg.AUX) != 0x01:
        print "Failed to set SPI peripherals."
        sys.exit()

    print "Writing data into SPI"
    try:
        while True:
            try:
                velocity = int(raw_input('Velocity: '))

                if velocity > 2000:
                    velocity = 2000
                elif velocity < -2000:
                    velocity  = -2000
            except ValueError:
                print 'Incorrect input'
                continue

            data = [ord(x) for x in struct.pack('>Bh', 1, velocity)]
            crc = crc8(data, 0x00)
            data.append(crc)

            print 'Sending data: ', data, ' len= ', len(data)
            spi.CS_Low()
            spi.bulk_trans(len(data), data)
            spi.CS_High()

    except KeyboardInterrupt:
        print 'Keyboard interrupt'

    print "Reset Bus Pirate to user terminal: ",
    if spi.resetBP():
        print "OK."
    else:
        print "failed."
        sys.exit()

