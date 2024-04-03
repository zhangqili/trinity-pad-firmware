/*
 * fezui_advancedconfigpage.c
 *
 *  Created on: Aug 20, 2023
 *      Author: xq123
 */

#include "fezui.h"
#include "fezui_var.h"

#define ROW_HEIGHT 16
#define SPERATOR_X 80

#define MAINBAR_X (SPERATOR_X + 4)
#define MAINBAR_Y (ROW_HEIGHT + 4)
#define MAINBAR_W (18)
#define MAINBAR_H (40)

#define SUBBAR_X (SPERATOR_X + 24 + 4)
#define SUBBAR_Y (ROW_HEIGHT + 4)
#define SUBBAR_W (18)
#define SUBBAR_H (40)

static void keyconfig_digital_mode_menu_cb(void *m);
static void keyconfig_analog_normal_mode_menu_cb(void *m);
static void keyconfig_analog_rapid_mode_menu_cb(void *m);
static void keyconfig_analog_speed_mode_menu_cb(void *m);

static bool configing = false;
static bool mode_switching = false;
static float *target_property;
static float advancedconfigpage_interval;

static fezui_animated_listbox_t keyconfig_digital_mode_menu;
static const char *keyconfig_digital_mode_menu_items[] = {"Mode"};

static fezui_animated_listbox_t keyconfig_analog_normal_mode_menu;
static const char *keyconfig_analog_normal_mode_menu_items[] = {"Mode", "Upper bound", "Lower bound", "Trigger distance", "Schmitt parameter"};

static fezui_animated_listbox_t keyconfig_analog_rapid_mode_menu;
static const char *keyconfig_analog_rapid_mode_menu_items[] = {"Mode", "Upper bound", "Lower bound", "Trigger distance", "Release distance", "Upper deadzone", "Lower deadzone"};

static fezui_animated_listbox_t keyconfig_analog_speed_mode_menu;
static const char *keyconfig_analog_speed_mode_menu_items[] = {"Mode", "Upper bound", "Lower bound", "Trigger speed", "Release speed", "Upper deadzone", "Lower deadzone"};

static fezui_animated_listbox_t *current_menu = &keyconfig_digital_mode_menu;

AdvancedKey *current_config_advanced_key;

static fezui_progressbar_t mainbar = {.max = 4096, .min = 0, .orientation = ORIENTATION_VERTICAL};
static fezui_progressbar_t subbar = {.max = 1, .min = 0, .orientation = ORIENTATION_VERTICAL};

static void keyconfig_cursor_cb(fezui_t *fezui_ptr, fezui_cursor_t *cursor, void *item)
{
    if (configing)
    {
        cursor->x += cursor->w - 4*6;
        cursor->y += cursor->h / 2;
        cursor->w = 4*6;
        cursor->h /= 2;
    }
    else if (mode_switching)
    {
        cursor->x += cursor->w - 4*13;
        cursor->y += cursor->h / 2;
        cursor->w = 4*13;
        cursor->h /= 2;
    }
}

static void keyconfig_digital_mode_menu_cb(void *m)
{
    switch (((fezui_list_base_t *)m)->selected_index)
    {
    case 0:
        mode_switching = true;
        keyconfig_digital_mode_menu.listbox.list.selected_index = 0;
        keyconfig_analog_normal_mode_menu.listbox.list.selected_index = 0;
        keyconfig_analog_rapid_mode_menu.listbox.list.selected_index = 0;
        keyconfig_analog_speed_mode_menu.listbox.list.selected_index = 0;
        break;
    default:
        break;
    }
}

static void keyconfig_digital_mode_menu_draw_cb(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, void *item, uint16_t index)
{
    u8g2_DrawStr(&(fezui.u8g2), x, y + h / 2, item);
    const uint8_t *font_bk = fezui_ptr->u8g2.font;
    u8g2_SetFont(&(fezui_ptr->u8g2), u8g2_font_4x6_mr);
    switch (index)
    {
    case 0:
        fezui_printf_right_aligned(&fezui, x + w, y + h, "Digital");
        break;
    }
    u8g2_SetFont(&(fezui_ptr->u8g2), font_bk);
}


static void keyconfig_analog_normal_mode_menu_cb(void *m)
{
    switch (((fezui_list_base_t *)m)->selected_index)
    {
    case 0:
        mode_switching = true;
        keyconfig_digital_mode_menu.listbox.list.selected_index = 0;
        keyconfig_analog_normal_mode_menu.listbox.list.selected_index = 0;
        keyconfig_analog_rapid_mode_menu.listbox.list.selected_index = 0;
        keyconfig_analog_speed_mode_menu.listbox.list.selected_index = 0;
        break;
    case 1:
        configing = true;
        target_property = &(current_config_advanced_key->upper_bound);
        advancedconfigpage_interval = 0.1;
        break;
    case 2:
        configing = true;
        target_property = &(current_config_advanced_key->lower_bound);
        advancedconfigpage_interval = 0.1;
        break;
    case 3:
        configing = true;
        target_property = &(current_config_advanced_key->trigger_distance);
        advancedconfigpage_interval = 0.001;
        break;
    case 4:
        configing = true;
        target_property = &(current_config_advanced_key->schmitt_parameter);
        advancedconfigpage_interval = 0.001;
        break;
    default:
        break;
    }
}

static void keyconfig_analog_normal_mode_menu_draw_cb(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, void *item, uint16_t index)
{
    u8g2_DrawStr(&(fezui.u8g2), x, y + h / 2, item);
    const uint8_t *font_bk = fezui_ptr->u8g2.font;
    u8g2_SetFont(&(fezui_ptr->u8g2), u8g2_font_4x6_mr);
    switch (index)
    {
    case 0:
        fezui_printf_right_aligned(&fezui, x + w, y + h, "Analog Normal");
        break;
    case 1:
        fezui_printf_right_aligned(&fezui, x + w, y + h, "%6.1f", current_config_advanced_key->upper_bound);
        break;
    case 2:
        fezui_printf_right_aligned(&fezui, x + w, y + h, "%6.1f", current_config_advanced_key->lower_bound);
        break;
    case 3:
        fezui_printf_right_aligned(&fezui, x + w, y + h, "%6.1f%%", (current_config_advanced_key->trigger_distance) * 100);
        break;
    case 4:
        fezui_printf_right_aligned(&fezui, x + w, y + h, "%6.1f%%", (current_config_advanced_key->schmitt_parameter) * 100);
        break;
    }
    u8g2_SetFont(&(fezui_ptr->u8g2), font_bk);
}

static void keyconfig_analog_rapid_mode_menu_cb(void *m)
{

    switch (((fezui_list_base_t *)m)->selected_index)
    {
    case 0:
        mode_switching = true;
        keyconfig_digital_mode_menu.listbox.list.selected_index = 0;
        keyconfig_analog_normal_mode_menu.listbox.list.selected_index = 0;
        keyconfig_analog_rapid_mode_menu.listbox.list.selected_index = 0;
        keyconfig_analog_speed_mode_menu.listbox.list.selected_index = 0;
        break;
    case 1:
        configing = true;
        target_property = &(current_config_advanced_key->upper_bound);
        advancedconfigpage_interval = 0.1;
        break;
    case 2:
        configing = true;
        target_property = &(current_config_advanced_key->lower_bound);
        advancedconfigpage_interval = 0.1;
        break;
    case 3:
        configing = true;
        target_property = &(current_config_advanced_key->trigger_distance);
        advancedconfigpage_interval = 0.001;
        break;
    case 4:
        configing = true;
        target_property = &(current_config_advanced_key->release_distance);
        advancedconfigpage_interval = 0.001;
        break;
    case 5:
        configing = true;
        target_property = &(current_config_advanced_key->upper_deadzone);
        advancedconfigpage_interval = 0.001;
        break;
    case 6:
        configing = true;
        target_property = &(current_config_advanced_key->lower_deadzone);
        advancedconfigpage_interval = 0.001;
        break;
    default:
        break;
    }
}

static void keyconfig_analog_rapid_mode_menu_draw_cb(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, void *item, uint16_t index)
{
    u8g2_DrawStr(&(fezui_ptr->u8g2), x, y + h / 2, item);
    const uint8_t *font_bk = fezui_ptr->u8g2.font;
    u8g2_SetFont(&(fezui_ptr->u8g2), u8g2_font_4x6_mr);
    switch (index)
    {
    case 0:
        fezui_printf_right_aligned(fezui_ptr, x + w, y + h, "Analog Rapid");
        break;
    case 1:
        fezui_printf_right_aligned(fezui_ptr, x + w, y + h, "%6.1f", current_config_advanced_key->upper_bound);
        break;
    case 2:
        fezui_printf_right_aligned(fezui_ptr, x + w, y + h, "%6.1f", current_config_advanced_key->lower_bound);
        break;
    case 3:
        fezui_printf_right_aligned(fezui_ptr, x + w, y + h, "%6.1f%%", (current_config_advanced_key->trigger_distance) * 100);
        break;
    case 4:
        fezui_printf_right_aligned(fezui_ptr, x + w, y + h, "%6.1f%%", (current_config_advanced_key->release_distance) * 100);
        break;
    case 5:
        fezui_printf_right_aligned(fezui_ptr, x + w, y + h, "%6.1f%%", (current_config_advanced_key->upper_deadzone) * 100);
        break;
    case 6:
        fezui_printf_right_aligned(fezui_ptr, x + w, y + h, "%6.1f%%", (current_config_advanced_key->lower_deadzone) * 100);
        break;
    }
    u8g2_SetFont(&(fezui_ptr->u8g2), font_bk);
}

static void keyconfig_analog_speed_mode_menu_cb(void *m)
{

    switch (((fezui_list_base_t *)m)->selected_index)
    {
    case 0:
        mode_switching = true;
        keyconfig_digital_mode_menu.listbox.list.selected_index = 0;
        keyconfig_analog_normal_mode_menu.listbox.list.selected_index = 0;
        keyconfig_analog_rapid_mode_menu.listbox.list.selected_index = 0;
        keyconfig_analog_speed_mode_menu.listbox.list.selected_index = 0;
        break;
    case 1:
        configing = true;
        target_property = &(current_config_advanced_key->upper_bound);
        advancedconfigpage_interval = 0.1;
        break;
    case 2:
        configing = true;
        target_property = &(current_config_advanced_key->lower_bound);
        advancedconfigpage_interval = 0.1;
        break;
    case 3:
        configing = true;
        target_property = &(current_config_advanced_key->trigger_speed);
        advancedconfigpage_interval = 0.001;
        break;
    case 4:
        configing = true;
        target_property = &(current_config_advanced_key->release_speed);
        advancedconfigpage_interval = 0.001;
        break;
    case 5:
        configing = true;
        target_property = &(current_config_advanced_key->upper_deadzone);
        advancedconfigpage_interval = 0.001;
        break;
    case 6:
        configing = true;
        target_property = &(current_config_advanced_key->lower_deadzone);
        advancedconfigpage_interval = 0.001;
        break;
    default:
        break;
    }
}

static void keyconfig_analog_speed_mode_menu_draw_cb(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, void *item, uint16_t index)
{
    u8g2_DrawStr(&(fezui_ptr->u8g2), x, y + h / 2, item);
    const uint8_t *font_bk = fezui_ptr->u8g2.font;
    u8g2_SetFont(&(fezui_ptr->u8g2), u8g2_font_4x6_mr);
    switch (index)
    {
    case 0:
        fezui_printf_right_aligned(fezui_ptr, x + w, y + h, "Analog Speed");
        break;
    case 1:
        fezui_printf_right_aligned(fezui_ptr, x + w, y + h, "%6.1f", current_config_advanced_key->upper_bound);
        break;
    case 2:
        fezui_printf_right_aligned(fezui_ptr, x + w, y + h, "%6.1f", current_config_advanced_key->lower_bound);
        break;
    case 3:
        fezui_printf_right_aligned(fezui_ptr, x + w, y + h, "%6.1f%%", (current_config_advanced_key->trigger_speed) * 100);
        break;
    case 4:
        fezui_printf_right_aligned(fezui_ptr, x + w, y + h, "%6.1f%%", (current_config_advanced_key->release_speed) * 100);
        break;
    case 5:
        fezui_printf_right_aligned(fezui_ptr, x + w, y + h, "%6.1f%%", (current_config_advanced_key->upper_deadzone) * 100);
        break;
    case 6:
        fezui_printf_right_aligned(fezui_ptr, x + w, y + h, "%6.1f%%", (current_config_advanced_key->lower_deadzone) * 100);
        break;
    }
    u8g2_SetFont(&(fezui_ptr->u8g2), font_bk);
}

void advancedconfigpage_init()
{
    fezui_animated_listbox_init(&keyconfig_digital_mode_menu, (void **)keyconfig_digital_mode_menu_items, sizeof(keyconfig_digital_mode_menu_items) / sizeof(const char *), keyconfig_digital_mode_menu_cb, keyconfig_digital_mode_menu_draw_cb, keyconfig_cursor_cb);
    fezui_animated_listbox_init(&keyconfig_analog_normal_mode_menu, (void **)keyconfig_analog_normal_mode_menu_items, sizeof(keyconfig_analog_normal_mode_menu_items) / sizeof(const char *), keyconfig_analog_normal_mode_menu_cb, keyconfig_analog_normal_mode_menu_draw_cb, keyconfig_cursor_cb);
    fezui_animated_listbox_init(&keyconfig_analog_rapid_mode_menu, (void **)keyconfig_analog_rapid_mode_menu_items, sizeof(keyconfig_analog_rapid_mode_menu_items) / sizeof(const char *), keyconfig_analog_rapid_mode_menu_cb, keyconfig_analog_rapid_mode_menu_draw_cb, keyconfig_cursor_cb);
    fezui_animated_listbox_init(&keyconfig_analog_speed_mode_menu, (void **)keyconfig_analog_speed_mode_menu_items, sizeof(keyconfig_analog_speed_mode_menu_items) / sizeof(const char *), keyconfig_analog_speed_mode_menu_cb, keyconfig_analog_speed_mode_menu_draw_cb, keyconfig_cursor_cb);
}

static void advancedconfigpage_tick(void *page)
{
    switch (current_config_advanced_key->mode)
    {
    case KEY_DIGITAL_MODE:
        current_menu = &keyconfig_digital_mode_menu;
        break;
    case KEY_ANALOG_NORMAL_MODE:
        current_menu = &keyconfig_analog_normal_mode_menu;
        break;
    case KEY_ANALOG_RAPID_MODE:
        current_menu = &keyconfig_analog_rapid_mode_menu;
        break;
    case KEY_ANALOG_SPEED_MODE:
        current_menu = &keyconfig_analog_speed_mode_menu;
        break;
    default:
        break;
    }
}

static void advancedconfigpage_draw(void *page)
{
    uint8_t color = u8g2_GetDrawColor(&(fezui.u8g2));

    u8g2_DrawVLine(&(fezui.u8g2), SPERATOR_X, 0, 64);
    u8g2_DrawBox(&(fezui.u8g2), SPERATOR_X, 0, WIDTH - SPERATOR_X, ROW_HEIGHT);

    mainbar.value = current_config_advanced_key->raw;
    fezui_draw_progressbar(&fezui, MAINBAR_X, MAINBAR_Y, MAINBAR_W, MAINBAR_H, &mainbar);

    subbar.max = current_config_advanced_key->upper_bound;
    subbar.min = current_config_advanced_key->lower_bound;
    subbar.value = current_config_advanced_key->raw;
    fezui_draw_progressbar(&fezui, SUBBAR_X, SUBBAR_Y, SUBBAR_W, SUBBAR_H, &subbar);

    u8g2_DrawHLine(&(fezui.u8g2), MAINBAR_X, MAINBAR_Y + MAINBAR_H - ROUND(MAINBAR_H * (current_config_advanced_key->upper_bound) / (4096.0F)), MAINBAR_W);
    u8g2_DrawLine(&(fezui.u8g2), MAINBAR_X + MAINBAR_W, MAINBAR_Y + MAINBAR_H - ROUND(MAINBAR_H * (current_config_advanced_key->upper_bound) / (4096.0F)), SUBBAR_X, SUBBAR_Y);
    u8g2_DrawLine(&(fezui.u8g2), MAINBAR_X + MAINBAR_W, MAINBAR_Y + MAINBAR_H - ROUND(MAINBAR_H * (current_config_advanced_key->lower_bound) / (4096.0F)), SUBBAR_X, SUBBAR_Y + SUBBAR_H - 1);


    u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x8_mr);
    fezui_draw_animated_listbox(&fezui, 0, 0, SPERATOR_X, HEIGHT, current_menu, ROW_HEIGHT);
    fezui_animated_listbox_get_cursor(&fezui, 0, 0, SPERATOR_X, HEIGHT, current_menu, ROW_HEIGHT, &g_target_cursor);


    u8g2_SetDrawColor(&(fezui.u8g2), 2);
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x8_mr);
    fezui_printf(&fezui, SPERATOR_X + 2, 8 - 1, "KEY%d", current_config_advanced_key - g_keyboard_advanced_keys + 1);

    u8g2_SetFont(&(fezui.u8g2), u8g2_font_4x6_mr);
    fezui_printf(&fezui, SPERATOR_X + 2, ROW_HEIGHT - 1, current_config_advanced_key->key.state ? "TRUE" : "FALSE");
    fezui_printf(&fezui, WIDTH - 4 * 7, 8 - 1, "%6.1f%%", current_config_advanced_key->value * 100);

    fezui_printf(&fezui, WIDTH - 4 * 6, 16 - 1, "%6.1f", current_config_advanced_key->raw);
    u8g2_SetDrawColor(&(fezui.u8g2), color);
    fezui_draw_cursor(&fezui, &g_fezui_cursor);
}

static void mode_update(int8_t x)
{
    int8_t mode = current_config_advanced_key->mode;
    mode += x;
    if (mode > KEY_ANALOG_SPEED_MODE)
    {
        mode = KEY_DIGITAL_MODE;
    }
    if (mode < KEY_DIGITAL_MODE)
    {
        mode = KEY_ANALOG_SPEED_MODE;
    }
    current_config_advanced_key->mode = mode;
}

static void advancedconfigpage_event_handler(void *e)
{
    switch (*(uint16_t *)e)
    {
    case KEY_UP_ARROW:

        if (configing)
        {
            *target_property -= advancedconfigpage_interval;
        }
        else if (mode_switching)
        {
            mode_update(-1);
        }
        else
        {
            fezui_animated_listbox_index_increase(current_menu, -1);
        }
        break;
    case KEY_DOWN_ARROW:
        if (configing)
        {
            *target_property += advancedconfigpage_interval;
        }
        else if (mode_switching)
        {
            mode_update(1);
        }
        else
        {
            fezui_animated_listbox_index_increase(current_menu, 1);
        }
        break;
    case KEY_ENTER:
        if (configing)
        {
            configing = false;
        }
        else if (mode_switching)
        {
            mode_switching = false;
        }
        else
        {
            fezui_animated_listbox_click(current_menu);
        }
        break;
    case KEY_ESC:
        if (configing || mode_switching)
        {
            configing = false;
            mode_switching = false;
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
static void advancedconfigpage_load(void *page)
{
    switch (current_config_advanced_key->mode)
    {
    case KEY_DIGITAL_MODE:
        current_menu = &keyconfig_digital_mode_menu;
        break;
    case KEY_ANALOG_NORMAL_MODE:
        current_menu = &keyconfig_analog_normal_mode_menu;
        break;
    case KEY_ANALOG_RAPID_MODE:
        current_menu = &keyconfig_analog_rapid_mode_menu;
        break;
    case KEY_ANALOG_SPEED_MODE:
        current_menu = &keyconfig_analog_speed_mode_menu;
        break;
    default:
        break;
    }
    fezui_animated_listbox_begin(current_menu);
    // keyid_prase(current_config_advanced_key->key.id, binding_text, 256);

}

fezui_page_t advancedconfigpage = {advancedconfigpage_tick, advancedconfigpage_draw, advancedconfigpage_load, advancedconfigpage_event_handler};
