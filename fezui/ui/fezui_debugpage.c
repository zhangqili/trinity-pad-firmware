/*
 * fezui_debugpage.c
 *
 *  Created on: 2023骞�6鏈�24鏃�
 *      Author: xq123
 */
#include "fezui.h"
#include "fezui_var.h"
#include "main.h"
#include "usbd_user.h"
#include "analog.h"

// static float target_ordinate = 0;
// static float target_abscissa = 0;

static uint16_t targetnum = 50;

static fezui_rolling_number_t rolling_number =
    {
        .digit = 5,
        .number = 0,
        .font = u8g2_font_micro_mn,
};

static fezui_animated_listbox_t listbox;
static fezui_flyout_numberic_dialog_t dialog;
static fezui_rangebase_t rangebase;

void listbox_cb(void *l)
{
}

void debugpage_init()
{
    fezui_animated_string_listbox_init(&listbox, g_hid_usage_names, sizeof(g_hid_usage_names) / sizeof(const char *), listbox_cb);
    listbox.listbox.show_scrollbar = true;
    fezui_rangebase_init(&rangebase,&targetnum,FEZUI_TYPE_UINT16,0,100,2);
}

static void debugpage_tick(void *page)
{
    fezui_rolling_number_update(&fezui, &rolling_number);
    fezui_flyout_numberic_dialog_update(&fezui, &dialog);
}
uint8_t debug[8];
static void debugpage_draw(void *page)
{
    //fezui_printf(&fezui, 64, 16, "%#lx", g_fezui_debug);

    u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x8_mr);
    // fezui_draw_animated_listbox(&fezui, 0, 0, WIDTH, HEIGHT, &listbox, 8, 1);
    // fezui_animated_listbox_get_cursor(&fezui, 0, 0, WIDTH, HEIGHT, &listbox, 8, &g_target_cursor);
    //u8g2_DrawUTF8(&(fezui.u8g2), 0, 64, (char *)read_buffer + 1);
    fezui_printf(&fezui,32,8,"%d",fezui.contrast);
    fezui_printf(&fezui, 64, 8, "%f", (-1)*(&rangebase)->interval);
    fezui_printf(&fezui, 64, 8, "%d", (int16_t)((-1)*(&rangebase)->interval));
    //fezui_draw_flyout_numberic_dialog(&fezui, &dialog);

    for (uint8_t i = 0; i < 8; i++)
    {
        //fezui_printf_right_aligned(&fezui,(i+1)*16,24,"%d",debug[i]);
        //fezui_printf_right_aligned(&fezui,(i+1)*16,32,"%d",read_buffer[i]);
        //fezui_printf_right_aligned(&fezui,(i+1)*16,40,"%d",read_buffer[i+8]);
        //fezui_printf_right_aligned(&fezui,(i+1)*16,48,"%d",read_buffer[i+16]);
        //fezui_printf_right_aligned(&fezui,(i+1)*16,56,"%d",read_buffer[i+24]);
    }
    
    
    fezui_printf(&fezui,0,16,"%ld",g_keyboard_advanced_keys[0].value);
    fezui_printf(&fezui,32,16,"%ld",g_keyboard_advanced_keys[1].value);
    fezui_printf(&fezui,64,16,"%ld",g_keyboard_advanced_keys[2].value);
    fezui_printf(&fezui,96,16,"%ld",g_keyboard_advanced_keys[3].value);
    fezui_printf(&fezui,0,24,"%ld",g_keyboard_advanced_keys[0].raw);
    fezui_printf(&fezui,32,24,"%ld",g_keyboard_advanced_keys[1].raw);
    fezui_printf(&fezui,64,24,"%ld",g_keyboard_advanced_keys[2].raw);
    fezui_printf(&fezui,96,24,"%ld",g_keyboard_advanced_keys[3].raw);
    fezui_printf(&fezui,0,32,"%f", (g_keyboard_advanced_keys[0].upper_bound - g_keyboard_advanced_keys[0].raw) / (float)(g_keyboard_advanced_keys[0].upper_bound - g_keyboard_advanced_keys[0].lower_bound));
    fezui_printf(&fezui,32,32,"%f",(g_keyboard_advanced_keys[1].upper_bound - g_keyboard_advanced_keys[1].raw) / (float)(g_keyboard_advanced_keys[1].upper_bound - g_keyboard_advanced_keys[1].lower_bound));
    fezui_printf(&fezui,64,32,"%f",(g_keyboard_advanced_keys[2].upper_bound - g_keyboard_advanced_keys[2].raw) / (float)(g_keyboard_advanced_keys[2].upper_bound - g_keyboard_advanced_keys[2].lower_bound));
    fezui_printf(&fezui,96,32,"%f",(g_keyboard_advanced_keys[3].upper_bound - g_keyboard_advanced_keys[3].raw) / (float)(g_keyboard_advanced_keys[3].upper_bound - g_keyboard_advanced_keys[3].lower_bound));
    fezui_printf(&fezui,0,40,"%ld",g_keyboard_advanced_keys[0].trigger_distance);
    fezui_printf(&fezui,32,40,"%ld",g_keyboard_advanced_keys[1].trigger_distance);
    fezui_printf(&fezui,64,40,"%ld",g_keyboard_advanced_keys[2].trigger_distance);
    fezui_printf(&fezui,96,40,"%ld",g_keyboard_advanced_keys[3].trigger_distance);
    fezui_printf(&fezui,0,48,"%ld",g_keyboard_advanced_keys[0].release_distance);
    fezui_printf(&fezui,32,48,"%ld",g_keyboard_advanced_keys[1].release_distance);
    fezui_printf(&fezui,64,48,"%ld",g_keyboard_advanced_keys[2].release_distance);
    fezui_printf(&fezui,96,48,"%ld",g_keyboard_advanced_keys[3].release_distance);

    fezui_draw_cursor(&fezui, &g_fezui_cursor);
}

static void debugpage_load(void *page)
{
    g_keyboard_send_report_enable=true;
    fezui_flyout_numberic_dialog_init(&dialog, &targetnum, FEZUI_TYPE_FLOAT, 0, 100, 0.1, "NUMBER");
    fezui_flyout_numberic_dialog_show(&dialog);
}

static void debugpage_event_handler(void *e)
{
    switch (*(uint16_t *)e)
    {
    case KEY_UP_ARROW:
        fezui_rangebase_increase(&rangebase, 1);
        break;
    case KEY_DOWN_ARROW:
        fezui_rangebase_increase(&rangebase, -1);
        targetnum+=(uint16_t)((-1)*(&rangebase)->interval);
        break;
    case KEY_SPACEBAR:
    case KEY_ENTER:
        g_keyboard_send_report_enable=false;
        g_current_layer = 0;
        fezui_frame_go_back(&g_mainframe);
        break;
    case KEY_ESC:
        //fezui_frame_go_back(&g_mainframe);
        //g_keyboard_send_report_enable=false;
        break;
    default:
        break;
    }
}

fezui_page_t debugpage = {debugpage_tick, debugpage_draw, debugpage_load, debugpage_event_handler};
