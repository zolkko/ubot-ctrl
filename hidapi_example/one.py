#!/usr/bin/env python

import usb.core
import usb.util
import time


USB_IF      = 0 # Interface
USB_TIMEOUT = 5 # Timeout in MS

USB_VENDOR  = 0x1234 # Rii
USB_PRODUCT = 0x0001 # Mini Wireless Keyboard

dev = usb.core.find(idVendor=USB_VENDOR, idProduct=USB_PRODUCT)
if dev:
    dev.set_configuration()

    cfg = dev.get_active_configuration()
    intf = cfg[(0,0)]
    print 'Active configuration =', cfg

    out_endpoint = usb.util.find_descriptor(
        intf,
        custom_match = lambda e: usb.util.endpoint_direction(
            e.bEndpointAddress) == usb.util.ENDPOINT_OUT)

    in_endpoint = usb.util.find_descriptor(
        intf,
        custom_match = lambda e: usb.util.endpoint_direction(
            e.bEndpointAddress) == usb.util.ENDPOINT_IN)

    if out_endpoint:
        print 'OUT endpoint detected 0x%x' % (out_endpoint.bEndpointAddress, )
    else:
        print 'Failed to detect IN endpoint'

    if in_endpoint:
        print 'IN endpoint detected 0x%x' % (in_endpoint.bEndpointAddress, )
    else:
        print 'Failed to detect IN endpoint'

    if dev.is_kernel_driver_active(USB_IF) is True:
        print 'Kernel address is active'
    else:
        print 'Kernel address is not active'

    while True:
        data = None
        try:
            data = dev.read(in_endpoint.bEndpointAddress, in_endpoint.wMaxPacketSize, USB_TIMEOUT)
            print 'Data=', data
        except usb.core.USBError as err:
            print 'error...', type(err)
            exit()
        time.sleep(0.01)
