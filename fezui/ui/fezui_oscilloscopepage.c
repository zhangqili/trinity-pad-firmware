/*
 * fezui_oscilloscopepage.c
 *
 *  Created on: 2023骞�6鏈�19鏃�
 *      Author: xq123
 */
#include "fezui.h"
#include "fezui_var.h"
#include "record.h"

static float offset_x = 0;
static float target_offset_x = 0;

void fezui_draw_wave(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w,
                     u8g2_uint_t h, LoopArray *arr, uint8_t *bits)
{
    uint8_t color_backup = u8g2_GetDrawColor(&(fezui_ptr->u8g2));
    u8g2_uint_t y1;
    u8g2_uint_t y2;
    for (u8g2_uint_t i = 0; i < w - 1; i++)
    {
        y1 = loop_array_get(arr, i) / 128;
        y2 = loop_array_get(arr, i + 1) / 128;
        if (y1 > h || y2 > h)
            if (!(bits[i / 8] & BIT(i % 8)))
                u8g2_DrawLine(&(fezui_ptr->u8g2), x + w - i - 1, y + 1, x + w - i - 2, y + 1);
            else
            {
                u8g2_DrawVLine(&(fezui_ptr->u8g2), x + w - i - 1, y + i, h - 1);
                u8g2_DrawLine(&(fezui_ptr->u8g2), x + w - i - 1, y + 1, x + w - i - 2, y + 1);
            }
        else if ((bits[i / 8] & BIT(i % 8)))
        {
            u8g2_DrawVLine(&(fezui_ptr->u8g2), x + w - i - 1, y + h - y1, y1);
            u8g2_DrawLine(&(fezui_ptr->u8g2), x + w - i - 1, y + h - y1, x + w - i - 2, y + h - y2);
        }
        else
            u8g2_DrawLine(&(fezui_ptr->u8g2), x + w - i - 1, y + h - y1, x + w - i - 2, y + h - y2);
    }
    y1 = loop_array_get(arr, w - 1) / 128;
    if ((bits[(w - 1) / 8] & BIT((w - 1) % 8)))
    {
        u8g2_DrawVLine(&(fezui_ptr->u8g2), x, y + h - y1, y1);
    }
    u8g2_SetFont(&(fezui_ptr->u8g2), u8g2_font_micro_tr);
    u8g2_DrawBox(&(fezui_ptr->u8g2), x, y, 17, 7);
    u8g2_SetDrawColor(&(fezui_ptr->u8g2), 2);
    fezui_printf(fezui_ptr, x + 1, y + 6, "%04d", loop_array_get(arr, 0));
    u8g2_SetDrawColor(&(fezui_ptr->u8g2), color_backup);
    // u8g2_SetDrawColor(&(fezui_ptr->u8g2), !fezui_ptr->invert);
}

void fezui_draw_detailed_wave(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, AdvancedKey *key, LoopArray *arr, uint8_t *bits)
{
    uint8_t color_backup = u8g2_GetDrawColor(&(fezui_ptr->u8g2));
    u8g2_uint_t y1;
    u8g2_uint_t y2;
    u8g2_uint_t w = 128;
    u8g2_uint_t h = 64;

    for (u8g2_uint_t i = 0; i < w - 1; i++)
    {
        y1 = loop_array_get(arr, i) / 64;
        y2 = loop_array_get(arr, i + 1) / 64;
        if (y1 > h || y2 > h)
            if (!(bits[i / 8] & BIT(i % 8)))
                u8g2_DrawLine(&(fezui_ptr->u8g2), x + w - i - 1, y + 1, x + w - i - 2, y + 1);
            else
            {
                u8g2_DrawVLine(&(fezui_ptr->u8g2), x + w - i - 1, y + i, h - 1);
                u8g2_DrawLine(&(fezui_ptr->u8g2), x + w - i - 1, y + 1, x + w - i - 2, y + 1);
            }
        else if ((bits[i / 8] & BIT(i % 8)))
        {
            u8g2_DrawVLine(&(fezui_ptr->u8g2), x + w - i - 1, y + h - y1, y1);
            u8g2_DrawLine(&(fezui_ptr->u8g2), x + w - i - 1, y + h - y1, x + w - i - 2, y + h - y2);
        }
        else
            u8g2_DrawLine(&(fezui_ptr->u8g2), x + w - i - 1, y + h - y1, x + w - i - 2, y + h - y2);
    }
    if ((bits[(w - 1) / 8] & BIT((w - 1) % 8)))
    {
        u8g2_DrawVLine(&(fezui_ptr->u8g2), x, y + h - y1, y1);
    }
    u8g2_SetFont(&(fezui_ptr->u8g2), u8g2_font_micro_tr);
    u8g2_DrawBox(&(fezui_ptr->u8g2), x, y, 17, 7);
    u8g2_SetDrawColor(&(fezui_ptr->u8g2), 2);
    fezui_printf(fezui_ptr, x + 1, y + 6, "%04d", loop_array_get(arr, 0));
    u8g2_SetDrawColor(&(fezui_ptr->u8g2), color_backup);
    // u8g2_SetDrawColor(&(fezui_ptr->u8g2), !fezui_ptr->invert);
    
}

static void oscilloscopepage_tick(void *page)
{
    record_analog_timer();
    CONVERGE_TO_ROUNDED(offset_x, target_offset_x, fezui.speed);
}
static void oscilloscopepage_draw(void *page)
{
    fezui_draw_wave(&(fezui), 0 - (u8g2_int_t)offset_x, 0, 64, 32, g_analog_historys + 0, (uint8_t *)g_bit_stream_datas[0]);
    fezui_draw_wave(&(fezui), 64 - (u8g2_int_t)offset_x, 0, 64, 32, g_analog_historys + 1, (uint8_t *)g_bit_stream_datas[1]);
    fezui_draw_wave(&(fezui), 0 - (u8g2_int_t)offset_x, 32, 64, 32, g_analog_historys + 2, (uint8_t *)g_bit_stream_datas[2]);
    fezui_draw_wave(&(fezui), 64 - (u8g2_int_t)offset_x, 32, 64, 32, g_analog_historys + 3, (uint8_t *)g_bit_stream_datas[3]);
    fezui_draw_detailed_wave(&fezui, 128 * 1 - (u8g2_int_t)offset_x, 0, g_keyboard_advanced_keys + 0, g_analog_historys + 0, (uint8_t *)g_bit_stream_datas[0]);
    fezui_draw_detailed_wave(&fezui, 128 * 2 - (u8g2_int_t)offset_x, 0, g_keyboard_advanced_keys + 1, g_analog_historys + 1, (uint8_t *)g_bit_stream_datas[1]);
    fezui_draw_detailed_wave(&fezui, 128 * 3 - (u8g2_int_t)offset_x, 0, g_keyboard_advanced_keys + 2, g_analog_historys + 2, (uint8_t *)g_bit_stream_datas[2]);
    fezui_draw_detailed_wave(&fezui, 128 * 4 - (u8g2_int_t)offset_x, 0, g_keyboard_advanced_keys + 3, g_analog_historys + 3, (uint8_t *)g_bit_stream_datas[3]);
}

static void oscilloscopepage_load(void *page)
{
    target_offset_x = 0;
}

static void oscilloscopepage_event_handler(void *e)
{
    switch (*(uint16_t *)e)
    {
    case KEY_UP_ARROW:
        target_offset_x += 128;
        if (target_offset_x > 128 * 4)
        {
            target_offset_x = 0;
        }
        break;
    case KEY_DOWN_ARROW:
        target_offset_x -= 128;
        if (target_offset_x < 0)
        {
            target_offset_x = 128 * 4;
        }
        break;
    case KEY_ENTER:
        break;
    case KEY_ESC:
        fezui_frame_go_back(&g_mainframe);
        fezui_cursor_set(&g_fezui_cursor, 0, 0, WIDTH, HEIGHT);
        break;
    default:
        break;
    }
}
fezui_page_t oscilloscopepage = {oscilloscopepage_tick, oscilloscopepage_draw, oscilloscopepage_load, oscilloscopepage_event_handler};
