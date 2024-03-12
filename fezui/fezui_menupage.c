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

static void menupage_logic(void *page)
{
    fezui_animated_listbox_update(&fezui,&mainmenu);
}
static void menupage_draw(void *page)
{
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_6x13_mr);
    fezui_draw_animated_listbox(&fezui,0,0,WIDTH,HEIGHT,&mainmenu,HEIGHT/4,3);
    fezui_animated_listbox_get_cursor(&fezui,0,0,WIDTH,HEIGHT,&mainmenu,HEIGHT/4,&target_cursor);
    fezui_draw_cursor(&fezui, &cursor);
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
    Keybaord_SendReport_Enable=false;
    fezui_animated_listbox_begin(&mainmenu);
    key_attach(&KEY_FN_K6, KEY_EVENT_DOWN, LAMBDA(void,(void*k){fezui_link_frame_go_back(&mainframe);}));
    key_attach(&KEY_FN_K5, KEY_EVENT_DOWN, LAMBDA(void,(void*k){fezui_animated_listbox_click(&mainmenu);}));
    key_attach(&KEY_KNOB, KEY_EVENT_DOWN, LAMBDA(void,(void*k){fezui_animated_listbox_click(&mainmenu);}));
    key_attach(&KEY_KNOB_CLOCKWISE, KEY_EVENT_DOWN, LAMBDA(void,(void*k){fezui_animated_listbox_index_increase(&mainmenu, 1);}));
    key_attach(&KEY_KNOB_ANTICLOCKWISE, KEY_EVENT_DOWN, LAMBDA(void,(void*k){fezui_animated_listbox_index_increase(&mainmenu, -1);}));
}

fezui_link_page_t menupage={menupage_logic,menupage_draw,menupage_load};
