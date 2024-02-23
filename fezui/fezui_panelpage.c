#include "fezui.h"
#include "keyboard.h"
#include "fezui_var.h"

#define MARGIN_TILE 3

lefl_menu_t panelmenu;
const char *panelmenu_items[] = {"KEY1", "KEY2", "KEY3", "KEY4"};
static fezui_progressbar_t bars[4] = 
{
    {.max = 0, .min = 1, .orientation = ORIENTATION_VERTICAL},
    {.max = 0, .min = 1, .orientation = ORIENTATION_VERTICAL},
    {.max = 0, .min = 1, .orientation = ORIENTATION_VERTICAL},
    {.max = 0, .min = 1, .orientation = ORIENTATION_VERTICAL},
};

void panelmenu_cb(void *m)
{
    current_config_advanced_key = Keyboard_AdvancedKeys + ((lefl_menu_t *)m)->selected_index;
    fezui_link_frame_navigate(&mainframe, &advancedconfigpage);
}

void panelpage_init()
{
    lefl_menu_init(&panelmenu, panelmenu_items, sizeof(panelmenu_items) / sizeof(const char *), panelmenu_cb);
}

void panelpage_logic(void *page)
{
    fezui_cursor_set(
        &target_cursor,
        WIDTH / 4 * panelmenu.selected_index,
        0,
        // strlen(current_menu->items[current_menu->selected_index])*5+3,
        WIDTH / 4,
        HEIGHT);
}

void panelpage_draw(void *page)
{
    
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x8_mr);
    for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        u8g2_DrawStr(&fezui.u8g2,i*WIDTH/4+2,10,panelmenu.items[i]);
        bars[i].value = Keyboard_AdvancedKeys[i].value;
        fezui_draw_progressbar(&fezui, i*WIDTH/4+2, 20, WIDTH/4-2*2, 36, bars+i);
    }
    fezui_draw_cursor(&fezui, &cursor);
}


void panelpage_load(void *page)
{
    key_attach(&KEY_FN_K5, KEY_EVENT_DOWN, LAMBDA(
                                                    void, (void *k) { lefl_menu_click(&panelmenu); }));
    key_attach(&KEY_KNOB, KEY_EVENT_DOWN, LAMBDA(
                                                   void, (void *k) { lefl_menu_click(&panelmenu); }));
    key_attach(&KEY_FN_K6, KEY_EVENT_DOWN, LAMBDA(
                                                    void, (void *k) { fezui_link_frame_go_back(&mainframe); }));
    key_attach(&KEY_KNOB_CLOCKWISE, KEY_EVENT_DOWN, LAMBDA(
                                                             void, (void *k) { lefl_menu_index_increase(&panelmenu, 1); }));
    key_attach(&KEY_KNOB_ANTICLOCKWISE, KEY_EVENT_DOWN, LAMBDA(
                                                                 void, (void *k) { lefl_menu_index_increase(&panelmenu, -1); }));
}

fezui_link_page_t panelpage = {panelpage_logic, panelpage_draw, panelpage_load};
