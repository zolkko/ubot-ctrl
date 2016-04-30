#ifndef __USBD_DESC_H
#define __USBD_DESC_H

#ifdef __cplusplus
extern "C" {
#endif


#include <usbd_def.h>


#define DEVICE_ID1            (0x1FFF7A10)
#define DEVICE_ID2            (0x1FFF7A14)
#define DEVICE_ID3            (0x1FFF7A18)
#define USB_SIZ_STRING_SERIAL 0x1A


extern USBD_DescriptorsTypeDef VCP_Desc;


#ifdef __cplusplus
}
#endif

#endif /* __USBD_DESC_H */
