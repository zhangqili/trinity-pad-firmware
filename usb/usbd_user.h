#ifndef __HID_KEYBOARD_H
#define __HID_KEYBOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "usbd_core.h"
#include "usbd_hid.h"


#define HID_KEYBOARD_INT_EP          0x81
#define HID_KEYBOARD_INT_EP_SIZE     8
#define HID_KEYBOARD_INT_EP_INTERVAL 1

#define HID_MOUSE_INT_EP          0x82
#define HID_MOUSE_INT_EP_SIZE     12
#define HID_MOUSE_INT_EP_INTERVAL 1

/*!< hidraw in endpoint */
#define HIDRAW_IN_EP       0x83
#define HIDRAW_IN_EP_SIZE  64
#define HIDRAW_IN_INTERVAL 1

/*!< hidraw out endpoint */
#define HIDRAW_OUT_EP          0x01
#define HIDRAW_OUT_EP_SIZE     64
#define HIDRAW_OUT_EP_INTERVAL 1

extern uint32_t g_usb_report_count;
extern uint32_t g_usb_report_count1;
extern uint32_t g_usb_mouse_report_count;
extern uint32_t g_usb_mouse_report_count1;
extern uint32_t g_usb_raw_report_count;
extern uint32_t g_usb_raw_report_count1;

extern uint32_t g_usb_keyboard_interval;

void hid_init(void);
void hid_keyboard_send(uint8_t*buffer);
void hid_mouse_send(uint8_t*buffer);
void hid_raw_send(uint8_t*buffer,int size);
void hid_keyboard_test(void);
void hid_mouse_test(void);

#ifdef __cplusplus
}
#endif

#endif
