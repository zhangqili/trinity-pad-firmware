/*
 * fezui_debugpage.c
 *
 *  Created on: 2023骞�6鏈�24鏃�
 *      Author: xq123
 */
#include "fezui.h"
#include "fezui_var.h"
#include "main.h"

// static float target_ordinate = 0;
// static float target_abscissa = 0;

static float targetnum = 50;

static fezui_rolling_number_t rolling_number =
    {
        .digit = 5,
        .number = 0,
        .font = u8g2_font_micro_mn,
};

static fezui_animated_listbox_t listbox;
static fezui_flyout_numberic_dialog_t dialog;

void listbox_cb(void *l)
{
}

void debugpage_init()
{
    fezui_animated_listbox_init(&listbox, hid_usage_names, sizeof(hid_usage_names) / sizeof(const char *), listbox_cb);
    listbox.show_scrollbar = true;
}

static void debugpage_logic(void *page)
{
    fezui_rolling_number_update(&fezui, &rolling_number);
    fezui_flyout_numberic_dialog_update(&fezui, &dialog);
}
static void debugpage_draw(void *page)
{
    fezui_printf(&fezui, 64, 16, "%#lx", fezui_debug);

    // u8g2_SetFont(&fezui.u8g2, u8g2_font_8x13B_mf);
    fezui_printf(&fezui, 90, 60, "%d", (short)TIM8->CNT);

    u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x8_mr);
    // fezui_draw_animated_listbox(&fezui, 0, 0, WIDTH, HEIGHT, &listbox, 8, 1);
    // fezui_animated_listbox_get_cursor(&fezui, 0, 0, WIDTH, HEIGHT, &listbox, 8, &target_cursor);
    extern uint8_t read_buffer[64];
    u8g2_DrawStr(&(fezui.u8g2), 0, 64, (char *)read_buffer + 1);

    fezui_draw_flyout_numberic_dialog(&fezui, &dialog);
    fezui_draw_cursor(&fezui, &cursor);
}

static void debugpage_load(void *page)
{
    fezui_flyout_numberic_dialog_init(&dialog, &targetnum, FEZUI_TYPE_FLOAT, 0, 100, 0.1, "NUMBER");
    fezui_flyout_numberic_dialog_show(&dialog);

}

static void debugpage_event_handler(void *e)
{
    switch (*(uint16_t *)e)
    {
    case KEY_UP_ARROW:
        fezui_numberic_dialog_increase(&dialog.dialog, 1);
        break;
    case KEY_DOWN_ARROW:
        fezui_numberic_dialog_increase(&dialog.dialog, -1);
        break;
    case KEY_ENTER:
        fezui_link_frame_go_back(&mainframe);
        break;
    case KEY_ESC:
        fezui_link_frame_go_back(&mainframe);
        fezui_cursor_set(&cursor, 0, 0, WIDTH, HEIGHT);
        break;
    default:
        break;
    }
}

fezui_link_page_t debugpage = {debugpage_logic, debugpage_draw, debugpage_load, debugpage_event_handler};
