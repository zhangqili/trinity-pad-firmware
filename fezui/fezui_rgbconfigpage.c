/*
 * fezui_rgbconfigpage.c
 *
 *  Created on: Aug 22, 2023
 *      Author: xq123
 */
#include "fezui.h"
#include "rgb.h"
#include "fezui_var.h"
//#include "communication.h"

#define ROW_HEIGHT 8
#define SPERATOR_X 32

static bool key_selected = false;
static bool configing = false;

lefl_menu_t rgb_key_select_menu;
static const char* rgb_key_select_menu_items[] = {"Global", "KEY1", "KEY2", "KEY3", "KEY4"};

lefl_menu_t rgb_config_menu;
static const char* rgb_config_menu_items[] = {"Mode", "R", "G", "B", "H", "S", "V", "Speed"};

static const char* rgb_mode_items[]={"Static","Cycle","Linear","Trigger"};
static const char* rgb_global_mode_items[]={"Off", "Individual", "Wave", "String", "Fading String","Diamond Ripple","Fading Diamond Ripple","Jelly"};

static ColorRGB *target_rgb;
static ColorHSV *target_hsv;

static fezui_cursor_t config_cursor;
static fezui_cursor_t target_config_cursor;
lefl_menu_t rgb_global_config_menu;
static const char* rgb_global_config_menu_items[] = {"Mode", "R", "G", "B", "H", "S", "V", "Speed"};

void set_target_color()
{
    
    if(rgb_key_select_menu.selected_index)
    {
        target_rgb=&(RGB_Configs[rgb_key_select_menu.selected_index-1].rgb);
        target_hsv=&(RGB_Configs[rgb_key_select_menu.selected_index-1].hsv);
    }
    else
    {
        target_rgb=&(RGB_GlobalConfig.rgb);
        target_hsv=&(RGB_GlobalConfig.hsv);
    }
    
}

void rgb_key_select_menu_cb(void*m)
{
    key_selected = true;
}

void rgbconfigpage_init()
{
    lefl_menu_init(&rgb_key_select_menu, rgb_key_select_menu_items, sizeof(rgb_key_select_menu_items)/sizeof(const char*), rgb_key_select_menu_cb);
    lefl_menu_init(&rgb_config_menu, rgb_config_menu_items, sizeof(rgb_config_menu_items)/sizeof(const char*), LAMBDA(void,(void*k)
    {
        configing = true;
    }));
    lefl_menu_init(&rgb_global_config_menu, rgb_global_config_menu_items, sizeof(rgb_global_config_menu_items)/sizeof(const char*), NULL);
}

void rgbconfigpage_logic(void *page)
{
    
    fezui_cursor_set(
            &target_cursor,
            0,
            rgb_key_select_menu.selected_index*ROW_HEIGHT,
            SPERATOR_X,
            ROW_HEIGHT);
    if(key_selected)
    {
        if(configing)
        {
            if(rgb_config_menu.selected_index)
            {
                fezui_cursor_set(
                        &target_config_cursor,
                        WIDTH-16,
                        rgb_config_menu.selected_index*ROW_HEIGHT,
                        16,
                        ROW_HEIGHT);
            }
            else
            {
                if(rgb_key_select_menu.selected_index)
                {
                    fezui_cursor_set(
                            &target_config_cursor,
                            WIDTH - 1 - strlen(rgb_mode_items[RGB_Configs[rgb_key_select_menu.selected_index-1].mode])*5,
                            rgb_config_menu.selected_index*ROW_HEIGHT,
                            1 + strlen(rgb_mode_items[RGB_Configs[rgb_key_select_menu.selected_index-1].mode])*5,
                            ROW_HEIGHT);
                }
                else
                {
                    fezui_cursor_set(
                            &target_config_cursor,
                            WIDTH - 1 - strlen(rgb_global_mode_items[RGB_GlobalConfig.mode])*5,
                            rgb_config_menu.selected_index*ROW_HEIGHT,
                            1 + strlen(rgb_global_mode_items[RGB_GlobalConfig.mode])*5,
                            ROW_HEIGHT);
                }

            }
        }
        else
        {
            fezui_cursor_set(
                    &target_config_cursor,
                    SPERATOR_X,
                    rgb_config_menu.selected_index*ROW_HEIGHT,
                    WIDTH-SPERATOR_X,
                    ROW_HEIGHT);
        }
    }
    else
    {
        fezui_cursor_set(
                &target_config_cursor,
                SPERATOR_X,
                rgb_config_menu.selected_index*ROW_HEIGHT,
                1,
                ROW_HEIGHT);
    }
    
    fezui_cursor_move(&fezui, &config_cursor, &target_config_cursor);
}

void rgbconfigpage_draw(void *page)
{
    
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x8_mr);
    for(uint8_t i=0;i<rgb_key_select_menu.len;i++)
    {
        u8g2_DrawStr(&(fezui.u8g2),1 ,ROW_HEIGHT*(i+1)-1, rgb_key_select_menu.items[i]);
    }
    for(uint8_t i=0;i<rgb_config_menu.len;i++)
    {
        u8g2_DrawStr(&(fezui.u8g2),SPERATOR_X + 2,ROW_HEIGHT*(i+1)-1, rgb_config_menu.items[i]);
    }
    uint8_t font_width = u8g2_GetMaxCharWidth(&(fezui.u8g2));
    if(rgb_key_select_menu.selected_index)
    {
        u8g2_DrawStr(&(fezui.u8g2), WIDTH - strlen(rgb_mode_items[RGB_Configs[rgb_key_select_menu.selected_index-1].mode])*font_width, ROW_HEIGHT*1-1, rgb_mode_items[RGB_Configs[rgb_key_select_menu.selected_index-1].mode]);

        sprintf(fezui_buffer,"%d",RGB_Configs[rgb_key_select_menu.selected_index-1].rgb.r);
        u8g2_DrawStr(&(fezui.u8g2), WIDTH - strlen(fezui_buffer)*font_width, ROW_HEIGHT*2-1, fezui_buffer);

        sprintf(fezui_buffer,"%d", RGB_Configs[rgb_key_select_menu.selected_index-1].rgb.g);
        u8g2_DrawStr(&(fezui.u8g2), WIDTH - strlen(fezui_buffer)*font_width, ROW_HEIGHT*3-1, fezui_buffer);

        sprintf(fezui_buffer,"%d", RGB_Configs[rgb_key_select_menu.selected_index-1].rgb.b);
        u8g2_DrawStr(&(fezui.u8g2), WIDTH - strlen(fezui_buffer)*font_width, ROW_HEIGHT*4-1, fezui_buffer);

        sprintf(fezui_buffer,"%0.0f", RGB_Configs[rgb_key_select_menu.selected_index-1].speed*1000);
        u8g2_DrawStr(&(fezui.u8g2), WIDTH - strlen(fezui_buffer)*font_width, ROW_HEIGHT*8-1, fezui_buffer);
    }
    else
    {
        u8g2_DrawStr(&(fezui.u8g2), WIDTH - strlen(rgb_global_mode_items[RGB_GlobalConfig.mode])*font_width, ROW_HEIGHT*1-1, rgb_global_mode_items[RGB_GlobalConfig.mode]);

        sprintf(fezui_buffer,"%0.0f",RGB_GlobalConfig.speed*1000);
        u8g2_DrawStr(&(fezui.u8g2), WIDTH - strlen(fezui_buffer)*font_width, ROW_HEIGHT*8-1, fezui_buffer);
    }
    sprintf(fezui_buffer,"%d",target_rgb->r);
    u8g2_DrawStr(&(fezui.u8g2), WIDTH - strlen(fezui_buffer)*font_width, ROW_HEIGHT*2-1, fezui_buffer);

    sprintf(fezui_buffer,"%d",target_rgb->g);
    u8g2_DrawStr(&(fezui.u8g2), WIDTH - strlen(fezui_buffer)*font_width, ROW_HEIGHT*3-1, fezui_buffer);

    sprintf(fezui_buffer,"%d",target_rgb->b);
    u8g2_DrawStr(&(fezui.u8g2), WIDTH - strlen(fezui_buffer)*font_width, ROW_HEIGHT*4-1, fezui_buffer);

    sprintf(fezui_buffer,"%d",target_hsv->h);
    u8g2_DrawStr(&(fezui.u8g2), WIDTH - strlen(fezui_buffer)*font_width, ROW_HEIGHT*5-1, fezui_buffer);

    sprintf(fezui_buffer,"%d",target_hsv->s);
    u8g2_DrawStr(&(fezui.u8g2), WIDTH - strlen(fezui_buffer)*font_width, ROW_HEIGHT*6-1, fezui_buffer);

    sprintf(fezui_buffer,"%d",target_hsv->v);
    u8g2_DrawStr(&(fezui.u8g2), WIDTH - strlen(fezui_buffer)*font_width, ROW_HEIGHT*7-1, fezui_buffer);

    u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x8_mr);
    u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X+2, 8 - 1, current_key_name);

    u8g2_DrawBox(&(fezui.u8g2), 0, ROW_HEIGHT*6, SPERATOR_X, ROW_HEIGHT*2);
    uint8_t color = u8g2_GetDrawColor(&(fezui.u8g2));
    u8g2_SetDrawColor(&(fezui.u8g2), 2);
    u8g2_DrawStr(&(fezui.u8g2), +1, ROW_HEIGHT*7, "#");
    if(rgb_key_select_menu.selected_index)
    {
        sprintf(fezui_buffer,"%02x",RGB_Configs[rgb_key_select_menu.selected_index-1].rgb.r);
        sprintf(fezui_buffer+2,"%02x",RGB_Configs[rgb_key_select_menu.selected_index-1].rgb.g);
        sprintf(fezui_buffer+4,"%02x",RGB_Configs[rgb_key_select_menu.selected_index-1].rgb.b);
        strupr(fezui_buffer);
        u8g2_DrawStr(&(fezui.u8g2), 1, ROW_HEIGHT*8, fezui_buffer);
    }
    else
    {
        sprintf(fezui_buffer,"%02x",RGB_GlobalConfig.rgb.r);
        sprintf(fezui_buffer+2,"%02x",RGB_GlobalConfig.rgb.g);
        sprintf(fezui_buffer+4,"%02x",RGB_GlobalConfig.rgb.b);
        strupr(fezui_buffer);
        u8g2_DrawStr(&(fezui.u8g2), 1, ROW_HEIGHT*8, fezui_buffer);
    }
    u8g2_SetDrawColor(&(fezui.u8g2), color);

    fezui_draw_cursor(&fezui, &cursor);
    fezui_draw_cursor(&fezui, &config_cursor);

    u8g2_DrawVLine(&(fezui.u8g2), SPERATOR_X, 0, 64);
    

}

void usart_rgb_set()
{
}

static void key_up_cb(void *k)
{
    
    if(key_selected)
    {
        if(configing)
        {
            switch(rgb_config_menu.selected_index)
            {
                case 0:
                    if(rgb_key_select_menu.selected_index)
                    {
                        VAR_LOOP_INCREMENT(RGB_Configs[rgb_key_select_menu.selected_index-1].mode,RGB_MODE_STATIC,RGB_MODE_REACT_TRIGGER,1)
                    }
                    else
                    {
                        VAR_LOOP_INCREMENT(RGB_GlobalConfig.mode,RGB_GLOBAL_MODE_OFF,RGB_GLOBAL_MODE_JELLY,1)
                    }
                    break;
                case 1:
                    VAR_LOOP_INCREMENT(RGB_Configs[rgb_key_select_menu.selected_index-1].rgb.r,0,255,1)
                    rgb_to_hsv(target_hsv, target_rgb);
                    break;
                case 2:
                    VAR_LOOP_INCREMENT(RGB_Configs[rgb_key_select_menu.selected_index-1].rgb.g,0,255,1)
                    rgb_to_hsv(target_hsv, target_rgb);
                    break;
                case 3:
                    VAR_LOOP_INCREMENT(RGB_Configs[rgb_key_select_menu.selected_index-1].rgb.b,0,255,1)
                    rgb_to_hsv(target_hsv, target_rgb);
                    break;
                case 4:
                    VAR_LOOP_INCREMENT(target_hsv->h,0,360,1)
                    hsv_to_rgb(target_rgb, target_hsv);
                    break;
                case 5:
                    VAR_LOOP_INCREMENT(target_hsv->s,0,100,1)
                    hsv_to_rgb(target_rgb, target_hsv);
                    break;
                case 6:
                    VAR_LOOP_INCREMENT(target_hsv->v,0,100,1)
                    hsv_to_rgb(target_rgb, target_hsv);
                    break;
                case 7:
                    if(rgb_key_select_menu.selected_index)
                    {
                        RGB_Configs[rgb_key_select_menu.selected_index-1].speed+=0.001;
                    }
                    else
                    {
                        RGB_GlobalConfig.speed+=0.001;
                    }
                    break;
                default:
                    break;
            }
            usart_rgb_set();
        }
        else
        {
            lefl_menu_index_increase(&rgb_config_menu, 1);
        }
    }
    else
    {
        lefl_menu_index_increase(&rgb_key_select_menu, 1);
        set_target_color();
    }
    
}


static void key_down_cb(void *k)
{
    
    if(key_selected)
    {
        if(configing)
        {
            switch(rgb_config_menu.selected_index)
            {
                case 0:
                    if(rgb_key_select_menu.selected_index)
                    {
                        VAR_LOOP_DECREMENT(RGB_Configs[rgb_key_select_menu.selected_index-1].mode,RGB_MODE_STATIC,RGB_MODE_REACT_TRIGGER,1)
                    }
                    else
                    {
                        VAR_LOOP_INCREMENT(RGB_GlobalConfig.mode,RGB_GLOBAL_MODE_OFF,RGB_GLOBAL_MODE_JELLY,1)
                    }
                    break;
                case 1:
                    VAR_LOOP_DECREMENT(RGB_Configs[rgb_key_select_menu.selected_index-1].rgb.r,0,255,1)
                    rgb_to_hsv(target_hsv, target_rgb);
                    break;
                case 2:
                    VAR_LOOP_DECREMENT(RGB_Configs[rgb_key_select_menu.selected_index-1].rgb.g,0,255,1)
                    rgb_to_hsv(target_hsv, target_rgb);
                    break;
                case 3:
                    VAR_LOOP_DECREMENT(RGB_Configs[rgb_key_select_menu.selected_index-1].rgb.b,0,255,1)
                    rgb_to_hsv(target_hsv, target_rgb);
                    break;
                case 4:
                    VAR_LOOP_DECREMENT(target_hsv->h,0,360,1)
                    hsv_to_rgb(target_rgb, target_hsv);
                    break;
                case 5:
                    VAR_LOOP_DECREMENT(target_hsv->s,0,100,1)
                    hsv_to_rgb(target_rgb, target_hsv);
                    break;
                case 6:
                    VAR_LOOP_DECREMENT(target_hsv->v,0,100,1)
                    hsv_to_rgb(target_rgb, target_hsv);
                    break;
                case 7:
                    if(rgb_key_select_menu.selected_index)
                    {
                        RGB_Configs[rgb_key_select_menu.selected_index-1].speed-=0.001;
                    }
                    else
                    {
                        RGB_GlobalConfig.speed-=0.001;
                    }
                    break;
                default:
                    break;
            }
            usart_rgb_set();
        }
        else
        {
            lefl_menu_index_increase(&rgb_config_menu, -1);
        }
    }
    else
    {
        lefl_menu_index_increase(&rgb_key_select_menu, -1);
        set_target_color();
    }
    
}

void rgbconfigpage_load(void *page)
{
    fezui_cursor_set(
            &config_cursor,
            SPERATOR_X,
            0,
            1,
            HEIGHT);
    set_target_color();
    key_attach(&KEY_FN_K6, KEY_EVENT_DOWN, LAMBDA(void,(void*k)
    {
        if(configing)
        {
            configing = false;
        }
        else
        {
            if(key_selected)
            {
                key_selected = false;
                //rgb_save();
            }
            else
            {
                fezui_link_frame_go_back(&mainframe);
            }
        }
    }));
    key_attach(&KEY_FN_K5, KEY_EVENT_DOWN, LAMBDA(void,(void*k)
    {
        if(key_selected)
        {
            configing = !configing;
        }
        else
        {
            lefl_menu_click(&rgb_key_select_menu);
        }
    }));
    key_attach(&KEY_KNOB, KEY_EVENT_DOWN, LAMBDA(void,(void*k)
    {
        if(key_selected)
        {
            configing = !configing;
        }
        else
        {
            lefl_menu_click(&rgb_key_select_menu);
        }
    }));
    key_attach(&KEY_KNOB_CLOCKWISE, KEY_EVENT_DOWN, key_up_cb);
    key_attach(&KEY_KNOB_ANTICLOCKWISE, KEY_EVENT_DOWN, key_down_cb);
}

fezui_link_page_t rgbconfigpage={rgbconfigpage_logic,rgbconfigpage_draw,rgbconfigpage_load};


