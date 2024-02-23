#ifndef __HID_KEYBOARD_H
#define __HID_KEYBOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "usbd_core.h"
#include "usbd_hid.h"


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
