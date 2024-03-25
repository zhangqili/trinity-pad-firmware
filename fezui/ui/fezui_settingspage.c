/*
 * fezui_settingspage.c
 *
 *  Created on: 2023骞�6鏈�19鏃�
 *      Author: xq123
 */
#include "fezui.h"
#include "fezui_var.h"
#include "main.h"
fezui_animated_listbox_t settingsmenu;
const char *settingsmenu_items[] = 
{
    "Keys Configuration", 
    "Keymap", 
    "RGB", 
    "Calibration", 
    "Display", 
    "Advanced Settings", 
    "About"};

#define ROW_HEIGHT 16
static void settings_menu_cb(void *menu);
void settingspage_init()
{
    fezui_animated_listbox_init(&settingsmenu, settingsmenu_items, sizeof(settingsmenu_items) / sizeof(const char *), settings_menu_cb);
    settingsmenu.show_scrollbar = true;
}

static void settingspage_tick(void *page)
{
    fezui_animated_listbox_update(&fezui, &settingsmenu);
}
static void settingspage_draw(void *page)
{
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_6x13_mr);
    fezui_draw_animated_listbox(&fezui, 0, 0, WIDTH, HEIGHT, &settingsmenu, ROW_HEIGHT, 3);
    fezui_animated_listbox_get_cursor(&fezui, 0, 0, WIDTH, HEIGHT, &settingsmenu, ROW_HEIGHT, &g_target_cursor);
    fezui_draw_cursor(&fezui, &g_fezui_cursor);
}
static void settings_menu_cb(void *menu)
{

    switch (((fezui_list_base_t *)menu)->selected_index)
    {
    case 0:
        fezui_link_frame_navigate(&mainframe, &panelpage);
        break;
    case 1:
        fezui_link_frame_navigate(&mainframe, &keymappage);
        break;
    case 2:
        fezui_link_frame_navigate(&mainframe, &rgbconfigpage);
        break;
    case 3:
        fezui_link_frame_navigate(&mainframe, &calibrationpage);
        break;
    case 4:
        fezui_link_frame_navigate(&mainframe, &displayconfigpage);
        break;
    case 5:
        fezui_link_frame_navigate(&mainframe, &advancedsettingspage);
        break;
    case 6:
        fezui_link_frame_navigate(&mainframe, &aboutpage);
        break;
    default:
        break;
    }
}
static void settingspage_load(void *page)
{
    fezui_animated_listbox_begin(&settingsmenu);
}
static void settingspage_event_handler(void *e)
{
    switch (*(uint16_t *)e)
    {
    case KEY_UP_ARROW:
        fezui_animated_listbox_index_increase(&settingsmenu, -1);
        break;
    case KEY_DOWN_ARROW:
        fezui_animated_listbox_index_increase(&settingsmenu, 1);
        break;
    case KEY_ENTER:
        fezui_animated_listbox_click(&settingsmenu);
        break;
    case KEY_ESC:
        fezui_link_frame_go_back(&mainframe);
        break;
    default:
        break;
    }
}

fezui_link_page_t settingspage = {settingspage_tick, settingspage_draw, settingspage_load, settingspage_event_handler};