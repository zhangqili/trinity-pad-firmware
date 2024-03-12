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

static void settingspage_logic(void *page)
{
    fezui_animated_listbox_update(&fezui, &settingsmenu);
}
static void settingspage_draw(void *page)
{
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_6x13_mr);
    fezui_draw_animated_listbox(&fezui, 0, 0, WIDTH, HEIGHT, &settingsmenu, ROW_HEIGHT, 3);
    fezui_animated_listbox_get_cursor(&fezui, 0, 0, WIDTH, HEIGHT, &settingsmenu, ROW_HEIGHT, &target_cursor);
    fezui_draw_cursor(&fezui, &cursor);
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
    key_attach(&KEY_FN_K5, KEY_EVENT_DOWN, LAMBDA(
                                                    void, (void *k) { fezui_animated_listbox_click(&settingsmenu); }));
    key_attach(&KEY_KNOB, KEY_EVENT_DOWN, LAMBDA(
                                                   void, (void *k) { fezui_animated_listbox_click(&settingsmenu); }));
    key_attach(&KEY_FN_K6, KEY_EVENT_DOWN, LAMBDA(
                                                    void, (void *k) { fezui_link_frame_go_back(&mainframe); }));
    key_attach(&KEY_KNOB_CLOCKWISE, KEY_EVENT_DOWN, LAMBDA(
                                                             void, (void *k) { fezui_animated_listbox_index_increase(&settingsmenu, 1); }));
    key_attach(&KEY_KNOB_ANTICLOCKWISE, KEY_EVENT_DOWN, LAMBDA(
                                                                 void, (void *k) { fezui_animated_listbox_index_increase(&settingsmenu, -1); }));
}

fezui_link_page_t settingspage = {settingspage_logic, settingspage_draw, settingspage_load};