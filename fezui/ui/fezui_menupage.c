/*
 * fezui_menupage.c
 *
 *  Created on: 2023骞�6鏈�19鏃�
 *      Author: xq123
 */

#include "fezui.h"
#include "keyboard.h"
#include "fezui_var.h"


static fezui_animated_listbox_t mainmenu;
static const char* mainmenu_items[4][LANG_NUM] = 
{
    {"Home","主页"},
    {"Oscilloscope","示波器"},
    {"Statistic","统计数据"},
    {"Settings","设置"}
};
static void main_menu_cb(void *menu);
void menupage_init()
{
    fezui_animated_listbox_init(&mainmenu, (void**)mainmenu_items, 4, main_menu_cb, i18n_item_draw, i18n_item_get_cursor);
}

typedef struct {
    float x;
    float y;
} Point;


static void menupage_tick(void *page)
{
}

static void menupage_draw(void *page)
{
    switch (fezui.lang)
    {
    case LANG_EN:
        u8g2_SetFont(&(fezui.u8g2), u8g2_font_6x13_mr);
        break;
    case LANG_ZH:
        u8g2_SetFont(&(fezui.u8g2), u8g2_font_wqy12_t_gb2312a);
        break;
    default:
        break;
    }
    //u8g2_DrawTriangle(&fezui.u8g2, points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);
    //u8g2_DrawTriangle(&fezui.u8g2, points[1].x, points[1].y, points[2].x, points[2].y, points[3].x, points[3].y);
    fezui_draw_animated_listbox(&fezui,0,0,WIDTH,HEIGHT,&mainmenu,HEIGHT/4);
    fezui_animated_listbox_get_cursor(&fezui,0,0,WIDTH,HEIGHT,&mainmenu,HEIGHT/4,&g_target_cursor);
    fezui_draw_cursor(&fezui, &g_fezui_cursor);
}

static void main_menu_cb(void *menu)
{
    switch (((fezui_animated_listbox_t*)menu)->listbox.list.selected_index)
    {
    case 0:
    	fezui_frame_go_back(&g_mainframe);
        break;
    case 1:
        fezui_frame_navigate(&g_mainframe, &oscilloscopepage);
        break;
    case 2:
    	fezui_frame_navigate(&g_mainframe, &statisticpage);
        break;
    case 3:
        fezui_frame_navigate(&g_mainframe, &settingspage);
        break;
    default:
        break;
    }
}

static void menupage_load(void *page)
{
#define LEFT 16
#define UP 16
    g_keyboard_send_report_enable=false;
    g_current_layer = 0;
    fezui_animated_listbox_begin(&mainmenu);
}

static void menupage_event_handler(void *e)
{
    switch (*(uint16_t *)e)
    {
    case KEY_UP_ARROW:
        fezui_animated_listbox_index_increase(&mainmenu, -1);
        break;
    case KEY_DOWN_ARROW:
        fezui_animated_listbox_index_increase(&mainmenu, 1);
        break;
    case KEY_SPACEBAR:
    case KEY_ENTER:
        fezui_animated_listbox_click(&mainmenu);
        break;
    case KEY_ESC:
        fezui_frame_go_back(&g_mainframe);
        break;
    default:
        break;
    }
}

fezui_page_t menupage={menupage_tick,menupage_draw,menupage_load,menupage_event_handler};
