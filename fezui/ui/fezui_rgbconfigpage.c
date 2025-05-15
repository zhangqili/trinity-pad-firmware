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
#define SPERATOR_X 32

static bool key_selected = false;
static bool configing = false;

static fezui_list_base_t rgb_key_select_menu;
static const char *rgb_key_select_menu_items[] = {"Global", "KEY1", "KEY2", "KEY3", "KEY4"};

static fezui_list_base_t rgb_config_menu;
static const char *rgb_config_menu_items[] = {"Mode", "R", "G", "B", "H", "S", "V", "Speed"};

static const char *rgb_mode_items[] = {"Fixed", "Static", "Cycle", "Linear", "Trigger", "String", "Fading String", "Diamond Ripple", "Fading Diamond Ripple", "Jelly"};
static const char *rgb_global_mode_items[] = {"OFF", "Blank","Rainbow","Wave"};

static ColorRGB *target_rgb;
static ColorHSV *target_hsv;

static fezui_cursor_t config_cursor;
static fezui_cursor_t target_config_cursor;
static fezui_list_base_t rgb_global_config_menu;
static const char *rgb_global_config_menu_items[] = {"Mode"};

static void set_target_color()
{

    if (rgb_key_select_menu.selected_index)
    {
        target_rgb = &(g_rgb_configs[rgb_key_select_menu.selected_index - 1].rgb);
        target_hsv = &(g_rgb_configs[rgb_key_select_menu.selected_index - 1].hsv);
    }
    else
    {
    }
}

static void rgb_key_select_menu_cb(void *m)
{
    key_selected = true;
    if (!((fezui_list_base_t*)m)->selected_index)
    {
        rgb_config_menu.selected_index = 0;
    }
    
}

static void rgb_config_menu_cb(void *m)
{
    configing = true;
}

void rgbconfigpage_init()
{
    fezui_list_base_init(&rgb_key_select_menu, (void **)rgb_key_select_menu_items, sizeof(rgb_key_select_menu_items) / sizeof(const char *), rgb_key_select_menu_cb);
    fezui_list_base_init(&rgb_config_menu, (void **)rgb_config_menu_items, sizeof(rgb_config_menu_items) / sizeof(const char *), rgb_config_menu_cb);
    fezui_list_base_init(&rgb_global_config_menu, (void **)rgb_global_config_menu_items, sizeof(rgb_global_config_menu_items) / sizeof(const char *), NULL);
}

static void rgbconfigpage_tick(void *page)
{

    fezui_cursor_set(
        &g_target_cursor,
        0,
        rgb_key_select_menu.selected_index * ROW_HEIGHT,
        SPERATOR_X,
        ROW_HEIGHT);
    if (key_selected)
    {
        if (configing)
        {
            if (rgb_config_menu.selected_index)
            {
                fezui_cursor_set(
                    &target_config_cursor,
                    WIDTH - 16,
                    rgb_config_menu.selected_index * ROW_HEIGHT,
                    16,
                    ROW_HEIGHT);
            }
            else
            {
                if (rgb_key_select_menu.selected_index)
                {
                    fezui_cursor_set(
                        &target_config_cursor,
                        WIDTH - 1 - strlen(rgb_mode_items[g_rgb_configs[rgb_key_select_menu.selected_index - 1].mode]) * 5,
                        rgb_config_menu.selected_index * ROW_HEIGHT,
                        1 + strlen(rgb_mode_items[g_rgb_configs[rgb_key_select_menu.selected_index - 1].mode]) * 5,
                        ROW_HEIGHT);
                }
                else
                {
                    fezui_cursor_set(
                        &target_config_cursor,
                        WIDTH - 1 - strlen(rgb_global_mode_items[g_rgb_base_config.mode]) * 5,
                        rgb_config_menu.selected_index * ROW_HEIGHT,
                        1 + strlen(rgb_global_mode_items[g_rgb_base_config.mode]) * 5,
                        ROW_HEIGHT);
                }
            }
        }
        else
        {
            fezui_cursor_set(
                &target_config_cursor,
                SPERATOR_X,
                rgb_config_menu.selected_index * ROW_HEIGHT,
                WIDTH - SPERATOR_X,
                ROW_HEIGHT);
        }
    }
    else
    {
        fezui_cursor_set(
            &target_config_cursor,
            SPERATOR_X,
            rgb_config_menu.selected_index * ROW_HEIGHT,
            1,
            ROW_HEIGHT);
    }

    fezui_cursor_move(&fezui, &config_cursor, &target_config_cursor);
}

static void rgbconfigpage_draw(void *page)
{

    u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x8_mr);
    for (uint8_t i = 0; i < rgb_key_select_menu.len; i++)
    {
        u8g2_DrawUTF8(&(fezui.u8g2), 1, ROW_HEIGHT * (i + 1), rgb_key_select_menu.items[i]);
    }
    uint8_t font_width = u8g2_GetMaxCharWidth(&(fezui.u8g2));
    u8g2_DrawUTF8(&(fezui.u8g2), SPERATOR_X + 2, ROW_HEIGHT * 1, rgb_config_menu.items[0]);
    if (rgb_key_select_menu.selected_index)
    {
        for (uint8_t i = 1; i < rgb_config_menu.len; i++)
        {
            u8g2_DrawUTF8(&(fezui.u8g2), SPERATOR_X + 2, ROW_HEIGHT * (i + 1), rgb_config_menu.items[i]);
        }
        u8g2_DrawUTF8(&(fezui.u8g2), WIDTH - strlen(rgb_mode_items[g_rgb_configs[rgb_key_select_menu.selected_index - 1].mode]) * font_width, ROW_HEIGHT * 1 - 1, rgb_mode_items[g_rgb_configs[rgb_key_select_menu.selected_index - 1].mode]);

        fezui_printf_right_aligned(&fezui, WIDTH, ROW_HEIGHT * 2, "%d", g_rgb_configs[rgb_key_select_menu.selected_index - 1].rgb.r);

        fezui_printf_right_aligned(&fezui, WIDTH, ROW_HEIGHT * 3, "%d", g_rgb_configs[rgb_key_select_menu.selected_index - 1].rgb.g);

        fezui_printf_right_aligned(&fezui, WIDTH, ROW_HEIGHT * 4, "%d", g_rgb_configs[rgb_key_select_menu.selected_index - 1].rgb.b);

        fezui_printf_right_aligned(&fezui, WIDTH, ROW_HEIGHT * 8, "%0.0f", g_rgb_configs[rgb_key_select_menu.selected_index - 1].speed * 1000);

        //fezui_printf_right_aligned(&fezui, WIDTH, ROW_HEIGHT * 2, "%d", target_rgb->r);

        //fezui_printf_right_aligned(&fezui, WIDTH, ROW_HEIGHT * 3, "%d", target_rgb->g);

        //fezui_printf_right_aligned(&fezui, WIDTH, ROW_HEIGHT * 4, "%d", target_rgb->b);

        fezui_printf_right_aligned(&fezui, WIDTH, ROW_HEIGHT * 5, "%d", target_hsv->h);

        fezui_printf_right_aligned(&fezui, WIDTH, ROW_HEIGHT * 6, "%d", target_hsv->s);

        fezui_printf_right_aligned(&fezui, WIDTH, ROW_HEIGHT * 7, "%d", target_hsv->v);
    }
    else
    {
        fezui_printf_right_aligned(&fezui, WIDTH, ROW_HEIGHT * 1, rgb_global_mode_items[g_rgb_base_config.mode]);
    }

    u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x8_mr);
    // u8g2_DrawUTF8(&(fezui.u8g2), SPERATOR_X + 2, 8 - 1, current_key_name);

    u8g2_DrawBox(&(fezui.u8g2), 0, ROW_HEIGHT * 6, SPERATOR_X, ROW_HEIGHT * 2);
    uint8_t color = u8g2_GetDrawColor(&(fezui.u8g2));
    u8g2_SetDrawColor(&(fezui.u8g2), 2);
    u8g2_DrawUTF8(&(fezui.u8g2), +1, ROW_HEIGHT * 7, "#");
    if (rgb_key_select_menu.selected_index)
    {
        fezui_printf(&fezui, 1, ROW_HEIGHT * 8, "%02X%02X%02X", g_rgb_configs[rgb_key_select_menu.selected_index - 1].rgb.r, g_rgb_configs[rgb_key_select_menu.selected_index - 1].rgb.g, g_rgb_configs[rgb_key_select_menu.selected_index - 1].rgb.b);
    }
    u8g2_SetDrawColor(&(fezui.u8g2), color);

    fezui_draw_cursor(&fezui, &g_fezui_cursor);
    fezui_draw_cursor(&fezui, &config_cursor);

    u8g2_DrawVLine(&(fezui.u8g2), SPERATOR_X, 0, 64);
}

static void rgbconfigpage_load(void *page)
{
    fezui_cursor_set(
        &config_cursor,
        SPERATOR_X,
        0,
        1,
        HEIGHT);
    set_target_color();
}

static void rgbconfigpage_event_handler(void *e)
{
    switch (*(uint16_t *)e)
    {
    case KEY_UP_ARROW:
        if (key_selected)
        {
            if (configing)
            {
                switch (rgb_config_menu.selected_index)
                {
                case 0:
                    if (rgb_key_select_menu.selected_index)
                    {
                        VAR_LOOP_INCREMENT(g_rgb_configs[rgb_key_select_menu.selected_index - 1].mode, RGB_MODE_FIXED, RGB_MODE_JELLY, 1)
                    }
                    else
                    {
                        VAR_LOOP_INCREMENT(g_rgb_base_config.mode, RGB_BASE_MODE_OFF, RGB_BASE_MODE_WAVE, 1)
                    }
                    break;
                case 1:
                    VAR_LOOP_INCREMENT(g_rgb_configs[rgb_key_select_menu.selected_index - 1].rgb.r, 0, 255, 1)
                    rgb_to_hsv(target_hsv, target_rgb);
                    break;
                case 2:
                    VAR_LOOP_INCREMENT(g_rgb_configs[rgb_key_select_menu.selected_index - 1].rgb.g, 0, 255, 1)
                    rgb_to_hsv(target_hsv, target_rgb);
                    break;
                case 3:
                    VAR_LOOP_INCREMENT(g_rgb_configs[rgb_key_select_menu.selected_index - 1].rgb.b, 0, 255, 1)
                    rgb_to_hsv(target_hsv, target_rgb);
                    break;
                case 4:
                    VAR_LOOP_INCREMENT(target_hsv->h, 0, 360, 1)
                    hsv_to_rgb(target_rgb, target_hsv);
                    break;
                case 5:
                    VAR_LOOP_INCREMENT(target_hsv->s, 0, 100, 1)
                    hsv_to_rgb(target_rgb, target_hsv);
                    break;
                case 6:
                    VAR_LOOP_INCREMENT(target_hsv->v, 0, 100, 1)
                    hsv_to_rgb(target_rgb, target_hsv);
                    break;
                case 7:
                    if (rgb_key_select_menu.selected_index)
                    {
                        g_rgb_configs[rgb_key_select_menu.selected_index - 1].speed += 0.001;
                    }
                    else
                    {
                    }
                    break;
                default:
                    break;
                }
            }
            else
            {
                if (rgb_key_select_menu.selected_index)
                {
                    fezui_list_base_index_increase(&rgb_config_menu, 1);
                }
            }
        }
        else
        {
            fezui_list_base_index_increase(&rgb_key_select_menu, 1);
            set_target_color();
        }
        break;
    case KEY_DOWN_ARROW:
        if (key_selected)
        {
            if (configing)
            {
                switch (rgb_config_menu.selected_index)
                {
                case 0:
                    if (rgb_key_select_menu.selected_index)
                    {
                        VAR_LOOP_DECREMENT(g_rgb_configs[rgb_key_select_menu.selected_index - 1].mode, RGB_MODE_FIXED, RGB_MODE_JELLY, 1)
                    }
                    else
                    {
                        VAR_LOOP_DECREMENT(g_rgb_base_config.mode, RGB_BASE_MODE_OFF, RGB_BASE_MODE_WAVE, 1)
                    }
                    break;
                case 1:
                    VAR_LOOP_DECREMENT(g_rgb_configs[rgb_key_select_menu.selected_index - 1].rgb.r, 0, 255, 1)
                    rgb_to_hsv(target_hsv, target_rgb);
                    break;
                case 2:
                    VAR_LOOP_DECREMENT(g_rgb_configs[rgb_key_select_menu.selected_index - 1].rgb.g, 0, 255, 1)
                    rgb_to_hsv(target_hsv, target_rgb);
                    break;
                case 3:
                    VAR_LOOP_DECREMENT(g_rgb_configs[rgb_key_select_menu.selected_index - 1].rgb.b, 0, 255, 1)
                    rgb_to_hsv(target_hsv, target_rgb);
                    break;
                case 4:
                    VAR_LOOP_DECREMENT(target_hsv->h, 0, 360, 1)
                    hsv_to_rgb(target_rgb, target_hsv);
                    break;
                case 5:
                    VAR_LOOP_DECREMENT(target_hsv->s, 0, 100, 1)
                    hsv_to_rgb(target_rgb, target_hsv);
                    break;
                case 6:
                    VAR_LOOP_DECREMENT(target_hsv->v, 0, 100, 1)
                    hsv_to_rgb(target_rgb, target_hsv);
                    break;
                case 7:
                    if (rgb_key_select_menu.selected_index)
                    {
                        g_rgb_configs[rgb_key_select_menu.selected_index - 1].speed -= 0.001;
                    }
                    else
                    {
                    }
                    break;
                default:
                    break;
                }
            }
            else
            {
                if (rgb_key_select_menu.selected_index)
                {
                    fezui_list_base_index_increase(&rgb_config_menu, -1);
                }
            }
        }
        else
        {
            fezui_list_base_index_increase(&rgb_key_select_menu, -1);
            set_target_color();
        }
        break;
    case KEY_SPACEBAR:
    case KEY_ENTER:
        if (key_selected)
        {
            configing = !configing;
        }
        else
        {
            fezui_list_base_click(&rgb_key_select_menu);
        }
        break;
    case KEY_ESC:
        if (configing)
        {
            configing = false;
        }
        else
        {
            if (key_selected)
            {
                key_selected = false;
                // rgb_save();
            }
            else
            {
                fezui_frame_go_back(&g_mainframe);
            }
        }
        break;
    default:
        break;
    }
}
fezui_page_t rgbconfigpage = {rgbconfigpage_tick, rgbconfigpage_draw, rgbconfigpage_load, rgbconfigpage_event_handler};
