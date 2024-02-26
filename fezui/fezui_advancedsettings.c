#include "fezui.h"
#include "fezui_var.h"
#include "main.h"
static fezui_animated_listbox_t advancedsettingsmenu;
static const char *advancedsettingsmenu_items[] = 
{
    "Debug",
    "Save", 
    "Reboot", 
    "Factory Reset", 
};
fezui_link_page_t advancedsettingspage = {advancedsettingspage_logic, advancedsettingspage_draw, advancedsettingspage_load};

#define ROW_HEIGHT 16

void advancedsettingspage_init()
{
    fezui_animated_listbox_init(&advancedsettingsmenu, advancedsettingsmenu_items, sizeof(advancedsettingsmenu_items) / sizeof(const char *), advancedsettings_menu_cb);
    advancedsettingsmenu.show_scrollbar = true;
}

void advancedsettingspage_logic(void *page)
{
}
void advancedsettingspage_draw(void *page)
{
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_6x13_mr);
    fezui_draw_animated_listbox(&fezui, 0, 0, WIDTH, HEIGHT, &advancedsettingsmenu, ROW_HEIGHT, 3);
    fezui_animated_listbox_get_cursor(&fezui, 0, 0, WIDTH, HEIGHT, &advancedsettingsmenu, ROW_HEIGHT, &target_cursor);
    fezui_draw_cursor(&fezui, &cursor);
}
void advancedsettings_menu_cb(void *menu)
{

    switch (((fezui_animated_listbox_t *)menu)->selected_index)
    {
    case 0:
        fezui_link_frame_navigate(&mainframe, &debugpage);
        break;
    case 1:
        keyboard_save();
        fezui_notification_begin(&fezui,&fezui_notification,"Configuration saved!",500,0.1);
        break;
    case 2:
        keyboard_system_reset();
        break;
    case 3:
        keyboard_factory_reset();
        break;
    default:
        break;
    }
}
void advancedsettingspage_load(void *page)
{
    fezui_animated_listbox_begin(&advancedsettingsmenu);
    key_attach(&KEY_FN_K5, KEY_EVENT_DOWN, LAMBDA(
                                                    void, (void *k) { fezui_animated_listbox_click(&advancedsettingsmenu); }));
    key_attach(&KEY_KNOB, KEY_EVENT_DOWN, LAMBDA(
                                                   void, (void *k) { fezui_animated_listbox_click(&advancedsettingsmenu); }));
    key_attach(&KEY_FN_K6, KEY_EVENT_DOWN, LAMBDA(
                                                    void, (void *k) { fezui_link_frame_go_back(&mainframe); }));
    key_attach(&KEY_KNOB_CLOCKWISE, KEY_EVENT_DOWN, LAMBDA(
                                                             void, (void *k) { fezui_animated_listbox_index_increase(&advancedsettingsmenu, 1); }));
    key_attach(&KEY_KNOB_ANTICLOCKWISE, KEY_EVENT_DOWN, LAMBDA(
                                                                 void, (void *k) { fezui_animated_listbox_index_increase(&advancedsettingsmenu, -1); }));
}
