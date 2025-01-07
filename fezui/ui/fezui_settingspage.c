/*
 * fezui_settingspage.c
 *
 *  Created on: 2023骞�6鏈�19鏃�
 *      Author: xq123
 */
#include "fezui.h"
#include "fezui_var.h"
#include "main.h"
static fezui_animated_listbox_t settingsmenu;
static const char *settingsmenu_items[7][LANG_NUM] = 
{
    {"Keys Configuration","按键设置"}, 
    {"Keymap","按键映射"}, 
    {"RGB","灯效"}, 
    {"Calibration","校准"}, 
    {"Display","显示"}, 
    {"Advanced Settings","高级设置"}, 
    {"About","关于"}
};

#define ROW_HEIGHT 16
static void settings_menu_cb(void *menu);
void settingspage_init()
{
    fezui_animated_listbox_init(&settingsmenu, (void**)settingsmenu_items, 7, settings_menu_cb, i18n_item_draw, i18n_item_get_cursor);
    settingsmenu.listbox.show_scrollbar = true;
}

static void settingspage_tick(void *page)
{
}
static void settingspage_draw(void *page)
{
    switch (fezui.lang)
    {
    case LANG_EN:
        u8g2_SetFont(&(fezui.u8g2), u8g2_font_6x13_mr);
        break;
    case LANG_ZH:
        u8g2_SetFont(&(fezui.u8g2), u8g2_font_wqy13_t_gb2312a);
        break;
    default:
        break;
    }
    fezui_draw_animated_listbox(&fezui, 0, 0, WIDTH, HEIGHT, &settingsmenu, ROW_HEIGHT);
    fezui_animated_listbox_get_cursor(&fezui, 0, 0, WIDTH, HEIGHT, &settingsmenu, ROW_HEIGHT, &g_target_cursor);
    fezui_draw_cursor(&fezui, &g_fezui_cursor);
}
static void settings_menu_cb(void *menu)
{

    switch (((fezui_list_base_t *)menu)->selected_index)
    {
    case 0:
        fezui_frame_navigate(&g_mainframe, &panelpage);
        break;
    case 1:
        fezui_frame_navigate(&g_mainframe, &keymappage);
        break;
    case 2:
        fezui_frame_navigate(&g_mainframe, &rgbconfigpage);
        break;
    case 3:
        fezui_frame_navigate(&g_mainframe, &calibrationpage);
        break;
    case 4:
        fezui_frame_navigate(&g_mainframe, &displayconfigpage);
        break;
    case 5:
        fezui_frame_navigate(&g_mainframe, &advancedsettingspage);
        break;
    case 6:
        fezui_frame_navigate(&g_mainframe, &aboutpage);
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
    case KEY_SPACEBAR:
    case KEY_ENTER:
        fezui_animated_listbox_click(&settingsmenu);
        break;
    case KEY_ESC:
        fezui_frame_go_back(&g_mainframe);
        break;
    default:
        break;
    }
}

fezui_page_t settingspage = {settingspage_tick, settingspage_draw, settingspage_load, settingspage_event_handler};