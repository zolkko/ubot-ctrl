#!/usr/bin/env python

import hid
import time


dev = hid.device()
dev.open_path('USB_1234_0001_1a121100')
# dev.open_path('USB_1234_0001_1a122000')

try:
    print 'Manufacturer: %s' % dev.get_manufacturer_string()
    print 'Product: %s' % dev.get_product_string()
    print 'Serial No: %s' % dev.get_serial_number_string()

    # try writing some data to the device
    for k in range(5):
        out_data = [0x2] + [0xaa] * 24
        #print 'write data=', out_data
        r = dev.write(out_data)
        print 'write =', out_data[1:]

        data = dev.read(64)
        if data:
            print 'read =', data
        else:
            print 'No read'
        print '-----' 

        time.sleep(1.0)
    dev.close()

except IOError, ex:
    print ex
finally:
    dev.close()

print 'Done'
