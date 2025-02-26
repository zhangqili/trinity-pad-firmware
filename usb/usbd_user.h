#ifndef __HID_KEYBOARD_H
#define __HID_KEYBOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "usbd_core.h"
#include "usbd_hid.h"

#define USBD_LANGID_STRING 1033

extern uint32_t g_usb_report_count;
extern uint32_t g_usb_report_count1;
extern uint32_t g_usb_mouse_report_count;
extern uint32_t g_usb_mouse_report_count1;
extern uint32_t g_usb_raw_report_count;
extern uint32_t g_usb_raw_report_count1;

extern uint32_t g_usb_keyboard_interval;

void usb_init(void);
int hid_keyboard_send(uint8_t *buffer, uint8_t size);
int hid_mouse_send(uint8_t*buffer);
int hid_raw_send(uint8_t*buffer,int size);
int hid_extra_send(uint8_t report_id, uint16_t usage);

#ifdef __cplusplus
}
#endif

#endif
