#ifndef __usbif_h__
#define __usbif_h__

#include "USBDevice.h"
#include "USBHID.h"

namespace ubot {

typedef struct _control_input_t {
    uint32_t speed;
} control_input_t;

typedef struct _control_output_t {
    uint32_t speed;
} control_output_t;


class UsbIf : public USBHID {
public:
    /**
     * Constructor
     *
     * @param vendor_id Your vendor_id
     * @param product_id Your product_id
     * @param product_release Your preoduct_release
     * @param connect_blocking define if the connection must be blocked if USB not plugged in
     */
     UsbIf(size_t outsize, size_t insize, uint16_t vendor_id = 0x1234, uint16_t product_id = 0x0001, uint16_t product_release = 0x0001) :
        USBHID(outsize, insize, vendor_id, product_id, product_release, false)
     {
         connect(false);
     };

     virtual uint8_t * stringIserialDesc() {
         static uint8_t stringIserialDescriptor[] = {
             0x16,                                                           /*bLength*/
             STRING_DESCRIPTOR,                                              /*bDescriptorType 0x03*/
             '0',0,'0',0,'0',0,'0',0,'0',0,'0',0,'0',0,'0',0,'0',0,'0',0,    /*bString iSerial - 0123456789*/
         };
         return stringIserialDescriptor;
     }

     virtual uint8_t * stringImanufacturerDesc() {
         static uint8_t stringImanufacturerDescriptor[] = {
             0x10,                                          /*bLength*/
             STRING_DESCRIPTOR,                             /*bDescriptorType 0x03*/
             'u',0,'b',0,'o',0,'t',0,'.',0,'i',0,'o',0,     /*bString iManufacturer - u-bot.io*/
         };
         return stringImanufacturerDescriptor;
     }

     virtual uint8_t * stringIproductDesc() {
         static uint8_t stringIproductDescriptor[] = {
             0x1a,                                     // bLength
             STRING_DESCRIPTOR,                        // bDescriptorType 0x03
             'u',0, 'b',0, 'o',0, 't',0,
             '-',0, 'c',0,
             'o',0, 'n',0, 't',0, 'r',0, 'o',0, 'l',0  // bString iProduct - ubot-control
         };
         return stringIproductDescriptor;
     }

protected:
    virtual bool EP1_OUT_callback();
    virtual bool EP1_IN_callback();

private:
    // HID_REPORT recv_report;
    // HID_REPORT send_report;
};

}

#endif // __usbif_h__
