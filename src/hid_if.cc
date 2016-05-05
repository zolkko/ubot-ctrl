#include "usbd_desc.h"
#include <usbd_ctlreq.h>
#include <usbd_core.h>
#include "hid_if.h"


typedef enum {
    UBOT_HID_IDLE = 0,
    UBOT_HID_BUSY,
} UBOT_HID_StateTypeDef;


#define USBD_UBOT_HID_OUTREPORT_BUF_SIZE    64


typedef struct {
    uint8_t Report_buf[USBD_UBOT_HID_OUTREPORT_BUF_SIZE];
    uint32_t Protocol;
    uint32_t IdleState;
    uint32_t AltSetting;
    uint32_t IsReportAvailable;
    UBOT_HID_StateTypeDef state;
} USBD_UBOT_HID_HandleTypeDef;


#define USBD_UBOT_HID_REPORT_DESC_SIZE      (58)


// HID Report Descriptor
__ALIGN_BEGIN static uint8_t HID_Report_Desc[USBD_UBOT_HID_REPORT_DESC_SIZE] __ALIGN_END = {
    0x06, 0xFF, 0x00,               // USAGE_PAGE (Vendor Page: 0xFF00)
    0x09, 0x01,                     // USAGE (Demo Kit)
    0xa1, 0x01,                     // COLLECTION (Application)

    // BAMPER_REPORT
    0x85, BAMPER_REPORT_ID,         // REPORT_ID
    0x09, 0x07,                     // USAGE (ADC)
    0x15, 0x00,                     // LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,               // LOGICAL_MAXIMUM (255)
    0x75, 0x08,                     // REPORT_SIZE (8)
    0x95, 0x02,                     // REPORT_COUNT(2)
    0x81, 0x82,                     // INPUT (Data,Var,Abs,Vol)

    // MOTOR_REPORT_ID
    0x85, MOTOR_REPORT_ID,          // REPORT_ID
    0x15, 0x00,                     // LOGICAL_MINIMUM (0)
    0x26, 0xFF, 0x00,               // LOGICAL_MAXIMUM (255)
    0x75, 0x08,                     // REPORT_SIZE(8)
    0x95, 24,                       // REPORT_COUNT(24)
    0x09, 0x01,                     // USAGE
    0x81, 0x02,                     // INPUT (array)
    0x95, 24,                       // REPORT_COUNT(24)
    0x09, 0x02,                     // USAGE
    0x91, 0x02,                     // OUTPUT (array)

    0xc0                            // END_COLLECTION
};


static int8_t UbotHID_Init(void)
{
    return USBD_OK;
}


static int8_t UbotHID_DeInit(void)
{
    return USBD_OK;
}


static int8_t UbotHID_OutEvent(uint8_t event_idx, uint8_t * data)
{
    switch (event_idx) {

        case BAMPER_REPORT_ID:
            break;

        default:
            return USBD_FAIL;
    }

    return USBD_OK;
}


/**
 * Interface structure for USBD library
 */

#define UBOT_HID_EPIN_ADDR                   0x81
#define UBOT_HID_EPIN_SIZE                   0x40

#define UBOT_HID_EPOUT_ADDR                  0x01
#define UBOT_HID_EPOUT_SIZE                  0x40

#define UBOT_HID_CONFIG_DESC_SIZE            41

#define UBOT_HID_DESC_SIZE                   9

#define UBOT_HID_DESCRIPTOR_TYPE             0x21
#define UBOT_HID_REPORT_DESC                 0x22

#define UBOT_HID_REQ_SET_PROTOCOL            0x0B
#define UBOT_HID_REQ_GET_PROTOCOL            0x03

#define UBOT_HID_REQ_SET_IDLE                0x0A
#define UBOT_HID_REQ_GET_IDLE                0x02

#define UBOT_HID_REQ_SET_REPORT              0x09
#define UBOT_HID_REQ_GET_REPORT              0x01


// USB HID device Configuration Descriptor
__ALIGN_BEGIN static uint8_t USBD_UBOT_HID_CfgDesc[UBOT_HID_CONFIG_DESC_SIZE] __ALIGN_END = {
    0x09,                               // bLength: Configuration Descriptor size
    USB_DESC_TYPE_CONFIGURATION,        // bDescriptorType: Configuration
    UBOT_HID_CONFIG_DESC_SIZE,

    // wTotalLength: Bytes returned
    0x00,
    0x01,                               // bNumInterfaces: 1 interface
    0x01,                               // bConfigurationValue: Configuration value
    0x00,                               // iConfiguration: Index of string descriptor describing the configuration
    0xC0,                               // bmAttributes: bus powered
    0x32,                               // MaxPower 100 mA: this current is used for detecting Vbus TODO: C_POWER(0)

    // Descriptor of CUSTOM HID interface
    0x09,                               // bLength: Interface Descriptor size
    USB_DESC_TYPE_INTERFACE,            // bDescriptorType: Interface descriptor type
    0x00,                               // bInterfaceNumber: Number of Interface
    0x00,                               // bAlternateSetting: Alternate setting
    0x02,                               // bNumEndpoints
    0x03,                               // bInterfaceClass: UBOT_HID
    0x00,                               // bInterfaceSubClass : no boot
    0x00,                               // nInterfaceProtocol : none
    0,                                  // iInterface: Index of string descriptor

    // Descriptor of UBOT_HID
    0x09,                               // bLength: UBOT_HID Descriptor size
    UBOT_HID_DESCRIPTOR_TYPE,           // bDescriptorType: UBOT_HID
    0x11,                               // bCUSTOM_HIDUSTOM_HID: UBOT_HID Class Spec release number
    0x01,
    0x00,                               // bCountryCode: Hardware target country
    0x01,                               // bNumDescriptors: Number of UBOT_HID class descriptors to follow
    0x22,                               // bDescriptorType
    USBD_UBOT_HID_REPORT_DESC_SIZE,     // wItemLength: Total length of Report descriptor
    0x00,

    // Descriptor of UBOT HID endpoints
    0x07,                               // bLength: Endpoint Descriptor size
    USB_DESC_TYPE_ENDPOINT,             // bDescriptorType:

    UBOT_HID_EPIN_ADDR,                 // bEndpointAddress: Endpoint Address (IN)
    0x03,                               // bmAttributes: Interrupt endpoint
    UBOT_HID_EPIN_SIZE,                 // wMaxPacketSize
    0x00,
    0x20,                               // bInterval: Polling Interval (20 ms)

    0x07,                               // bLength: Endpoint Descriptor size
    USB_DESC_TYPE_ENDPOINT,             // bDescriptorType:
    UBOT_HID_EPOUT_ADDR,                // bEndpointAddress: Endpoint Address (OUT)
    0x03,                               // bmAttributes: Interrupt endpoint
    UBOT_HID_EPOUT_SIZE,                // wMaxPacketSize
    0x00,
    0x20,                               // bInterval: Polling Interval (20 ms)
} ;


// USB CUSTOM_HID device Configuration Descriptor
__ALIGN_BEGIN static uint8_t USBD_UBOT_HID_Desc[UBOT_HID_DESC_SIZE] __ALIGN_END = {
    0x09,                               // bLength: UBOT_HID Descriptor size
    UBOT_HID_DESCRIPTOR_TYPE,           // bDescriptorType
    0x11,                               // bCUSTOM_HIDUSTOM_HID: Class Spec release number
    0x01,
    0x00,                               // bCountryCode: Hardware target country
    0x01,                               // bNumDescriptors: Number of class descriptors to follow
    0x22,                               // bDescriptorType
    USBD_UBOT_HID_REPORT_DESC_SIZE,     // wItemLength: Total length of Report descriptor
    0x00,
};


// USB Standard Device Descriptor
__ALIGN_BEGIN static uint8_t USBD_UBOT_HID_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END = {
    USB_LEN_DEV_QUALIFIER_DESC,
    USB_DESC_TYPE_DEVICE_QUALIFIER,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x40,
    0x01,
    0x00,
};


static uint8_t USBD_UBOT_HID_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t USBD_UBOT_HID_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t USBD_UBOT_HID_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static uint8_t *USBD_UBOT_HID_GetCfgDesc(uint16_t *length);
static uint8_t *USBD_UBOT_HID_GetDeviceQualifierDesc(uint16_t *length);
static uint8_t USBD_UBOT_HID_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_UBOT_HID_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_UBOT_HID_EP0_RxReady(USBD_HandleTypeDef *pdev);


USBD_ClassTypeDef USBD_UBOT_HID = {
    USBD_UBOT_HID_Init,
    USBD_UBOT_HID_DeInit,
    USBD_UBOT_HID_Setup,
    NULL,                           // EP0_TxSent
    USBD_UBOT_HID_EP0_RxReady,      // EP0_RxReady, STATUS STAGE IN
    USBD_UBOT_HID_DataIn,           // DataIn
    USBD_UBOT_HID_DataOut,
    NULL,                           // SOF
    NULL,
    NULL,
    USBD_UBOT_HID_GetCfgDesc,
    USBD_UBOT_HID_GetCfgDesc,
    USBD_UBOT_HID_GetCfgDesc,
    USBD_UBOT_HID_GetDeviceQualifierDesc,
};


/**
 * @brief  USBD_UBOT_HID_Init
 *         Initialize the UBOT_HID interface
 * @param  pdev: device instance
 * @param  cfgidx: Configuration index
 * @retval status
 */
static uint8_t USBD_UBOT_HID_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
    uint8_t ret = USBD_OK;
    USBD_UBOT_HID_HandleTypeDef *hhid;

    USBD_LL_OpenEP(pdev, UBOT_HID_EPIN_ADDR, USBD_EP_TYPE_INTR, UBOT_HID_EPIN_SIZE);
    USBD_LL_OpenEP(pdev, UBOT_HID_EPOUT_ADDR, USBD_EP_TYPE_INTR, UBOT_HID_EPOUT_SIZE);

    pdev->pClassData = USBD_malloc(sizeof(USBD_UBOT_HID_HandleTypeDef));

    if (pdev->pClassData == NULL) {
        ret = USBD_FAIL;
    } else {
        hhid = (USBD_UBOT_HID_HandleTypeDef*) pdev->pClassData;
        hhid->state = UBOT_HID_IDLE;
        UbotHID_Init();

        // Prepare Out endpoint to receive 1st packet
        USBD_LL_PrepareReceive(pdev, UBOT_HID_EPOUT_ADDR, hhid->Report_buf, USBD_UBOT_HID_OUTREPORT_BUF_SIZE);
    }

    return ret;
}


/**
 * @brief  USBD_UBOT_HID_Init
 *         DeInitialize the UBOT_HID layer
 * @param  pdev: device instance
 * @param  cfgidx: Configuration index
 * @retval status
 */
static uint8_t USBD_UBOT_HID_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
    USBD_LL_CloseEP(pdev, UBOT_HID_EPIN_ADDR);
    USBD_LL_CloseEP(pdev, UBOT_HID_EPOUT_ADDR);

    if (pdev->pClassData != NULL) {
        UbotHID_DeInit();
        USBD_free(pdev->pClassData);
        pdev->pClassData = NULL;
    }

    return USBD_OK;
}


/**
 * @brief  USBD_UBOT_HID_Setup
 *         Handle the UBOT_HID specific requests
 * @param  pdev: instance
 * @param  req: usb requests
 * @retval status
 */
static uint8_t USBD_UBOT_HID_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
    uint16_t len = 0;
    uint8_t  *pbuf = NULL;
    USBD_UBOT_HID_HandleTypeDef *hhid = (USBD_UBOT_HID_HandleTypeDef*)pdev->pClassData;

    switch (req->bmRequest & USB_REQ_TYPE_MASK) {
        case USB_REQ_TYPE_CLASS:
            switch (req->bRequest) {
                case UBOT_HID_REQ_SET_PROTOCOL:
                    hhid->Protocol = (uint8_t)(req->wValue);
                    break;

                case UBOT_HID_REQ_GET_PROTOCOL:
                    USBD_CtlSendData(pdev, (uint8_t *)&hhid->Protocol, 1);
                    break;

                case UBOT_HID_REQ_SET_IDLE:
                    hhid->IdleState = (uint8_t)(req->wValue >> 8);
                    break;

                case UBOT_HID_REQ_GET_IDLE:
                    USBD_CtlSendData(pdev, (uint8_t *)&hhid->IdleState, 1);
                    break;

                case UBOT_HID_REQ_SET_REPORT:
                    hhid->IsReportAvailable = 1;
                    USBD_CtlPrepareRx(pdev, hhid->Report_buf, (uint8_t)(req->wLength));
                    break;

                default:
                    USBD_CtlError(pdev, req);
                    return USBD_FAIL;
            }
            break;

        case USB_REQ_TYPE_STANDARD:
            switch (req->bRequest) {
                case USB_REQ_GET_DESCRIPTOR:
                    if (req->wValue >> 8 == UBOT_HID_REPORT_DESC) {
                        len = MIN(USBD_UBOT_HID_REPORT_DESC_SIZE, req->wLength);
                        pbuf = HID_Report_Desc;
                    } else if (req->wValue >> 8 == UBOT_HID_DESCRIPTOR_TYPE) {
                        pbuf = USBD_UBOT_HID_Desc;
                        len = MIN(UBOT_HID_DESC_SIZE, req->wLength);
                    }

                    USBD_CtlSendData(pdev, pbuf, len);
                    break;

                case USB_REQ_GET_INTERFACE:
                    USBD_CtlSendData (pdev, (uint8_t *)&hhid->AltSetting, 1);
                    break;

                case USB_REQ_SET_INTERFACE:
                    hhid->AltSetting = (uint8_t)(req->wValue);
                    break;
            }
    }

    return USBD_OK;
}


/**
 * @brief  USBD_UBOT_HID_SendReport
 *         Send UBOT_HID Report
 * @param  pdev: device instance
 * @param  buff: pointer to report
 * @retval status
 */
uint8_t USBD_UBOT_HID_SendReport(USBD_HandleTypeDef *pdev, uint8_t *report, uint16_t len)
{
    USBD_UBOT_HID_HandleTypeDef *hhid = (USBD_UBOT_HID_HandleTypeDef*)pdev->pClassData;

    if (pdev->dev_state == USBD_STATE_CONFIGURED) {
        if (hhid->state == UBOT_HID_IDLE) {
            hhid->state = UBOT_HID_BUSY;
            USBD_LL_Transmit(pdev, UBOT_HID_EPIN_ADDR, report, len);
        }
    }

    return USBD_OK;
}


/**
 * @brief  USBD_UBOT_HID_GetCfgDesc
 *         return configuration descriptor
 * @param  speed : current device speed
 * @param  length : pointer data length
 * @retval pointer to descriptor buffer
 */
static uint8_t *USBD_UBOT_HID_GetCfgDesc(uint16_t *length)
{
    *length = sizeof(USBD_UBOT_HID_CfgDesc);
    return USBD_UBOT_HID_CfgDesc;
}


/**
 * @brief  DeviceQualifierDescriptor
 *         return Device Qualifier descriptor
 * @param  length : pointer data length
 * @retval pointer to descriptor buffer
 */
static uint8_t *USBD_UBOT_HID_GetDeviceQualifierDesc(uint16_t *length)
{
    *length = sizeof (USBD_UBOT_HID_DeviceQualifierDesc);
    return USBD_UBOT_HID_DeviceQualifierDesc;
}


/**
 * @brief  USBD_UBOT_HID_DataIn
 *         handle data IN Stage
 * @param  pdev: device instance
 * @param  epnum: endpoint index
 * @retval status
 */
static uint8_t USBD_UBOT_HID_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
    ((USBD_UBOT_HID_HandleTypeDef *)pdev->pClassData)->state = UBOT_HID_IDLE;
    return USBD_OK;
}


/**
 * @brief  USBD_UBOT_HID_DataOut
 *         handle data OUT Stage
 * @param  pdev: device instance
 * @param  epnum: endpoint index
 * @retval status
 */
static uint8_t USBD_UBOT_HID_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
    USBD_UBOT_HID_HandleTypeDef *hhid = (USBD_UBOT_HID_HandleTypeDef *)pdev->pClassData;
    UbotHID_OutEvent(hhid->Report_buf[0], &hhid->Report_buf[1]);
    USBD_LL_PrepareReceive(pdev, UBOT_HID_EPOUT_ADDR , hhid->Report_buf, USBD_UBOT_HID_OUTREPORT_BUF_SIZE);

    return USBD_OK;
}


/**
 * @brief  USBD_UBOT_HID_EP0_RxReady
 *         Handles control request data.
 * @param  pdev: device instance
 * @retval status
 */
static uint8_t USBD_UBOT_HID_EP0_RxReady(USBD_HandleTypeDef *pdev)
{
    USBD_UBOT_HID_HandleTypeDef *hhid = (USBD_UBOT_HID_HandleTypeDef *)pdev->pClassData;

    if (hhid->IsReportAvailable == 1) {
        UbotHID_OutEvent(hhid->Report_buf[0], &hhid->Report_buf[1]);
        hhid->IsReportAvailable = 0;
    }

    return USBD_OK;
}
