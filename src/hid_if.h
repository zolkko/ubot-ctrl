#ifndef __hid_if_h__
#define __hid_if_h__


#ifdef __cplusplus
extern "C" {
#endif


#define BAMPER_REPORT_ID                    0x01
#define MOTOR_REPORT_ID                     0x02


extern USBD_ClassTypeDef USBD_UBOT_HID;

uint8_t USBD_UBOT_HID_SendReport(USBD_HandleTypeDef *, uint8_t *, uint16_t);


#ifdef __cplusplus
}
#endif


#endif // __usbd_chid_h__
