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

static fezui_scrollview_t scrollview = { .content_height = ROW_HEIGHT*sizeof(hid_usage_names)/sizeof(const char*), .content_width =
        128, .abscissa = 0, .ordinate = 0 };

fezui_animated_listbox_t keylist;
uint16_t* current_target_id;
KeyBinding* current_target_key_binding;

fezui_link_page_t keylistpage = {keylistpage_logic, keylistpage_draw, keylistpage_load};

static lefl_bit_array_t head_key_usage;

void keylist_cb(void *m)
{
    if(((fezui_animated_listbox_t*)m)->selected_index<8)
    {
        //lefl_bit_array_set(&head_key_usage, ((lefl_menu_t*)m)->selected_index, !lefl_bit_array_get(&head_key_usage, ((lefl_menu_t*)m)->selected_index));
        bool b = !(current_target_key_binding->modifier&BIT(((lefl_menu_t*)m)->selected_index));
        current_target_key_binding->modifier&=(~(1<<((lefl_menu_t*)m)->selected_index));
        current_target_key_binding->modifier|=(b<<((lefl_menu_t*)m)->selected_index);
    }
    else
    {
        current_target_key_binding->keycode = ((lefl_menu_t*)m)->selected_index-8;
    }
}

void keylistpage_init()
{
    fezui_animated_listbox_init(&keylist, hid_usage_names, sizeof(hid_usage_names)/sizeof(const char*), keylist_cb);
    keylist.show_scrollbar=true;
}

void keylistpage_logic(void *page)
{
    fezui_animated_listbox_update(&fezui,&keylist);
}
void keylistpage_draw(void *page)
{
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x8_mr);
    fezui_draw_animated_listbox(&fezui,0,0,WIDTH,HEIGHT,&keylist,ROW_HEIGHT,1);
    fezui_animated_listbox_get_cursor(&fezui,0,0,WIDTH,HEIGHT,&keylist,ROW_HEIGHT,&target_cursor);
    for(uint8_t i=0;i<keylist.len;i++)
    {
        u8g2_DrawFrame(&(fezui.u8g2),116, (u8g2_int_t)floorf((ROW_HEIGHT * (i) -  FEZUI_ANIMATION_GET_VALUE(&keylist.scroll_animation, keylist.offset,keylist.targetoffset) + 1) * keylist.start_animation.value + 0.5),6,6);
    }
    for(uint8_t i=0;i<8;i++)
    {
        if(current_target_key_binding->modifier&BIT(i))
        {
            u8g2_DrawBox(&(fezui.u8g2),116,(u8g2_int_t)floorf((ROW_HEIGHT*i + 1 - FEZUI_ANIMATION_GET_VALUE(&keylist.scroll_animation, keylist.offset,keylist.targetoffset)) * keylist.start_animation.value + 0.5),6,6);
        }
    }
    u8g2_DrawBox(&(fezui.u8g2),116,(u8g2_int_t)floorf((ROW_HEIGHT*((current_target_key_binding->keycode)+8) + 1 -  FEZUI_ANIMATION_GET_VALUE(&keylist.scroll_animation, keylist.offset,keylist.targetoffset)) * keylist.start_animation.value + 0.5),6,6);
    fezui_draw_cursor(&fezui, &cursor);
}

void keylistpage_load(void *page)
{
    //lefl_bit_array_init(&head_key_usage, (size_t*)&(current_target_key_binding->modifier), 8);
    fezui_animated_listbox_begin(&keylist);
    key_attach(&KEY_KNOB, KEY_EVENT_DOWN, LAMBDA(void,(void*k){fezui_animated_listbox_click(&keylist);}));
    key_attach(&KEY_FN_K5, KEY_EVENT_DOWN, LAMBDA(void,(void*k){fezui_animated_listbox_click(&keylist);}));
    key_attach(&KEY_FN_K6, KEY_EVENT_DOWN, LAMBDA(void,(void*k){fezui_link_frame_go_back(&mainframe);}));
    key_attach(&KEY_KNOB_CLOCKWISE, KEY_EVENT_DOWN, LAMBDA(void,(void*k){fezui_animated_listbox_index_increase(&keylist, 1);}));
    key_attach(&KEY_KNOB_ANTICLOCKWISE, KEY_EVENT_DOWN, LAMBDA(void,(void*k){fezui_animated_listbox_index_increase(&keylist, -1);}));
}
