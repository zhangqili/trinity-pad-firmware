/*
 * fezui_calibrationpage.c
 *
 *  Created on: 2023年6月24日
 *      Author: xq123
 */

#include "fezui.h"
#include "fezui_var.h"
#include "analog.h"

fezui_list_base_t calibrationmenu;
const char *calibrationmenu_items[] = {"KEY1", "KEY2", "KEY3", "KEY4"};

uint8_t calibrationstate = 0;

void drawrawdata(uint8_t x, uint8_t y, uint8_t n)
{
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_4x6_mr);

    fezui_printf(&fezui, x, y + 7 * 1, "state:%d", g_keyboard_advanced_keys[n].key.state);

    fezui_printf(&fezui, x, y + 7 * 2, "raw:%4.1f", g_keyboard_advanced_keys[n].raw);

    fezui_printf(&fezui, x + 64, y + 7 * 2, "value:%.3f", g_keyboard_advanced_keys[n].value);

    fezui_printf(&fezui, x, y + 7 * 3, "mode:%d", g_keyboard_advanced_keys[n].mode);

    fezui_printf(&fezui, x, y + 7 * 4, "distance:(%.3f, %.3f)", g_keyboard_advanced_keys[n].trigger_distance, g_keyboard_advanced_keys[n].release_distance);

    fezui_printf(&fezui, x, y + 7 * 5, "speed:(%.3f, %.3f)", g_keyboard_advanced_keys[n].trigger_speed, g_keyboard_advanced_keys[n].release_speed);

    fezui_printf(&fezui, x, y + 7 * 6, "deadzone:(%.3f, %.3f)", g_keyboard_advanced_keys[n].upper_deadzone, g_keyboard_advanced_keys[n].lower_deadzone);

    fezui_printf(&fezui, x, y + 7 * 7, "bound:(%.3f, %.3f)", g_keyboard_advanced_keys[n].upper_bound, g_keyboard_advanced_keys[n].lower_bound);

    fezui_printf(&fezui, x, y + 7 * 8, "range:%.3f", g_keyboard_advanced_keys[n].range);
}

void calibrationpage_init()
{
    fezui_list_base_init(&calibrationmenu, calibrationmenu_items, sizeof(calibrationmenu_items) / sizeof(const char *), NULL);
}

static void calibrationpage_tick(void *page)
{
    fezui_cursor_set(
        &g_target_cursor,
        100,
        0,
        28,
        9);
}

static void calibrationpage_draw(void *page)
{
    drawrawdata(0, 8, calibrationmenu.selected_index);
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_4x6_mr);
    // u8g2_DrawBox(&(fezui.u8g2), x, y, 13, 7);
    // u8g2_SetDrawColor(&(fezui.u8g2), 2);
    fezui_printf(&fezui, 0, 7, "Press OK to reset range");
    fezui_printf(&fezui, 102, 7, "<%s>", calibrationmenu.items[calibrationmenu.selected_index]);
    // u8g2_SetDrawColor(&(fezui.u8g2), 1);
    fezui_draw_cursor(&fezui, &g_fezui_cursor);
}

static void calibrationpage_event_handler(void *e)
{
    switch (*(uint16_t *)e)
    {
    case KEY_UP_ARROW:
        fezui_list_base_index_increase(&calibrationmenu, -1);
        break;
    case KEY_DOWN_ARROW:
        fezui_list_base_index_increase(&calibrationmenu, 1);
        break;
    case KEY_ENTER:
        analog_reset_range();
        break;
    case KEY_ESC:
        fezui_link_frame_go_back(&mainframe);
        break;
    default:
        break;
    }
}
static void calibrationpage_load(void *page)
{
    calibrationstate = 0;
}

fezui_link_page_t calibrationpage = {calibrationpage_tick, calibrationpage_draw, calibrationpage_load, calibrationpage_event_handler};