/*
 * fezui_calibrationpage.c
 *
 *  Created on: 2023年6月24日
 *      Author: xq123
 */

#include "fezui.h"
#include "fezui_var.h"
#include "analog.h"

#define MARGIN_LEFT 2
#define MARGIN_TOP 15
#define MARGIN_RIGHT 2
#define ROW_HEIGHT 7

static enum CALIBRATION_STATE {
    CALIBRATION_IDLE,
    CALIBRATION_BEGIN,
    CALIBRATION_PRESS,
    CALIBRATION_FINISH
} calibration_state;

static fezui_list_base_t calibrationmenu;
static const char *calibrationmenu_items[] = {"KEY1", "KEY2", "KEY3", "KEY4"};
static fezui_cursor_t fake_cursor;
static fezui_cursor_t fake_target_cursor = {WIDTH, 0, 0, ROW_HEIGHT};

static float max_values[ADVANCED_KEY_NUM];
static float min_values[ADVANCED_KEY_NUM];

#define PRESS_PREIOD (288)
static uint16_t calibration_time;

static void calibrate_upper_bound()
{

    for (int i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        g_keyboard_advanced_keys[i].config.upper_bound = g_ADC_Averages[i];
        max_values[i] = g_ADC_Averages[i];
        min_values[i] = g_ADC_Averages[i];
    }
}

static void calibrate_lower_bound()
{
    for (int i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        if (fabsf(g_keyboard_advanced_keys[i].config.upper_bound - max_values[i]) > fabsf(g_keyboard_advanced_keys[i].config.upper_bound - min_values[i]))
        {
            g_keyboard_advanced_keys[i].config.lower_bound = max_values[i];
            g_keyboard_advanced_keys[i].config.calibration_mode = KEY_AUTO_CALIBRATION_POSITIVE;
        }
        else
        {
            g_keyboard_advanced_keys[i].config.lower_bound = min_values[i];
            g_keyboard_advanced_keys[i].config.calibration_mode = KEY_AUTO_CALIBRATION_NEGATIVE;
        }
    }
    calibration_state = CALIBRATION_FINISH;
}

static void drawrawdata(uint8_t x, uint8_t y, uint8_t n)
{
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_4x6_mr);

    fezui_printf(&fezui, MARGIN_LEFT, MARGIN_TOP + ROW_HEIGHT * 1, "state");
    fezui_printf_right_aligned(&fezui, WIDTH / 2 - MARGIN_RIGHT, MARGIN_TOP + ROW_HEIGHT * 1, "%s", g_keyboard_advanced_keys[n].key.state ? "True" : "False");

    fezui_printf(&fezui, WIDTH / 2 + MARGIN_LEFT, MARGIN_TOP + ROW_HEIGHT * 1, "mode");
    fezui_printf_right_aligned(&fezui, WIDTH - MARGIN_RIGHT, MARGIN_TOP + ROW_HEIGHT * 1, "%d", g_keyboard_advanced_keys[n].config.mode);

    fezui_printf(&fezui, MARGIN_LEFT, MARGIN_TOP + ROW_HEIGHT * 2, "raw");
    fezui_printf_right_aligned(&fezui, WIDTH / 2 - MARGIN_RIGHT, MARGIN_TOP + ROW_HEIGHT * 2, "%4.3f", g_keyboard_advanced_keys[n].raw);

    fezui_printf(&fezui, WIDTH / 2 + MARGIN_LEFT, MARGIN_TOP + ROW_HEIGHT * 2, "value");
    fezui_printf_right_aligned(&fezui, WIDTH - MARGIN_RIGHT, MARGIN_TOP + ROW_HEIGHT * 2, "%.3f", g_keyboard_advanced_keys[n].value);

    fezui_printf(&fezui, MARGIN_LEFT, MARGIN_TOP + ROW_HEIGHT * 3, "distance");
    fezui_printf_right_aligned(&fezui, WIDTH - MARGIN_RIGHT, MARGIN_TOP + ROW_HEIGHT * 3, "(%.3f, %.3f)", g_keyboard_advanced_keys[n].config.trigger_distance, g_keyboard_advanced_keys[n].config.release_distance);

    fezui_printf(&fezui, MARGIN_LEFT, MARGIN_TOP + ROW_HEIGHT * 4, "speed");
    fezui_printf_right_aligned(&fezui, WIDTH - MARGIN_RIGHT, MARGIN_TOP + ROW_HEIGHT * 4, "(%.3f, %.3f)", g_keyboard_advanced_keys[n].config.trigger_speed, g_keyboard_advanced_keys[n].config.release_speed);

    fezui_printf(&fezui, MARGIN_LEFT, MARGIN_TOP + ROW_HEIGHT * 5, "deadzone");
    fezui_printf_right_aligned(&fezui, WIDTH - MARGIN_RIGHT, MARGIN_TOP + ROW_HEIGHT * 5, "(%.3f, %.3f)", g_keyboard_advanced_keys[n].config.upper_deadzone, g_keyboard_advanced_keys[n].config.lower_deadzone);

    fezui_printf(&fezui, MARGIN_LEFT, MARGIN_TOP + ROW_HEIGHT * 6, "bound");
    fezui_printf_right_aligned(&fezui, WIDTH - MARGIN_RIGHT, MARGIN_TOP + ROW_HEIGHT * 6, "(%.3f, %.3f)", g_keyboard_advanced_keys[n].config.upper_bound, g_keyboard_advanced_keys[n].config.lower_bound);

    fezui_printf(&fezui, MARGIN_LEFT, MARGIN_TOP + ROW_HEIGHT * ROW_HEIGHT, "calibration mode");
    switch (g_keyboard_advanced_keys[n].config.calibration_mode)
    {
    case KEY_NO_CALIBRATION:
        fezui_printf_right_aligned(&fezui, WIDTH - MARGIN_RIGHT, MARGIN_TOP + ROW_HEIGHT * 7, "OFF");
        break;

    case KEY_AUTO_CALIBRATION_POSITIVE:
        fezui_printf_right_aligned(&fezui, WIDTH - MARGIN_RIGHT, MARGIN_TOP + ROW_HEIGHT * 7, "Positive");
        break;
    case KEY_AUTO_CALIBRATION_NEGATIVE:
        fezui_printf_right_aligned(&fezui, WIDTH - MARGIN_RIGHT, MARGIN_TOP + ROW_HEIGHT * 7, "Negative");
        break;
    default:
        break;
    }
}

void calibrationpage_init()
{
    fezui_list_base_init(&calibrationmenu, (void **)calibrationmenu_items, sizeof(calibrationmenu_items) / sizeof(const char *), NULL);
}

static void calibrationpage_tick(void *page)
{
    switch (calibration_state)
    {
    case CALIBRATION_BEGIN:
        fezui_cursor_set(
            &g_target_cursor,
            128,
            0,
            0,
            7);
        break;
    case CALIBRATION_PRESS:
        for (int i = 0; i < ADVANCED_KEY_NUM; i++)
        {
            if (g_ADC_Averages[i] > max_values[i])
            {
                max_values[i] = g_ADC_Averages[i];
            }
            if (g_ADC_Averages[i] < min_values[i])
            {
                min_values[i] = g_ADC_Averages[i];
            }
        }

        calibration_time++;
        if (calibration_time > PRESS_PREIOD)
        {
            calibrate_lower_bound();
        }
        fezui_cursor_set(
            &g_fezui_cursor,
            0,
            0,
            WIDTH * (calibration_time / (float)PRESS_PREIOD),
            7);
        fezui_cursor_set(
            &g_target_cursor,
            0,
            0,
            WIDTH * (calibration_time / (float)PRESS_PREIOD),
            7);

        break;
    default:
        fezui_cursor_set(
            &g_target_cursor,
            100,
            0,
            28,
            7);
        break;
    }
    fezui_cursor_move(&fezui, &fake_cursor, &fake_target_cursor);
}

static void calibrationpage_draw(void *page)
{
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_4x6_mr);
    switch (calibration_state)
    {
    case CALIBRATION_IDLE:
        drawrawdata(0, 15, calibrationmenu.selected_index);
        // u8g2_DrawBox(&(fezui.u8g2), x, y, 13, ROW_HEIGHT);
        // u8g2_SetDrawColor(&(fezui.u8g2), 2);
        fezui_printf(&fezui, MARGIN_LEFT, ROW_HEIGHT, "Release all the keys");
        fezui_printf(&fezui, MARGIN_LEFT, ROW_HEIGHT * 2, "and press OK to start.");
        fezui_printf_right_aligned(&fezui, WIDTH - MARGIN_RIGHT, ROW_HEIGHT, "<%s>", calibrationmenu.items[calibrationmenu.selected_index]);
        // u8g2_SetDrawColor(&(fezui.u8g2), 1);
        break;
    case CALIBRATION_PRESS:
        fezui_draw_cursor(&fezui, &fake_cursor);
        fezui_printf(&fezui, 1, ROW_HEIGHT, "Press all the keys to bottom");
        break;
    case CALIBRATION_FINISH:
        drawrawdata(0, 15, calibrationmenu.selected_index);
        fezui_printf(&fezui, 1, ROW_HEIGHT, "Calibration over.");
        fezui_printf(&fezui, 102, ROW_HEIGHT, "<%s>", calibrationmenu.items[calibrationmenu.selected_index]);
        break;
    default:
        break;
    }
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
    case KEY_SPACEBAR:
    case KEY_ENTER:
        switch (calibration_state)
        {
        case CALIBRATION_FINISH:
        case CALIBRATION_IDLE:
            calibration_state = CALIBRATION_PRESS;
            memcpy(&fake_cursor, &g_fezui_cursor, sizeof(fezui_cursor_t));
            fezui_cursor_set(&g_fezui_cursor, 0, 0, 0, 8);
            fezui_cursor_set(&g_target_cursor, 0, 0, 0, 8);
            calibration_time = 0;
            // analog_average();
            calibrate_upper_bound();
            break;
        case CALIBRATION_PRESS:
            //calibration_state = CALIBRATION_FINISH;
            break;
        default:
            calibration_state = CALIBRATION_IDLE;
            break;
        }
        // analog_reset_range();
        break;
    case KEY_ESC:
        fezui_frame_go_back(&g_mainframe);
        break;
    default:
        break;
    }
}
static void calibrationpage_load(void *page)
{
    calibration_state = CALIBRATION_IDLE;
}

fezui_page_t calibrationpage = {calibrationpage_tick, calibrationpage_draw, calibrationpage_load, calibrationpage_event_handler};