/********************************** (C) COPYRIGHT *******************************
 * File Name          : usbd_descriptor.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/18
 * Description        : All descriptors for the keyboard and mouse composite device.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/


#ifndef __USBD_DESC_H
#define __USBD_DESC_H

/*******************************************************************************/
/* Header File */
#include "stdint.h"

/*******************************************************************************/
/* Macro Definition */

#define HID_KEYBOARD_INT_EP          0x81
#define HID_KEYBOARD_INT_EP_SIZE     8
#define HID_KEYBOARD_INT_EP_INTERVAL 1

#define HID_MOUSE_INT_EP          0x82
#define HID_MOUSE_INT_EP_SIZE     4
#define HID_MOUSE_INT_EP_INTERVAL 1

/*!< hidraw in endpoint */
#define HIDRAW_IN_EP       0x83
#define HIDRAW_IN_EP_SIZE  64
#define HIDRAW_IN_INTERVAL 1

/*!< hidraw out endpoint */
#define HIDRAW_OUT_EP          0x01
#define HIDRAW_OUT_EP_SIZE     64
#define HIDRAW_OUT_EP_INTERVAL 1


#define USBD_VID           0xffff
#define USBD_PID           0xffff
#define USBD_MAX_POWER     300
#define USBD_LANGID_STRING 1033


#define USB_HID_CONFIG_DESC_SIZ       (9+25+25+32)
#define HID_KEYBOARD_REPORT_DESC_SIZE 63
#define HID_MOUSE_REPORT_DESC_SIZE 74
#define HID_RAW_REPORT_DESC_SIZE 38


/* File Version */
#define DEF_FILE_VERSION              0x01

/* USB Device Info */
#define DEF_USB_VID                   0xFFFF
#define DEF_USB_PID                   0xFFFF

/* USB Device Descriptor, Device Serial Number(bcdDevice) */
#define DEF_IC_PRG_VER                DEF_FILE_VERSION

/* USB Device Endpoint Size */
#define DEF_USBD_UEP0_SIZE            64     /* usb hs/fs device end-point 0 size */
/* HS */
#define DEF_USBD_HS_PACK_SIZE         512    /* usb hs device max bluk/int pack size */
#define DEF_USBD_HS_ISO_PACK_SIZE     1024   /* usb hs device max iso pack size */
/* FS */
#define DEF_USBD_FS_PACK_SIZE         64     /* usb fs device max bluk/int pack size */
#define DEF_USBD_FS_ISO_PACK_SIZE     1023   /* usb fs device max iso pack size */
/* LS */
#define DEf_USBD_LS_UEP0_SIZE         8      /* usb ls device end-point 0 size */
#define DEF_USBD_LS_PACK_SIZE         64     /* usb ls device max int pack size */

/* USB Device Endpoint1-6 Size */
/* HS */
#define DEF_USB_EP1_HS_SIZE           DEF_USBD_HS_PACK_SIZE
#define DEF_USB_EP2_HS_SIZE           DEF_USBD_HS_PACK_SIZE
#define DEF_USB_EP3_HS_SIZE           DEF_USBD_HS_PACK_SIZE
#define DEF_USB_EP4_HS_SIZE           DEF_USBD_HS_PACK_SIZE
#define DEF_USB_EP5_HS_SIZE           DEF_USBD_HS_PACK_SIZE
#define DEF_USB_EP6_HS_SIZE           DEF_USBD_HS_PACK_SIZE
/* FS */
#define DEF_USB_EP1_FS_SIZE           DEF_USBD_FS_PACK_SIZE
#define DEF_USB_EP2_FS_SIZE           DEF_USBD_FS_PACK_SIZE
#define DEF_USB_EP3_FS_SIZE           DEF_USBD_FS_PACK_SIZE
#define DEF_USB_EP4_FS_SIZE           DEF_USBD_FS_PACK_SIZE
#define DEF_USB_EP5_FS_SIZE           DEF_USBD_FS_PACK_SIZE
#define DEF_USB_EP6_FS_SIZE           DEF_USBD_FS_PACK_SIZE
/* LS */
/* ... */

/* USB Device Descriptor Length */
/* Note: If a descriptor does not exist, set the length to 0. */
#define DEF_USBD_DEVICE_DESC_LEN      ( (uint16_t)MyDevDescr[ 0 ] )
#define DEF_USBD_CONFIG_DESC_LEN      ( (uint16_t)MyCfgDescr[ 2 ] + ( (uint16_t)MyCfgDescr[ 3 ] << 8 ) )
#define DEF_USBD_REPORT_DESC_LEN_KB   HID_KEYBOARD_REPORT_DESC_SIZE
#define DEF_USBD_REPORT_DESC_LEN_MS   HID_MOUSE_REPORT_DESC_SIZE
#define DEF_USBD_REPORT_DESC_LEN_RAW  HID_RAW_REPORT_DESC_SIZE
#define DEF_USBD_LANG_DESC_LEN        ( (uint16_t)MyLangDescr[ 0 ] )
#define DEF_USBD_MANU_DESC_LEN        ( (uint16_t)MyManuInfo[ 0 ] )
#define DEF_USBD_PROD_DESC_LEN        ( (uint16_t)MyProdInfo[ 0 ] )
#define DEF_USBD_SN_DESC_LEN          ( (uint16_t)MySerNumInfo[ 0 ] )
#define DEF_USBD_QUALFY_DESC_LEN      ( (uint16_t)MyQuaDesc[ 0 ])
#define DEF_USBD_BOS_DESC_LEN         0
#define DEF_USBD_FS_OTH_DESC_LEN      0
#define DEF_USBD_HS_OTH_DESC_LEN      0




/*******************************************************************************/
/* Descriptor Declaration */
extern const uint8_t MyDevDescr[ ];
extern const uint8_t MyCfgDescr[ ];
extern const uint8_t KeyRepDesc[HID_KEYBOARD_REPORT_DESC_SIZE];
extern const uint8_t MouseRepDesc[HID_MOUSE_REPORT_DESC_SIZE];
extern const uint8_t HidRepDesc[HID_RAW_REPORT_DESC_SIZE];
extern const uint8_t MyQuaDesc[ ];
extern const uint8_t MyLangDescr[ ];
extern const uint8_t MyManuInfo[ ];
extern const uint8_t MyProdInfo[ ];
extern const uint8_t MySerNumInfo[ ];

#endif
