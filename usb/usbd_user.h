#ifndef __HID_KEYBOARD_H
#define __HID_KEYBOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "usbd_core.h"
#include "usbd_hid.h"

#define USBD_LANGID_STRING 1033

typedef struct __USBBuffer
{
    uint8_t read_buffer[64];
    uint8_t send_buffer[64];
    uint8_t state;
} USBBuffer;

extern uint32_t g_usb_report_count;
extern uint32_t g_usb_report_count1;
extern uint32_t g_usb_mouse_report_count;
extern uint32_t g_usb_mouse_report_count1;
extern uint32_t g_usb_raw_report_count;
extern uint32_t g_usb_raw_report_count1;

extern uint32_t g_usb_keyboard_interval;

extern USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX USBBuffer raw_buffer;

void usb_init(void);
int usb_send_shared_ep(uint8_t *buffer, uint8_t size);
int usb_send_keyboard(uint8_t *buffer, uint8_t size);
int usb_send_raw(uint8_t *buffer, uint8_t size);
int usb_send_midi(uint8_t *buffer, uint8_t size);

#ifdef __cplusplus
}
#endif

#endif
