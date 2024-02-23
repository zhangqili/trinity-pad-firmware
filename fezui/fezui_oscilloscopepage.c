/*
 * fezui_oscilloscopepage.c
 *
 *  Created on: 2023骞�6鏈�19鏃�
 *      Author: xq123
 */
#include "fezui.h"
#include "fezui_var.h"

fezui_link_page_t oscilloscopepage={oscilloscopepage_logic,oscilloscopepage_draw,oscilloscopepage_load};

static float offset_x=0;
static float target_offset_x=0;

void oscilloscopepage_logic(void *page)
{
    lefl_loop_array_push_back(analog_historys+0, (uint16_t)Keyboard_AdvancedKeys[0].raw);
    lefl_loop_array_push_back(analog_historys+1, (uint16_t)Keyboard_AdvancedKeys[1].raw);
    lefl_loop_array_push_back(analog_historys+2, (uint16_t)Keyboard_AdvancedKeys[2].raw);
    lefl_loop_array_push_back(analog_historys+3, (uint16_t)Keyboard_AdvancedKeys[3].raw);
    CONVERGE_TO_ROUNDED(offset_x, target_offset_x,fezui.speed);
}
void oscilloscopepage_draw(void *page)
{
    fezui_draw_wave(&(fezui), 0-(u8g2_int_t)offset_x, 0, 64, 32, analog_historys+0, lines+0);
    fezui_draw_wave(&(fezui), 64-(u8g2_int_t)offset_x, 0, 64, 32, analog_historys+1, lines+1);
    fezui_draw_wave(&(fezui), 0-(u8g2_int_t)offset_x, 32, 64, 32, analog_historys+2, lines+2);
    fezui_draw_wave(&(fezui), 64-(u8g2_int_t)offset_x, 32, 64, 32, analog_historys+3, lines+3);
    fezui_draw_detailed_wave(&fezui, 128*1-(u8g2_int_t)offset_x, 0, Keyboard_AdvancedKeys+0, analog_historys+0, lines+0);
    fezui_draw_detailed_wave(&fezui, 128*2-(u8g2_int_t)offset_x, 0, Keyboard_AdvancedKeys+1, analog_historys+1, lines+1);
    fezui_draw_detailed_wave(&fezui, 128*3-(u8g2_int_t)offset_x, 0, Keyboard_AdvancedKeys+2, analog_historys+2, lines+2);
    fezui_draw_detailed_wave(&fezui, 128*4-(u8g2_int_t)offset_x, 0, Keyboard_AdvancedKeys+3, analog_historys+3, lines+3);

}

void oscilloscopepage_load(void *page)
{
    target_offset_x=0;
    //target_offset_x+=128;
    //target_offset_x+=128;
    key_attach(&KEY_FN_K6, KEY_EVENT_DOWN, LAMBDA(void,(void*k){fezui_link_frame_go_back(&mainframe);fezui_cursor_set(&cursor ,0 ,0 ,WIDTH ,HEIGHT);}));
    key_attach(&KEY_FN_K5, KEY_EVENT_DOWN, LAMBDA(void,(void*k){fezui_link_frame_go_back(&mainframe);fezui_cursor_set(&cursor ,0 ,0 ,WIDTH ,HEIGHT);}));

    key_attach(&KEY_KNOB, KEY_EVENT_DOWN, LAMBDA(void,(void*k){fezui_link_frame_go_back(&mainframe);fezui_cursor_set(&cursor ,0 ,0 ,WIDTH ,HEIGHT);}));
    key_attach(&KEY_KNOB_CLOCKWISE, KEY_EVENT_DOWN, LAMBDA(void,(void*k)
            {
                target_offset_x+=128;
                if(target_offset_x>128*4)
                {
                    target_offset_x=0;
                }
            }));
    key_attach(&KEY_KNOB_ANTICLOCKWISE, KEY_EVENT_DOWN, LAMBDA(void,(void*k)
            {
                target_offset_x-=128;
                if(target_offset_x<0)
                {
                    target_offset_x=128*4;
                }
            }));
}
