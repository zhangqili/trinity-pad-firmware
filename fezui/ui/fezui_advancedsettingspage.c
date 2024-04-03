#include "fezui.h"
#include "fezui_var.h"
#include "main.h"
static fezui_animated_listbox_t advancedsettingsmenu;
static const char* advancedsettingsmenu_items[] =
{
    "Debug",
    "Save",
    "Reboot",
    "Factory Reset",
    "Enter Bootloader",
};

#define ROW_HEIGHT 16

static void advancedsettings_menu_cb(void* menu)
{
    switch (((fezui_list_base_t *)menu)->selected_index)
    {
        case 0:
            fezui_frame_navigate(&g_mainframe, &debugpage);
            break;
        case 1:
            keyboard_save();
            fezui_notification_begin(&fezui, &fezui_notification, "Configuration saved!", 500, 0.1);
            break;
        case 2:
            keyboard_system_reset();
            break;
        case 3:
            keyboard_factory_reset();
            break;
        case 4:
            JumpToBootloader();
            break;
        default:
            break;
    }
}

void advancedsettingspage_init()
{
    fezui_animated_string_listbox_init(&advancedsettingsmenu, advancedsettingsmenu_items,
                                sizeof(advancedsettingsmenu_items) / sizeof(const char *), advancedsettings_menu_cb);
    advancedsettingsmenu.listbox.show_scrollbar = true;
}

static void advancedsettingspage_tick(void* page)
{
}

static void advancedsettingspage_draw(void* page)
{
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_6x13_mr);
    fezui_draw_animated_listbox(&fezui, 0, 0, WIDTH, HEIGHT, &advancedsettingsmenu, ROW_HEIGHT);
    fezui_animated_listbox_get_cursor(&fezui, 0, 0, WIDTH, HEIGHT, &advancedsettingsmenu, ROW_HEIGHT, &g_target_cursor);
    fezui_draw_cursor(&fezui, &g_fezui_cursor);
}

static void advancedsettingspage_load(void* page)
{
    fezui_animated_listbox_begin(&advancedsettingsmenu);
}

static void advancedsettingspage_event_handler(void* e)
{
    switch (*(uint16_t *)e)
    {
        case KEY_UP_ARROW:
            fezui_animated_listbox_index_increase(&advancedsettingsmenu, -1);
            break;
        case KEY_DOWN_ARROW:
            fezui_animated_listbox_index_increase(&advancedsettingsmenu, 1);
            break;
        case KEY_ENTER:
            fezui_animated_listbox_click(&advancedsettingsmenu);
            break;
        case KEY_ESC:
            fezui_frame_go_back(&g_mainframe);
            break;
        default:
            break;
    }
}

fezui_page_t advancedsettingspage = {
    advancedsettingspage_tick, advancedsettingspage_draw, advancedsettingspage_load, advancedsettingspage_event_handler
};
