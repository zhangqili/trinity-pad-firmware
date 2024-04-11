/*
 * fezui_displayconfigpage.c
 *
 *  Created on: Aug 24, 2023
 *      Author: xq123
 */
#include "fezui.h"
#include "fezui_var.h"

#define ROW_HEIGHT 16

static fezui_animated_listbox_t displayconfig_menu;
static uint16_t speed;
static const fezui_menuitem_t *displayconfig_menu_ptr_items[] =
{
    &(fezui_menuitem_t){"bInvert", &fezui.invert},
    &(fezui_menuitem_t){"8Contrast", &fezui.contrast},
    &(fezui_menuitem_t){"uSpeed", &speed},
    &(fezui_menuitem_t){"uTimeout", &fezui.screensaver_timeout},
};
//static const fezui_menuitem_t *displayconfig_menu_ptr_items[4];
void sliderdialog_show(void *target, uint8_t type, float min, float max, float interval, const char *title);
void displayconfig_menu_cb(void *m)
{
    extern fezui_page_t sliderdialog;
    switch (displayconfig_menu.listbox.list.selected_index)
    {
    case 0:
        sliderdialog_show(&fezui.invert, FEZUI_TYPE_BOOL, 0, 1, 1, "Invert");
        break;
    case 1:
        sliderdialog_show(&fezui.contrast, FEZUI_TYPE_UINT8, 0, 255, 1, "Contrast");
        break;
    case 2:
        sliderdialog_show(&speed, FEZUI_TYPE_UINT16, 1, 20, 1, "Speed");
        break;
    case 3:
        sliderdialog_show(&fezui.screensaver_timeout, FEZUI_TYPE_UINT16, 0, 600, 1, "Timeout");
        break;
    default:
        break;
    }
    fezui_frame_show_dialog(&g_mainframe, &sliderdialog);
}

void displayconfigpage_init()
{   
    fezui_animated_menu_list_init(&displayconfig_menu, displayconfig_menu_ptr_items, sizeof(displayconfig_menu_ptr_items) / sizeof(fezui_menuitem_t*), displayconfig_menu_cb);
}

static void displayconfigpage_tick(void *page)
{
    //fezui_animated_listbox_update(&fezui, &displayconfig_menu);
}

static void displayconfigpage_draw(void *page)
{
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_6x13_mr);
    fezui_draw_animated_listbox(&fezui, 0, 0, WIDTH, HEIGHT, &displayconfig_menu, 16);
    fezui_animated_listbox_get_cursor(&fezui, 0, 0, WIDTH, HEIGHT, &displayconfig_menu, 16, &g_target_cursor);
    fezui_draw_cursor(&fezui, &g_fezui_cursor);
    fezui.speed = ((float)speed) / 100;
    fezui_apply(&fezui);
}

static void displayconfigpage_load(void *page)
{
    fezui_animated_listbox_begin(&displayconfig_menu);
    speed = fezui.speed * 100;
}

static void displayconfigpage_event_handler(void *e)
{
    switch (*(uint16_t *)e)
    {
    case KEY_UP_ARROW:
        fezui_animated_listbox_index_increase(&displayconfig_menu, 1);
        break;
    case KEY_DOWN_ARROW:
        fezui_animated_listbox_index_increase(&displayconfig_menu, -1);
        break;
    case KEY_ENTER:
        displayconfig_menu_cb(&displayconfig_menu);
        break;
    case KEY_ESC:
        fezui_frame_go_back(&g_mainframe);
        break;
    default:
        break;
    }
}

fezui_page_t displayconfigpage = {displayconfigpage_tick, displayconfigpage_draw, displayconfigpage_load,displayconfigpage_event_handler};
