/*
 * fezui_menupage.c
 *
 *  Created on: 2023骞�6鏈�19鏃�
 *      Author: xq123
 */

#include "fezui.h"
#include "keyboard.h"
#include "fezui_var.h"


fezui_animated_listbox_t mainmenu;
const char* mainmenu_items[] = {"Home","Oscilloscope","Statistic","Settings"};
static void main_menu_cb(void *menu);
void menupage_init()
{
    fezui_animated_listbox_init(&mainmenu, mainmenu_items, sizeof(mainmenu_items)/sizeof(const char*), main_menu_cb);
}

static void menupage_tick(void *page)
{
    fezui_animated_listbox_update(&fezui,&mainmenu);
}
static void menupage_draw(void *page)
{
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_6x13_mr);
    fezui_draw_animated_listbox(&fezui,0,0,WIDTH,HEIGHT,&mainmenu,HEIGHT/4,3);
    fezui_animated_listbox_get_cursor(&fezui,0,0,WIDTH,HEIGHT,&mainmenu,HEIGHT/4,&g_target_cursor);
    fezui_draw_cursor(&fezui, &g_fezui_cursor);
}

static void main_menu_cb(void *menu)
{
    switch (((fezui_list_base_t*)menu)->selected_index)
    {
    case 0:
    	fezui_link_frame_go_back(&mainframe);
        break;
    case 1:
        fezui_link_frame_navigate(&mainframe, &oscilloscopepage);
        break;
    case 2:
    	fezui_link_frame_navigate(&mainframe, &statisticpage);
        break;
    case 3:
        fezui_link_frame_navigate(&mainframe, &settingspage);
        break;
    default:
        break;
    }
}

static void menupage_load(void *page)
{
    g_keybaord_send_report_enable=false;
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
    case KEY_ENTER:
        fezui_animated_listbox_click(&mainmenu);
        break;
    case KEY_ESC:
        fezui_link_frame_go_back(&mainframe);
        break;
    default:
        break;
    }
}

fezui_link_page_t menupage={menupage_tick,menupage_draw,menupage_load,menupage_event_handler};
