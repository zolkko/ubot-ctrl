#ifndef __USBD_CONF_TEMPLATE_H
#define __USBD_CONF_TEMPLATE_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stm32f4xx.h>
#include <stm32f4xx_hal.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define USBD_MAX_NUM_INTERFACES             1
#define USBD_MAX_NUM_CONFIGURATION          1
#define USBD_MAX_STR_DESC_SIZ               0x100
#define USBD_SUPPORT_USER_STRING            0
#define USBD_SELF_POWERED                   1
#define USBD_DEBUG_LEVEL                    2

// CDC Class Config
#define USBD_CDC_INTERVAL                   2000

// Custom HID Config
#define USBD_CUSTOMHID_OUTREPORT_BUF_SIZE   64
#define USBD_CUSTOM_HID_REPORT_DESC_SIZE    (47)  //48

// Memory management macros
#define USBD_malloc                         malloc
#define USBD_free                           free
#define USBD_memset                         memset
#define USBD_memcpy                         memcpy

// DEBUG macros
#if (USBD_DEBUG_LEVEL > 0)
#define USBD_UsrLog(...)                    printf(__VA_ARGS__); printf("\n");
#else
#define USBD_UsrLog(...)
#endif

#if (USBD_DEBUG_LEVEL > 1)
#define USBD_ErrLog(...)                    printf("ERROR: "); printf(__VA_ARGS__); printf("\n");
#else
#define USBD_ErrLog(...)
#endif

#if (USBD_DEBUG_LEVEL > 2)
#define USBD_DbgLog(...)                    printf("DEBUG : "); printf(__VA_ARGS__); printf("\n");
#else
#define USBD_DbgLog(...)
#endif


#ifdef __cplusplus
}
#endif

#endif
