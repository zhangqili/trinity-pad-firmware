/*
 * Copyright (c) 2024 Zhangqi Li (@zhangqili)
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "key.h"
#include "advanced_key.h"
#include "keyboard_conf.h"
#include "keyboard_def.h"
#include "layer.h"
#include "keycode.h"

#define KEYBINDING(keycode, modifier) (((modifier) << 8) | (keycode))
#define KEY_KEYCODE(binding) ((binding) & 0xFF)
#define KEY_MODIFIER(binding) (((binding) >> 8) & 0xFF)

typedef struct
{
    uint8_t buffer[8];
    uint8_t keynum;
} Keyboard_6KROBuffer;

typedef struct
{
    uint8_t *buffer;
    uint8_t length;
} Keyboard_NKROBuffer;

typedef struct
{
    uint16_t id;
    uint8_t event;
} KeyboardEvent;
#define MK_EVENT(id, event) ((KeyboardEvent){(id), (event)})


extern Key g_keyboard_keys[KEY_NUM];
extern AdvancedKey g_keyboard_advanced_keys[ADVANCED_KEY_NUM];
extern const uint16_t g_default_keymap[LAYER_NUM][ADVANCED_KEY_NUM + KEY_NUM];
extern uint16_t g_keymap[LAYER_NUM][ADVANCED_KEY_NUM + KEY_NUM];

extern Keyboard_6KROBuffer g_keyboard_6kro_buffer;

extern uint8_t g_keyboard_knob_flag;
extern volatile bool g_keyboard_send_report_enable;

extern volatile bool g_debug_enable;
extern volatile bool g_keyboard_send_flag;


void keyboard_event_handler(KeyboardEvent event);

uint16_t keyboard_get_keycode(uint8_t id);
void keyboard_add_buffer(uint16_t keycode);

void keyboard_buffer_send(void);
void keyboard_buffer_clear(void);

int keyboard_6KRObuffer_add(Keyboard_6KROBuffer *buf, uint16_t key);
void keyboard_6KRObuffer_send(Keyboard_6KROBuffer *buf);
void keyboard_6KRObuffer_clear(Keyboard_6KROBuffer *buf);

void keyboard_NKRObuffer_init(Keyboard_NKROBuffer*buf,uint8_t* data_buf,uint8_t length);
int keyboard_NKRObuffer_add(Keyboard_NKROBuffer*buf,uint16_t key);
void keyboard_NKRObuffer_send(Keyboard_NKROBuffer*buf);
void keyboard_NKRObuffer_clear(Keyboard_NKROBuffer*buf);

void keyboard_init(void);
void keyboard_system_reset(void);
void keyboard_factory_reset(void);
void keyboard_jump_to_bootloader(void);
void keyboard_user_handler(uint8_t code);
void keyboard_scan(void);
void keyboard_send_report(void);
void keyboard_post_process(void);
void keyboard_recovery(void);
void keyboard_save(void);
void keyboard_task(void);
void keyboard_delay(uint32_t ms);
void keyboard_hid_send(uint8_t *report, uint16_t len);

#endif /* KEYBOARD_H_ */
