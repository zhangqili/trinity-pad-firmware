/*
 * fezui_keylistpage.c
 *
 *  Created on: Aug 20, 2023
 *      Author: xq123
 */


/*
 * fezui_keyselectpage.c
 *
 *  Created on: 2023骞�6鏈�19鏃�
 *      Author: xq123
 */
#include "fezui.h"
#include "fezui_var.h"
#include "keyboard.h"

#define ROW_HEIGHT 8

static fezui_animated_listbox_t keylist;
uint16_t* current_target_id;
uint16_t* current_target_key_binding;
#define KEY_MODIFIER_PART *(((uint8_t*)current_target_key_binding)+1)
#define KEY_KEYCODE_PART *(((uint8_t*)current_target_key_binding))

static void keylist_cb(void *m)
{
    if(((fezui_list_base_t*)m)->selected_index<8)
    {
        if (KEY_KEYCODE_PART == MOUSE_COLLECTION)
        {
            return;
        }
        bool b = !(KEY_MODIFIER_PART&BIT(((fezui_list_base_t*)m)->selected_index));
        KEY_MODIFIER_PART&=(~(1<<((fezui_list_base_t*)m)->selected_index));
        KEY_MODIFIER_PART|=(b<<((fezui_list_base_t*)m)->selected_index);
    }
    else
    {
        uint16_t keycode = ((fezui_list_base_t*)m)->selected_index - 8;
        if (keycode >= MOUSE_COLLECTION && keycode <= (MOUSE_COLLECTION + 6))
        {
            KEY_MODIFIER_PART = keycode - MOUSE_COLLECTION;
            KEY_KEYCODE_PART = MOUSE_COLLECTION;
        }
        else
        {
            KEY_KEYCODE_PART = keycode;
        }
    }
}

void keylistpage_init()
{
    fezui_animated_string_listbox_init(&keylist, g_hid_usage_names, sizeof(g_hid_usage_names)/sizeof(const char*), keylist_cb);
    keylist.listbox.show_scrollbar=true;
}

static void keylistpage_tick(void *page)
{
}
static void keylistpage_draw(void *page)
{
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x8_mr);
    fezui_draw_animated_listbox(&fezui,0,0,WIDTH,HEIGHT,&keylist,ROW_HEIGHT);
    fezui_animated_listbox_get_cursor(&fezui,0,0,WIDTH,HEIGHT,&keylist,ROW_HEIGHT,&g_target_cursor);
    for(uint8_t i=0;i<keylist.listbox.list.len;i++)
    {
        u8g2_DrawFrame(&(fezui.u8g2),116, (u8g2_int_t)floorf((ROW_HEIGHT * (i) -  FEZUI_ANIMATION_GET_VALUE(&keylist.scroll_animation, keylist.listbox.offset,keylist.targetoffset) + 1) * keylist.start_animation.value + 0.5),6,6);
    }
    switch (KEY_KEYCODE_PART)
    {
    case MOUSE_COLLECTION:
        u8g2_DrawBox(&(fezui.u8g2),116,(u8g2_int_t)floorf((ROW_HEIGHT*((KEY_MODIFIER_PART+MOUSE_COLLECTION)+8) + 1 -  FEZUI_ANIMATION_GET_VALUE(&keylist.scroll_animation, keylist.listbox.offset,keylist.targetoffset)) * keylist.start_animation.value + 0.5),6,6);
        break;
    default:
        for(uint8_t i=0;i<8;i++)
        {
            if(KEY_MODIFIER_PART&BIT(i))
            {
                u8g2_DrawBox(&(fezui.u8g2),116,(u8g2_int_t)floorf((ROW_HEIGHT*i + 1 - FEZUI_ANIMATION_GET_VALUE(&keylist.scroll_animation, keylist.listbox.offset,keylist.targetoffset)) * keylist.start_animation.value + 0.5),6,6);
            }
        }
        u8g2_DrawBox(&(fezui.u8g2),116,(u8g2_int_t)floorf((ROW_HEIGHT*((KEY_KEYCODE_PART)+8) + 1 -  FEZUI_ANIMATION_GET_VALUE(&keylist.scroll_animation, keylist.listbox.offset,keylist.targetoffset)) * keylist.start_animation.value + 0.5),6,6);
        break;
    }
    fezui_draw_cursor(&fezui, &g_fezui_cursor);
}

static void keylistpage_load(void *page)
{
    //lefl_bit_array_init(&head_key_usage, (size_t*)&(current_target_key_binding->modifier), 8);
    fezui_animated_listbox_begin(&keylist);
}
static void keylistpage_event_handler(void *e)
{
    switch (*(uint16_t *)e)
    {
    case KEY_UP_ARROW:
        fezui_animated_listbox_index_increase(&keylist, -1);
        break;
    case KEY_DOWN_ARROW:
        fezui_animated_listbox_index_increase(&keylist, 1);
        break;
    case KEY_SPACEBAR:
    case KEY_ENTER:
        fezui_animated_listbox_click(&keylist);
        break;
    case KEY_ESC:
        fezui_frame_go_back(&g_mainframe);
        break;
    default:
        break;
    }
}

fezui_page_t keylistpage = {keylistpage_tick, keylistpage_draw, keylistpage_load, keylistpage_event_handler};