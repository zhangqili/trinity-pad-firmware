/*
 * Copyright (c) 2025 Zhangqi Li (@zhangqili)
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "joystick.h"
#include "keyboard_def.h"

Joystick g_joystick;

void joystick_add_buffer(Keycode keycode)
{
    if (keycode >= JOYSTICK_BUTTON_COUNT) return;

    g_joystick.buttons[keycode / 8] |= 1 << (keycode % 8);
    g_joystick.dirty = true;
}

int joystick_buffer_send(Joystick *buf)
{
    return joystick_hid_send((uint8_t*)buf, sizeof(Joystick));
}

__WEAK int joystick_hid_send(uint8_t *report, uint16_t len)
{
    UNUSED(report);
    UNUSED(len);
    return 0;
}