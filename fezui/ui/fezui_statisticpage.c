/*
 * fezui_statisticpage.c
 *
 *  Created on: Jun 25, 2023
 *      Author: xq123
 */
#include "fezui.h"
#include "fezui_var.h"
#include "record.h"

#define ROW_HEIGHT 10
#define KEY_WIDTH 5

static fezui_scrollview_t scrollview =
    {
        .content_height = 100,
        .content_width = 64,
        .ordinate = 0};

static float target_ordinate = 0;

static void statisticpage_tick(void *page)
{
    CONVERGE_TO_ROUNDED(scrollview.ordinate, target_ordinate, fezui.speed);
}
static void statisticpage_draw(void *page)
{

    u8g2_SetFont(&(fezui.u8g2), u8g2_font_helvB08_tr);
    fezui_printf(&fezui, 0, ROW_HEIGHT * 6 - (u8g2_int_t)(scrollview.ordinate), "- System");
    fezui_printf(&fezui, 0, ROW_HEIGHT * 1 - (u8g2_int_t)(scrollview.ordinate), "- Total hits");

    u8g2_SetFont(&fezui.u8g2, u8g2_font_5x8_mr);

    fezui_printf(&fezui, 0, ROW_HEIGHT * 2 - (u8g2_int_t)(scrollview.ordinate), "  KEY1");

    fezui_printf(&fezui, 0, ROW_HEIGHT * 3 - (u8g2_int_t)(scrollview.ordinate), "  KEY2");

    fezui_printf(&fezui, 0, ROW_HEIGHT * 4 - (u8g2_int_t)(scrollview.ordinate), "  KEY3");

    fezui_printf(&fezui, 0, ROW_HEIGHT * 5 - (u8g2_int_t)(scrollview.ordinate), "  KEY4");

    fezui_printf(&fezui, 0, ROW_HEIGHT * 7 - (u8g2_int_t)(scrollview.ordinate), "  Run time");

    fezui_printf_right_aligned(&fezui, WIDTH - 6, ROW_HEIGHT * 7 - (u8g2_int_t)(scrollview.ordinate),  "%02ldh%02ldm%02lds", g_fezui_run_time / 3600, g_fezui_run_time / 60 % 60, g_fezui_run_time % 60);

    fezui_printf_right_aligned(&fezui, WIDTH - 6, ROW_HEIGHT * 2 - (u8g2_int_t)(scrollview.ordinate), "%8ld", g_key_counts[0]);

    fezui_printf_right_aligned(&fezui, WIDTH - 6, ROW_HEIGHT * 3 - (u8g2_int_t)(scrollview.ordinate),  "%8ld", g_key_counts[1]);

    fezui_printf_right_aligned(&fezui, WIDTH - 6, ROW_HEIGHT * 4 - (u8g2_int_t)(scrollview.ordinate),  "%8ld", g_key_counts[2]);

    fezui_printf_right_aligned(&fezui, WIDTH - 6, ROW_HEIGHT * 5 - (u8g2_int_t)(scrollview.ordinate),  "%8ld", g_key_counts[3]);

    fezui_draw_scrollview(&fezui, 0, 0, 128, 64, &scrollview);
}

static void statisticpage_load(void *page)
{
}

static void statisticpage_event_handler(void *e)
{
    switch (*(uint16_t *)e)
    {
    case KEY_UP_ARROW:
        target_ordinate -= 10;
        if (target_ordinate < 0)
            target_ordinate = 0;
        break;
    case KEY_DOWN_ARROW:
        target_ordinate += 10;
        if (target_ordinate + 64 > scrollview.content_height)
            target_ordinate = scrollview.content_height - 64;
        break;
    case KEY_SPACEBAR:
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

fezui_page_t statisticpage = {statisticpage_tick, statisticpage_draw, statisticpage_load, statisticpage_event_handler};