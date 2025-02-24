#ifndef __HID_KEYBOARD_H
#define __HID_KEYBOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "usbd_core.h"
#include "usbd_hid.h"

#define USBD_VID           0xffff
#define USBD_PID           0xffff
#define USBD_MAX_POWER     300
#define USBD_LANGID_STRING 1033

#define HID_KEYBOARD_INT_EP          0x81
#define HID_KEYBOARD_INT_EP_SIZE     8
#define HID_KEYBOARD_INT_EP_INTERVAL 1

#define HID_MOUSE_INT_EP          0x82
#define HID_MOUSE_INT_EP_SIZE     64
#define HID_MOUSE_INT_EP_INTERVAL 1

/*!< hidraw in endpoint */
#define HIDRAW_IN_EP       0x83
#define HIDRAW_IN_EP_SIZE  64
#define HIDRAW_IN_INTERVAL 1

/*!< hidraw out endpoint */
#define HIDRAW_OUT_EP          0x01
#define HIDRAW_OUT_EP_SIZE     64
#define HIDRAW_OUT_EP_INTERVAL 1

#define HID_KEYBOARD_REPORT_DESC_SIZE 63
#define HID_MOUSE_REPORT_DESC_SIZE 52
#define HID_RAW_REPORT_DESC_SIZE 38

#define USB_HID_CONFIG_DESC_SIZ       (9+25+25+32)

#define usb_device_descriptor hid_descriptor
#define usb_device_descriptor_length hid_descriptor[0]
#define usb_config_descriptor (&hid_descriptor[18])
#define usb_config_descriptor_length ((uint16_t)hid_descriptor[20] + ((uint16_t)hid_descriptor[21] << 8))
#define usb_string0_descriptor (hid_descriptor+18+USB_HID_CONFIG_DESC_SIZ)
#define usb_string0_descriptor_length (hid_descriptor[18+USB_HID_CONFIG_DESC_SIZ])
#define usb_string1_descriptor (usb_string0_descriptor+usb_string0_descriptor_length)
#define usb_string1_descriptor_length (*usb_string1_descriptor)
#define usb_string2_descriptor (usb_string1_descriptor+usb_string1_descriptor_length)
#define usb_string2_descriptor_length (*usb_string2_descriptor)
#define usb_serial_descriptor (usb_string2_descriptor+usb_string2_descriptor_length)
#define usb_serial_descriptor_length (*usb_serial_descriptor)
#define usb_qua_descriptor (usb_serial_descriptor+usb_serial_descriptor_length)
#define usb_qua_descriptor_length (*usb_qua_descriptor)

extern const uint8_t hid_descriptor[];
extern const uint8_t hid_keyboard_report_desc[HID_KEYBOARD_REPORT_DESC_SIZE];
extern const uint8_t hid_mouse_report_desc[HID_MOUSE_REPORT_DESC_SIZE];
extern const uint8_t hid_raw_report_desc[HID_RAW_REPORT_DESC_SIZE];

extern uint32_t g_usb_report_count;
extern uint32_t g_usb_report_count1;
extern uint32_t g_usb_mouse_report_count;
extern uint32_t g_usb_mouse_report_count1;
extern uint32_t g_usb_raw_report_count;
extern uint32_t g_usb_raw_report_count1;

extern uint32_t g_usb_keyboard_interval;

void hid_init(void);
int hid_keyboard_send(uint8_t*buffer);
int hid_mouse_send(uint8_t*buffer);
int hid_raw_send(uint8_t*buffer,int size);
void hid_keyboard_test(void);
void hid_mouse_test(void);

#ifdef __cplusplus
}
#endif

#endif
