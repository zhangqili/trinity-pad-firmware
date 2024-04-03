/*
 * fezui_rgbconfigpage.c
 *
 *  Created on: Aug 22, 2023
 *      Author: xq123
 */
#include "fezui.h"
#include "rgb.h"
#include "fezui_var.h"
// #include "communication.h"

#define ROW_HEIGHT 8
#define SUBROW_HEIGHT 16
#define SPERATOR_X 48

static fezui_scrollview_t scrollview = {.content_height = ROW_HEIGHT * (ADVANCED_KEY_NUM + KEY_NUM)};
static bool key_selected = false;
static float target_ordinate = 0;
static float start_animation = 0;

fezui_animated_listbox_t key_select_list;
static const char *key_select_list_items[] = {"KEY1", "KEY2", "KEY3", "KEY4", "FN_KEY1", "FN_KEY2", "FN_KEY3", "FN_KEY4", "FN_KEY5", "FN_KEY6", "KNOB", "KNOB_CW", "KNOB_CCW"};

fezui_list_base_t layer_select_menu;
static const char *layer_select_menu_items[] = {"Layer 1", "Layer 2", "Layer 3"};

static fezui_cursor_t config_cursor;
static fezui_cursor_t target_config_cursor;
static fezui_scrolling_text_t scrolling_text[LAYER_NUM];
static char binding_text[LAYER_NUM][128];
extern uint16_t *current_target_key_binding;
void layer_select_menu_cb(void *menu)
{
    current_target_key_binding = &g_keymap[layer_select_menu.selected_index][key_select_list.listbox.list.selected_index];
    fezui_frame_navigate(&g_mainframe, &keylistpage);
}

static void key_select_list_cb(void*e)
{
    key_selected = true;
}

void keymappage_init()
{
    fezui_list_base_init(&layer_select_menu, (void**)layer_select_menu_items, sizeof(layer_select_menu_items) / sizeof(const char *), layer_select_menu_cb);
    fezui_animated_string_listbox_init(&key_select_list, key_select_list_items, sizeof(key_select_list_items) / sizeof(const char *), key_select_list_cb);
    key_select_list.listbox.show_scrollbar = true;
}

static void keymappage_tick(void *page)
{
    if (key_selected)
    {
        fezui_cursor_set(
            &target_config_cursor,
            SPERATOR_X,
            layer_select_menu.selected_index * SUBROW_HEIGHT,
            WIDTH - SPERATOR_X,
            SUBROW_HEIGHT);
    }
    else
    {
        fezui_cursor_set(
            &target_config_cursor,
            SPERATOR_X,
            layer_select_menu.selected_index * SUBROW_HEIGHT,
            1,
            SUBROW_HEIGHT);
    }

    for (uint8_t i = 0; i < LAYER_NUM; i++)
    {
        fezui_scrolling_text_update(scrolling_text + i);
    }
    if ((key_select_list.listbox.list.selected_index + 1) * ROW_HEIGHT - target_ordinate > 64)
    {
        target_ordinate = (key_select_list.listbox.list.selected_index + 1) * ROW_HEIGHT - 64;
    }
    if ((key_select_list.listbox.list.selected_index) * ROW_HEIGHT < target_ordinate)
    {
        target_ordinate = (key_select_list.listbox.list.selected_index) * ROW_HEIGHT;
    }
    CONVERGE_TO_ROUNDED(scrollview.ordinate, target_ordinate, fezui.speed);
    CONVERGE_TO(start_animation, 1, fezui.speed);
    fezui_cursor_move(&fezui, &config_cursor, &target_config_cursor);
}

static void keymappage_draw(void *page)
{
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x8_mr);
    fezui_draw_animated_listbox(&fezui, 0, 0, SPERATOR_X, HEIGHT, &key_select_list, ROW_HEIGHT);
    fezui_animated_listbox_get_cursor(&fezui, 0, 0, SPERATOR_X, HEIGHT, &key_select_list, ROW_HEIGHT, &g_target_cursor);
    for (uint8_t i = 0; i < LAYER_NUM; i++)
    {
        u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X + 2, SUBROW_HEIGHT * (i + 1) - 8 - 1, layer_select_menu.items[i]);
        fezui_draw_scrolling_text(&fezui, SPERATOR_X + 2, SUBROW_HEIGHT * (i + 1) - 2, scrolling_text + i);
    }
    fezui_draw_cursor(&fezui, &g_fezui_cursor);
    fezui_draw_cursor(&fezui, &config_cursor);

    u8g2_DrawVLine(&(fezui.u8g2), SPERATOR_X, 0, 64);
}

static void show_binding_text()
{
    for (uint8_t i = 0; i < LAYER_NUM; i++)
    {
        keyid_prase(g_keymap[i][key_select_list.listbox.list.selected_index], binding_text[i], 128);
        fezui_scrolling_text_init(&fezui, scrolling_text + i, 78, 0.2, u8g2_font_4x6_mr, binding_text[i]);
        fezui_scrolling_text_begin(scrolling_text + i);
    }
}

static void keymappage_load(void *page)
{
    show_binding_text();
    start_animation = 0;
    fezui_animated_listbox_begin(&key_select_list);
    fezui_cursor_set(
        &config_cursor,
        SPERATOR_X,
        0,
        1,
        HEIGHT);
}
static void keymappage_event_handler(void *e)
{
    switch (*(uint16_t *)e)
    {
    case KEY_UP_ARROW:
        if (key_selected)
        {
            fezui_list_base_index_increase(&layer_select_menu, 1);
        }
        else
        {
            fezui_animated_listbox_index_increase(&key_select_list, 1);
            show_binding_text();
        }
        break;
    case KEY_DOWN_ARROW:
        if (key_selected)
        {
            fezui_list_base_index_increase(&layer_select_menu, -1);
        }
        else
        {
            fezui_animated_listbox_index_increase(&key_select_list, -1);
            show_binding_text();
        }
        break;
    case KEY_ENTER:
        if (key_selected)
        {
            fezui_list_base_click(&layer_select_menu);
        }
        else
        {
            fezui_animated_listbox_click(&key_select_list);
        }
        break;
    case KEY_ESC:
        if (key_selected)
        {
            key_selected = false;
        }
        else
        {
            fezui_frame_go_back(&g_mainframe);
        }
        break;
    default:
        break;
    }
}

fezui_page_t keymappage = {keymappage_tick, keymappage_draw, keymappage_load, keymappage_event_handler};
