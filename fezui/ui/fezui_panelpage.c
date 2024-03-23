#include "fezui.h"
#include "keyboard.h"
#include "fezui_var.h"

#define MARGIN_TILE 3

fezui_list_base_t panelmenu;
const char *panelmenu_items[] = {"KEY1", "KEY2", "KEY3", "KEY4"};
static fezui_progressbar_t bars[4] =
    {
        {.max = 0, .min = 1, .orientation = ORIENTATION_VERTICAL},
        {.max = 0, .min = 1, .orientation = ORIENTATION_VERTICAL},
        {.max = 0, .min = 1, .orientation = ORIENTATION_VERTICAL},
        {.max = 0, .min = 1, .orientation = ORIENTATION_VERTICAL},
};

static void panelmenu_cb(void *m)
{
    current_config_advanced_key = g_keyboard_advanced_keys + ((fezui_list_base_t *)m)->selected_index;
    fezui_link_frame_navigate(&mainframe, &advancedconfigpage);
}

void panelpage_init()
{
    fezui_list_base_init(&panelmenu, panelmenu_items, sizeof(panelmenu_items) / sizeof(const char *), panelmenu_cb);
}

static void panelpage_logic(void *page)
{
    fezui_cursor_set(
        &target_cursor,
        WIDTH / 4 * panelmenu.selected_index,
        0,
        // strlen(current_menu->items[current_menu->selected_index])*5+3,
        WIDTH / 4,
        HEIGHT);
}

static void panelpage_draw(void *page)
{

    u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x8_mr);
    for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        u8g2_DrawStr(&fezui.u8g2, i * WIDTH / 4 + 2, 10, panelmenu.items[i]);
        bars[i].value = g_keyboard_advanced_keys[i].value;
        fezui_draw_progressbar(&fezui, i * WIDTH / 4 + 2, 20, WIDTH / 4 - 2 * 2, 36, bars + i);
    }
    fezui_draw_cursor(&fezui, &cursor);
}

static void panelpage_load(void *page)
{
}

static void panelpage_event_handler(void *e)
{
    switch (*(uint16_t *)e)
    {
    case KEY_UP_ARROW:
        fezui_list_base_index_increase(&panelmenu, 1);
        break;
    case KEY_DOWN_ARROW:
        fezui_list_base_index_increase(&panelmenu, -1);
        break;
    case KEY_ENTER:
        fezui_list_base_click(&panelmenu);
        break;
    case KEY_ESC:
        fezui_link_frame_go_back(&mainframe);
        break;
    default:
        break;
    }
}

fezui_link_page_t panelpage = {panelpage_logic, panelpage_draw, panelpage_load, panelpage_event_handler};
