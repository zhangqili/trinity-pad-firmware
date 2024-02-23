/*
 * fezui_debugpage.c
 *
 *  Created on: 2023骞�6鏈�24鏃�
 *      Author: xq123
 */
#include "fezui.h"
#include "fezui_var.h"
#include "main.h"

fezui_link_page_t debugpage = {debugpage_logic, debugpage_draw, debugpage_load};
static float target_ordinate = 0;
static float target_abscissa = 0;

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

void debugpage_logic(void *page)
{
    fezui_rolling_number_update(&fezui, &rolling_number);
    fezui_flyout_numberic_dialog_update(&fezui, &dialog);
}
void debugpage_draw(void *page)
{

    sprintf(fezui_buffer, "%#x", fezui_debug);
    u8g2_DrawStr(&(fezui.u8g2), 64, 16, fezui_buffer);

    // u8g2_SetFont(&fezui.u8g2, u8g2_font_8x13B_mf);
    sprintf(fezui_buffer, "%d", (short)TIM8->CNT);
    u8g2_DrawStr(&(fezui.u8g2), 90, 60, fezui_buffer);

    u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x8_mr);
    // fezui_draw_animated_listbox(&fezui, 0, 0, WIDTH, HEIGHT, &listbox, 8, 1);
    // fezui_animated_listbox_get_cursor(&fezui, 0, 0, WIDTH, HEIGHT, &listbox, 8, &target_cursor);
    extern uint8_t read_buffer[64];
    sprintf(fezui_buffer, "%d", read_buffer[0]);
    u8g2_DrawStr(&(fezui.u8g2), 0, 64, read_buffer+1);

    fezui_draw_flyout_numberic_dialog(&fezui, &dialog);
    fezui_draw_cursor(&fezui, &cursor);
}

void debugpage_load(void *page)
{
    fezui_flyout_numberic_dialog_init(&dialog, &targetnum, FEZUI_TYPE_FLOAT, 0, 100, 0.1, "NUMBER");
    fezui_flyout_numberic_dialog_show(&dialog);
    key_attach(&KEY_FN_K5, KEY_EVENT_DOWN, LAMBDA(
                                                    void, (void *k) {fezui_link_frame_go_back(&mainframe);fezui_cursor_set(&cursor ,0 ,0 ,WIDTH ,HEIGHT); }));
    key_attach(&KEY_FN_K6, KEY_EVENT_DOWN, LAMBDA(
                                                    void, (void *k) {fezui_link_frame_go_back(&mainframe);fezui_cursor_set(&cursor ,0 ,0 ,WIDTH ,HEIGHT); }));
    key_attach(&KEY_KNOB, KEY_EVENT_DOWN, LAMBDA(
                                                   void, (void *k) {fezui_link_frame_go_back(&mainframe);fezui_cursor_set(&cursor ,0 ,0 ,WIDTH ,HEIGHT); }));
    key_attach(&KEY_KNOB_CLOCKWISE, KEY_EVENT_DOWN, LAMBDA(
                                                             void, (void *k) { fezui_numberic_dialog_increase(&dialog, 1); }));
    key_attach(&KEY_KNOB_ANTICLOCKWISE, KEY_EVENT_DOWN, LAMBDA(
                                                                 void, (void *k) { fezui_numberic_dialog_increase(&dialog, -1); }));
}
