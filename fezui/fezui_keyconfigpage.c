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

fezui_list_base_t keyconfig_menu;

static const char* keyconfig_menu_items[] = {"Bindings"};
static char binding_text[128];
char current_key_name[32];

static fezui_scrolling_text_t scrolling_text;

void keyconfigpage_init()
{
    fezui_list_base_init(&keyconfig_menu, keyconfig_menu_items, sizeof(keyconfig_menu_items)/sizeof(const char*), LAMBDA(void,(void*k)
    {
        fezui_link_frame_navigate(&mainframe, &keylistpage);
    }));
}

static void keyconfigpage_logic(void *page)
{
    fezui_cursor_set(
            &target_cursor ,
            0,
            keyconfig_menu.selected_index*ROW_HEIGHT,
            SPERATOR_X,
            ROW_HEIGHT);
    fezui_scrolling_text_update(&scrolling_text);
}

static void keyconfigpage_draw(void *page)
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

static void keyconfigpage_load(void *page)
{
    //keyid_prase(*current_target_id, binding_text, 128);
    fezui_scrolling_text_init(&fezui,&scrolling_text, 78, 0.2, u8g2_font_4x6_mr, binding_text);
}

static void keyconfigpage_event_handler(void *e)
{
    //keyid_prase(*current_target_id, binding_text, 128);
    switch (*(uint16_t *)e)
    {
    case KEY_UP_ARROW:
        break;
    case KEY_DOWN_ARROW:
        break;
    case KEY_ENTER:
        fezui_list_base_click(&keyconfig_menu);
        break;
    case KEY_ESC:
        fezui_link_frame_go_back(&mainframe);
        break;
    default:
        break;
    }

}
fezui_link_page_t keyconfigpage={keyconfigpage_logic,keyconfigpage_draw,keyconfigpage_load,keyconfigpage_event_handler};
