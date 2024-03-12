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

#define MAINBAR_X (SPERATOR_X+4)
#define MAINBAR_Y (ROW_HEIGHT+4)
#define MAINBAR_W (18)
#define MAINBAR_H (40)

#define SUBBAR_X (SPERATOR_X+24+4)
#define SUBBAR_Y (ROW_HEIGHT+4)
#define SUBBAR_W (18)
#define SUBBAR_H (40)

void keyconfig_digital_mode_menu_cb(void* m);
void keyconfig_analog_normal_mode_menu_cb(void* m);
void keyconfig_analog_rapid_mode_menu_cb(void* m);
void keyconfig_analog_speed_mode_menu_cb(void* m);

static fezui_scrollview_t scrollview =
{
    //.content_height=100,
    .content_width=80,
    .ordinate=0
};
static float target_ordinate=0;

static bool configing = false;
static bool mode_switching = false;
static float* target_property;
static float advancedconfigpage_interval;

fezui_list_base_t keyconfig_digital_mode_menu;
const char* keyconfig_digital_mode_menu_items[] = {"Mode"};

fezui_list_base_t keyconfig_analog_normal_mode_menu;
const char* keyconfig_analog_normal_mode_menu_items[] = {"Mode","Upper bound","Lower bound","Trigger distance","Schmitt parameter"};

fezui_list_base_t keyconfig_analog_rapid_mode_menu;
const char* keyconfig_analog_rapid_mode_menu_items[] = {"Mode","Upper bound","Lower bound","Trigger distance","Release distance","Upper deadzone","Lower deadzone"};

fezui_list_base_t keyconfig_analog_speed_mode_menu;
const char* keyconfig_analog_speed_mode_menu_items[] = {"Mode","Upper bound","Lower bound","Trigger speed","Release speed","Upper deadzone","Lower deadzone"};

fezui_list_base_t *current_menu=&keyconfig_digital_mode_menu;

AdvancedKey* current_config_advanced_key;

static fezui_progressbar_t mainbar = {.max=4096,.min=0,.orientation=ORIENTATION_VERTICAL};
static fezui_progressbar_t subbar = {.max=1,.min=0,.orientation=ORIENTATION_VERTICAL};
static fezui_scrolling_text_t scrolling_text;
static char binding_text[256];

void keyconfig_digital_mode_menu_cb(void* m)
{
    switch (((fezui_list_base_t*)m)->selected_index)
    {
        case 0:
            mode_switching=true;
            keyconfig_digital_mode_menu.selected_index=0;
            keyconfig_analog_normal_mode_menu.selected_index=0;
            keyconfig_analog_rapid_mode_menu.selected_index=0;
            keyconfig_analog_speed_mode_menu.selected_index=0;
            break;
        default:
            break;
    }
}

void keyconfig_analog_normal_mode_menu_cb(void* m)
{
    switch (((fezui_list_base_t*)m)->selected_index)
    {
        case 0:
            mode_switching=true;
            keyconfig_digital_mode_menu.selected_index=0;
            keyconfig_analog_normal_mode_menu.selected_index=0;
            keyconfig_analog_rapid_mode_menu.selected_index=0;
            keyconfig_analog_speed_mode_menu.selected_index=0;
            break;
        case 1:
            configing=true;
            target_property = &(current_config_advanced_key->upper_bound);
            advancedconfigpage_interval = 0.1;
            break;
        case 2:
            configing=true;
            target_property = &(current_config_advanced_key->lower_bound);
            advancedconfigpage_interval = 0.1;
            break;
        case 3:
            configing=true;
            target_property = &(current_config_advanced_key->trigger_distance);
            advancedconfigpage_interval = 0.001;
            break;
        case 4:
            configing=true;
            target_property = &(current_config_advanced_key->schmitt_parameter);
            advancedconfigpage_interval = 0.001;
            break;
        default:
            break;
    }
}

void keyconfig_analog_rapid_mode_menu_cb(void* m)
{

    switch (((fezui_list_base_t*)m)->selected_index)
    {
        case 0:
            mode_switching=true;
            keyconfig_digital_mode_menu.selected_index=0;
            keyconfig_analog_normal_mode_menu.selected_index=0;
            keyconfig_analog_rapid_mode_menu.selected_index=0;
            keyconfig_analog_speed_mode_menu.selected_index=0;
            break;
        case 1:
            configing=true;
            target_property = &(current_config_advanced_key->upper_bound);
            advancedconfigpage_interval = 0.1;
            break;
        case 2:
            configing=true;
            target_property = &(current_config_advanced_key->lower_bound);
            advancedconfigpage_interval = 0.1;
            break;
        case 3:
            configing=true;
            target_property = &(current_config_advanced_key->trigger_distance);
            advancedconfigpage_interval = 0.001;
            break;
        case 4:
            configing=true;
            target_property = &(current_config_advanced_key->release_distance);
            advancedconfigpage_interval = 0.001;
            break;
        case 5:
            configing=true;
            target_property = &(current_config_advanced_key->upper_deadzone);
            advancedconfigpage_interval = 0.001;
            break;
        case 6:
            configing=true;
            target_property = &(current_config_advanced_key->lower_deadzone);
            advancedconfigpage_interval = 0.001;
            break;
        default:
            break;
    }
}

void keyconfig_analog_speed_mode_menu_cb(void* m)
{

    switch (((fezui_list_base_t*)m)->selected_index)
    {
        case 0:
            mode_switching=true;
            keyconfig_digital_mode_menu.selected_index=0;
            keyconfig_analog_normal_mode_menu.selected_index=0;
            keyconfig_analog_rapid_mode_menu.selected_index=0;
            keyconfig_analog_speed_mode_menu.selected_index=0;
            break;
        case 1:
            configing=true;
            target_property = &(current_config_advanced_key->upper_bound);
            advancedconfigpage_interval = 0.1;
            break;
        case 2:
            configing=true;
            target_property = &(current_config_advanced_key->lower_bound);
            advancedconfigpage_interval = 0.1;
            break;
        case 3:
            configing=true;
            target_property = &(current_config_advanced_key->trigger_speed);
            advancedconfigpage_interval = 0.001;
            break;
        case 4:
            configing=true;
            target_property = &(current_config_advanced_key->release_speed);
            advancedconfigpage_interval = 0.001;
            break;
        case 5:
            configing=true;
            target_property = &(current_config_advanced_key->upper_deadzone);
            advancedconfigpage_interval = 0.001;
            break;
        case 6:
            configing=true;
            target_property = &(current_config_advanced_key->lower_deadzone);
            advancedconfigpage_interval = 0.001;
            break;
        default:
            break;
    }
}

void advancedconfigpage_init()
{
    fezui_list_base_init(&keyconfig_digital_mode_menu, keyconfig_digital_mode_menu_items, sizeof(keyconfig_digital_mode_menu_items)/sizeof(const char*), keyconfig_digital_mode_menu_cb);
    fezui_list_base_init(&keyconfig_analog_normal_mode_menu, keyconfig_analog_normal_mode_menu_items, sizeof(keyconfig_analog_normal_mode_menu_items)/sizeof(const char*), keyconfig_analog_normal_mode_menu_cb);
    fezui_list_base_init(&keyconfig_analog_rapid_mode_menu, keyconfig_analog_rapid_mode_menu_items, sizeof(keyconfig_analog_rapid_mode_menu_items)/sizeof(const char*), keyconfig_analog_rapid_mode_menu_cb);
    fezui_list_base_init(&keyconfig_analog_speed_mode_menu, keyconfig_analog_speed_mode_menu_items, sizeof(keyconfig_analog_speed_mode_menu_items)/sizeof(const char*), keyconfig_analog_speed_mode_menu_cb);

}

static void advancedconfigpage_logic(void *page)
{
    if(configing)
    {
        fezui_cursor_set(
                &target_cursor ,
                SPERATOR_X-4*6,
                current_menu->selected_index*ROW_HEIGHT + ROW_HEIGHT/2 - (u8g2_int_t)scrollview.ordinate,
                //strlen(current_menu->items[current_menu->selected_index])*5+3,
                4*6,
                ROW_HEIGHT/2);
    }
    else if(mode_switching)
    {
        fezui_cursor_set(
                &target_cursor ,
                SPERATOR_X-4*13,
                current_menu->selected_index*ROW_HEIGHT - ROW_HEIGHT/2 - (u8g2_int_t)scrollview.ordinate,
                //strlen(current_menu->items[current_menu->selected_index])*5+3,
                4*13,
                ROW_HEIGHT/2);
    }
    else
    {
        fezui_cursor_set(
                &target_cursor ,
                0,
                current_menu->selected_index*ROW_HEIGHT - (u8g2_int_t)scrollview.ordinate,
                //strlen(current_menu->items[current_menu->selected_index])*5+3,
                SPERATOR_X,
                ROW_HEIGHT);
    }
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
    scrollview.content_height = ROW_HEIGHT*current_menu->len;
    if((current_menu->selected_index+1)*ROW_HEIGHT-target_ordinate>64)
    {
        target_ordinate = (current_menu->selected_index+1)*ROW_HEIGHT-64;
    }
    if((current_menu->selected_index)*ROW_HEIGHT<target_ordinate)
    {
        target_ordinate = (current_menu->selected_index)*ROW_HEIGHT;
    }
    if(current_config_advanced_key->mode==KEY_DIGITAL_MODE)
    {
        target_ordinate = 0;
    }
    fezui_scrolling_text_update(&scrolling_text);
    CONVERGE_TO_ROUNDED(scrollview.ordinate, target_ordinate, fezui.speed);
}

static void advancedconfigpage_draw(void *page)
{
    uint8_t color = u8g2_GetDrawColor(&(fezui.u8g2));

    u8g2_DrawVLine(&(fezui.u8g2), SPERATOR_X, 0, 64);
    u8g2_DrawBox(&(fezui.u8g2), SPERATOR_X,0 ,WIDTH-SPERATOR_X, ROW_HEIGHT);

    mainbar.value=current_config_advanced_key->raw;
    fezui_draw_progressbar(&fezui, MAINBAR_X, MAINBAR_Y, MAINBAR_W, MAINBAR_H, &mainbar);

    subbar.max=current_config_advanced_key->upper_bound;
    subbar.min=current_config_advanced_key->lower_bound;
    subbar.value = current_config_advanced_key->raw;
    fezui_draw_progressbar(&fezui, SUBBAR_X, SUBBAR_Y, SUBBAR_W, SUBBAR_H, &subbar);

    u8g2_DrawHLine(&(fezui.u8g2), MAINBAR_X, MAINBAR_Y+MAINBAR_H-ROUND(MAINBAR_H*(current_config_advanced_key->upper_bound)/(4096.0F)), MAINBAR_W);
    u8g2_DrawLine(&(fezui.u8g2), MAINBAR_X+MAINBAR_W, MAINBAR_Y+MAINBAR_H-ROUND(MAINBAR_H*(current_config_advanced_key->upper_bound)/(4096.0F)), SUBBAR_X,SUBBAR_Y);
    u8g2_DrawLine(&(fezui.u8g2), MAINBAR_X+MAINBAR_W, MAINBAR_Y+MAINBAR_H-ROUND(MAINBAR_H*(current_config_advanced_key->lower_bound)/(4096.0F)), SUBBAR_X,SUBBAR_Y+SUBBAR_H-1);

    u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x8_mr);
    for(uint8_t i=0;i<current_menu->len;i++)
    {
        u8g2_DrawStr(&(fezui.u8g2),1,ROW_HEIGHT*(i+1)-9 - (u8g2_int_t)scrollview.ordinate,current_menu->items[i]);
    }

    u8g2_SetFont(&(fezui.u8g2), u8g2_font_4x6_mr);
    switch (current_config_advanced_key->mode)
    {
        case KEY_DIGITAL_MODE:
            u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X-4*7, ROW_HEIGHT*1-2 - (u8g2_int_t)scrollview.ordinate, "Digital");
            break;
        case KEY_ANALOG_NORMAL_MODE:
            u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X-4*13, ROW_HEIGHT*1-2 - (u8g2_int_t)scrollview.ordinate, "Analog Normal");

            sprintf(fezui_buffer,"%6.1f",current_config_advanced_key->upper_bound);
            u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X-4*6, ROW_HEIGHT*2-2 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

            sprintf(fezui_buffer,"%6.1f",current_config_advanced_key->lower_bound);
            u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X-4*6, ROW_HEIGHT*3-2 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

            sprintf(fezui_buffer,"%6.1f%%",(current_config_advanced_key->trigger_distance)*100);
            u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X-4*7, ROW_HEIGHT*4-2 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

            sprintf(fezui_buffer,"%6.1f%%",(current_config_advanced_key->schmitt_parameter)*100);
            u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X-4*7, ROW_HEIGHT*5-2 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);
            break;
        case KEY_ANALOG_RAPID_MODE:
            u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X-4*12, ROW_HEIGHT*1-2 - (u8g2_int_t)scrollview.ordinate, "Analog Rapid");

            sprintf(fezui_buffer,"%6.1f",current_config_advanced_key->upper_bound);
            u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X-4*6, ROW_HEIGHT*2-2 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

            sprintf(fezui_buffer,"%6.1f",current_config_advanced_key->lower_bound);
            u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X-4*6, ROW_HEIGHT*3-2 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

            sprintf(fezui_buffer,"%6.1f%%",(current_config_advanced_key->trigger_distance)*100);
            u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X-4*7, ROW_HEIGHT*4-2 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

            sprintf(fezui_buffer,"%6.1f%%",(current_config_advanced_key->release_distance)*100);
            u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X-4*7, ROW_HEIGHT*5-2 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

            sprintf(fezui_buffer,"%6.1f%%",(current_config_advanced_key->upper_deadzone)*100);
            u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X-4*7, ROW_HEIGHT*6-2 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

            sprintf(fezui_buffer,"%6.1f%%",(current_config_advanced_key->lower_deadzone)*100);
            u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X-4*7, ROW_HEIGHT*7-2 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);
            break;
        case KEY_ANALOG_SPEED_MODE:
            u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X-4*12, ROW_HEIGHT*1-2 - (u8g2_int_t)scrollview.ordinate, "Analog Speed");

            sprintf(fezui_buffer,"%6.1f",current_config_advanced_key->upper_bound);
            u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X-4*6, ROW_HEIGHT*2-2 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

            sprintf(fezui_buffer,"%6.1f",current_config_advanced_key->lower_bound);
            u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X-4*6, ROW_HEIGHT*3-2 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

            sprintf(fezui_buffer,"%6.1f%%",(current_config_advanced_key->trigger_speed)*100);
            u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X-4*7, ROW_HEIGHT*4-2 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

            sprintf(fezui_buffer,"%6.1f%%",(current_config_advanced_key->release_speed)*100);
            u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X-4*7, ROW_HEIGHT*5-2 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

            sprintf(fezui_buffer,"%6.1f%%",(current_config_advanced_key->upper_deadzone)*100);
            u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X-4*7, ROW_HEIGHT*6-2 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

            sprintf(fezui_buffer,"%6.1f%%",(current_config_advanced_key->lower_deadzone)*100);
            u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X-4*7, ROW_HEIGHT*7-2 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);
            break;
        default:
            break;
    }

    fezui_draw_scrolling_text(&fezui, 1, ROW_HEIGHT*1-2 - (u8g2_int_t)scrollview.ordinate, &scrolling_text);

    u8g2_SetDrawColor(&(fezui.u8g2), 2);
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x8_mr);
    sprintf(fezui_buffer,"KEY%d",current_config_advanced_key-Keyboard_AdvancedKeys+1);
    u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X+2, 8 - 1, fezui_buffer);

    u8g2_SetFont(&(fezui.u8g2), u8g2_font_4x6_mr);
    if(current_config_advanced_key->key.state)
    {
        u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X+2, ROW_HEIGHT - 1, "TRUE");
    }
    else
    {
        u8g2_DrawStr(&(fezui.u8g2), SPERATOR_X+2, ROW_HEIGHT - 1, "FALSE");
    }
    sprintf(fezui_buffer,"%6.1f%%",current_config_advanced_key->value*100);
    u8g2_DrawStr(&(fezui.u8g2), WIDTH-4*7, 8 - 1, fezui_buffer);

    sprintf(fezui_buffer,"%6.1f",current_config_advanced_key->raw);
    u8g2_DrawStr(&(fezui.u8g2), WIDTH-4*6, 16 - 1, fezui_buffer);
    u8g2_SetDrawColor(&(fezui.u8g2), color);
    fezui_draw_cursor(&fezui, &cursor);
}

static void mode_update(int8_t x)
{
    current_config_advanced_key->mode += x;
    if(current_config_advanced_key->mode>KEY_ANALOG_SPEED_MODE)
    {
        current_config_advanced_key->mode=KEY_DIGITAL_MODE;
    }
    if(current_config_advanced_key->mode<KEY_DIGITAL_MODE)
    {
        current_config_advanced_key->mode=KEY_ANALOG_SPEED_MODE;
    }
}

static void advancedconfigpage_load(void *page)
{
    //keyid_prase(current_config_advanced_key->key.id, binding_text, 256);
    fezui_scrolling_text_init(&fezui,&scrolling_text, 78, 0.2, u8g2_font_4x6_mr, binding_text);
    key_attach(&KEY_FN_K6, KEY_EVENT_DOWN, LAMBDA(void,(void*k)
    {
        if(configing||mode_switching)
        {
            configing=false;
            mode_switching=false;
        }
        else
        {
            analog_save();
            fezui_link_frame_go_back(&mainframe);
        }
    }));
    key_attach(&KEY_FN_K5, KEY_EVENT_DOWN, LAMBDA(void,(void*k)
    {
        if(configing)
        {
            configing = false;
        }
        if(mode_switching)
        {
            mode_switching = false;
        }
        else
        {
            fezui_list_base_click(current_menu);
        }
    }));
    key_attach(&KEY_KNOB, KEY_EVENT_DOWN, LAMBDA(void,(void*k)
    {
        if(configing)
        {
            configing = false;
        }
        if(mode_switching)
        {
            mode_switching = false;
        }
        else
        {
            fezui_list_base_click(current_menu);
        }
    }));
    key_attach(&KEY_KNOB_CLOCKWISE, KEY_EVENT_DOWN, LAMBDA(void,(void*k)
    {
        if(configing)
        {
            *target_property += advancedconfigpage_interval;
        }
        else if(mode_switching)
        {
            mode_update(1);
        }
        else
        {
            fezui_list_base_index_increase(current_menu, 1);
        }
    }));
    key_attach(&KEY_KNOB_ANTICLOCKWISE, KEY_EVENT_DOWN, LAMBDA(void,(void*k)
    {
        if(configing)
        {
            *target_property -= advancedconfigpage_interval;
        }
        else if(mode_switching)
        {
            mode_update(-1);
        }
        else
        {
            fezui_list_base_index_increase(current_menu, -1);
        }
    }));
}

fezui_link_page_t advancedconfigpage={advancedconfigpage_logic,advancedconfigpage_draw,advancedconfigpage_load};

