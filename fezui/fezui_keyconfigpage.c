/*
 * fezui_keyconfigpage.c
 *
 *      Author: xq123
 */
#include "fezui.h"
#include "fezui_var.h"
#include "keyboard.h"

#define ROW_HEIGHT 16
#define SPERATOR_X 80

lefl_menu_t keyconfig_menu;

static const char* keyconfig_menu_items[] = {"Bindings"};
static char binding_text[128];
char current_key_name[32];

static fezui_scrolling_text_t scrolling_text;

void keyconfigpage_init()
{
    lefl_menu_init(&keyconfig_menu, keyconfig_menu_items, sizeof(keyconfig_menu_items)/sizeof(const char*), LAMBDA(void,(void*k)
    {
        fezui_link_frame_navigate(&mainframe, &keylistpage);
    }));
}

void keyconfigpage_logic(void *page)
{
    fezui_cursor_set(
            &target_cursor ,
            0,
            keyconfig_menu.selected_index*ROW_HEIGHT,
            SPERATOR_X,
            ROW_HEIGHT);
    fezui_scrolling_text_update(&scrolling_text);
}

void keyconfigpage_draw(void *page)
{
    uint8_t color = u8g2_GetDrawColor(&(fezui.u8g2));

    u8g2_DrawVLine(&(fezui.u8g2), SPERATOR_X, 0, 64);
    u8g2_DrawBox(&(fezui.u8g2), SPERATOR_X,0 ,WIDTH-SPERATOR_X, ROW_HEIGHT);
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x8_mr);
    u8g2_DrawStr(&(fezui.u8g2),1,ROW_HEIGHT*1-9,keyconfig_menu.items[0]);
    fezui_draw_scrolling_text(&fezui, 1, ROW_HEIGHT*1-2, &scrolling_text);

    u8g2_SetDrawColor(&(fezui.u8g2), 2);
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x8_mr);
    u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X+2, 8 - 1, current_key_name);


    u8g2_SetDrawColor(&(fezui.u8g2), color);
    fezui_draw_cursor(&fezui, &cursor);
}

void keyconfigpage_load(void *page)
{
    //keyid_prase(*current_target_id, binding_text, 128);
    fezui_scrolling_text_init(&fezui,&scrolling_text, 78, 0.2, u8g2_font_4x6_mr, binding_text);

    key_attach(&KEY_FN_K5, KEY_EVENT_DOWN, LAMBDA(void,(void*k)
    {
        lefl_menu_click(&keyconfig_menu);
    }));
    key_attach(&KEY_FN_K6, KEY_EVENT_DOWN, LAMBDA(void,(void*k)
    {
        //Keyboard_ID_Save();
        fezui_link_frame_go_back(&mainframe);
    }));
    key_attach(&KEY_KNOB, KEY_EVENT_DOWN, LAMBDA(void,(void*k)
    {
        //Keyboard_ID_Save();
        fezui_link_frame_go_back(&mainframe);
    }));
    key_attach(&KEY_KNOB_CLOCKWISE, KEY_EVENT_DOWN, NULL);
    key_attach(&KEY_KNOB_ANTICLOCKWISE, KEY_EVENT_DOWN, NULL);
}

fezui_link_page_t keyconfigpage={keyconfigpage_logic,keyconfigpage_draw,keyconfigpage_load};
/*
static lefl_animation_base_t keyconfiganimationx={
        .easing_func=lefl_animation_bounce_ease,
        .from=100,
        .to=0,
        .end=100,
        .tick=0,
        .parameter1=3.0,
        .parameter2=3.0,
        .mode=LEFL_ANIMATION_EASE_OUT,
        .target=&deltax,
};

static lefl_animation_base_t keyconfiganimationy={
        .easing_func=lefl_animation_elastic_ease,
        .end=100,
        .tick=0,
        .parameter1=3.0,
        .parameter2=3.0,
        .mode=LEFL_ANIMATION_EASE_OUT,
        .target=&deltay,
};
*/
/*
void draw_pad()
{
    u8g2_DrawRFrame(&(fezui.u8g2), (uint8_t)deltax+40, 6, 86, 54, 3);
    u8g2_DrawFrame(&(fezui.u8g2), (uint8_t)deltax+40+30, 9, 50, 22);
    u8g2_DrawFrame(&(fezui.u8g2), (uint8_t)deltax+40+73, 3, 6, 4);
    u8g2_DrawFrame(&(fezui.u8g2), (uint8_t)deltax+40+60, 3, 6, 4);
    if(keys[1].state)
        u8g2_DrawBox(&(fezui.u8g2), (uint8_t)deltax+40+73, 59, 6, 4);
    else
        u8g2_DrawFrame(&(fezui.u8g2), (uint8_t)deltax+40+73, 59, 6, 4);
    if(keys[0].state)
        u8g2_DrawBox(&(fezui.u8g2), (uint8_t)deltax+40+60, 59, 6, 4);
    else
        u8g2_DrawFrame(&(fezui.u8g2), (uint8_t)deltax+40+60, 59, 6, 4);
    if(Keyboard_AdvancedKeys[0].key.state)
        u8g2_DrawBox(&(fezui.u8g2), (uint8_t)deltax+44+20*0, 6+32, 19, 19);
    else
        u8g2_DrawFrame(&(fezui.u8g2), (uint8_t)deltax+44+20*0, 6+32, 19, 19);
    if(Keyboard_AdvancedKeys[1].key.state)
        u8g2_DrawBox(&(fezui.u8g2), (uint8_t)deltax+44+20*1, 6+32, 19, 19);
    else
        u8g2_DrawFrame(&(fezui.u8g2), (uint8_t)deltax+44+20*1, 6+32, 19, 19);
    if(Keyboard_AdvancedKeys[2].key.state)
        u8g2_DrawBox(&(fezui.u8g2), (uint8_t)deltax+44+20*2, 6+32, 19, 19);
    else
        u8g2_DrawFrame(&(fezui.u8g2), (uint8_t)deltax+44+20*2, 6+32, 19, 19);
    if(Keyboard_AdvancedKeys[3].key.state)
        u8g2_DrawBox(&(fezui.u8g2), (uint8_t)deltax+44+20*3, 6+32, 19, 19);
    else
        u8g2_DrawFrame(&(fezui.u8g2), (uint8_t)deltax+44+20*3, 6+32, 19, 19);
    u8g2_DrawCircle(&(fezui.u8g2), (uint8_t)deltax+44, 32, 7, U8G2_DRAW_ALL);
    u8g2_DrawCircle(&(fezui.u8g2), (uint8_t)deltax+55, 20, 7, U8G2_DRAW_ALL);
}
*/
