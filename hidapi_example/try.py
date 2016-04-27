#!/usr/bin/env python

import hid
import time


dev = hid.device()
dev.open_path('USB_1234_0001_1a121100')

try:
    print 'Manufacturer: %s' % dev.get_manufacturer_string()
    print 'Product: %s' % dev.get_product_string()
    print 'Serial No: %s' % dev.get_serial_number_string()

    # try writing some data to the device
    for k in range(255):
        out_data = [0x0] + [k] * 63
        #print 'write data=', out_data
        r = dev.write(out_data)
        #print 'write res=', r

        data = dev.read(64)
        if data:
            #print 'read=', data
            pass
        else:
            print 'No read'
    dev.close()

except IOError, ex:
    print ex
finally:
    dev.close()

print 'Done'
