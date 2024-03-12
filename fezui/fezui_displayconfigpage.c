/*
 * fezui_displayconfigpage.c
 *
 *  Created on: Aug 24, 2023
 *      Author: xq123
 */
#include "fezui.h"
#include "fezui_var.h"

#define ROW_HEIGHT 16

static fezui_animated_menu_t displayconfig_menu;
static uint16_t speed;
static const fezui_menuitem_t displayconfig_menu_items[]=
{
    {"bInvert",&fezui.invert},
    {"8Contrast",&fezui.contrast},
    {"uSpeed",&speed},
    {"uTimeout",&fezui.screensaver_timeout},
};
static fezui_flyout_numberic_dialog_t dialog;
static bool configing;

void displayconfig_menu_cb(void*m)
{
    switch (displayconfig_menu.selected_index)
    {
        case 0:
            fezui_flyout_numberic_dialog_init(&dialog,&fezui.invert,FEZUI_TYPE_BOOL,0,1,1,"Invert");
            break;
        case 1:
            fezui_flyout_numberic_dialog_init(&dialog,&fezui.contrast,FEZUI_TYPE_UINT8,0,255,1,"Contrast");
            break;
        case 2:
            fezui_flyout_numberic_dialog_init(&dialog,&speed,FEZUI_TYPE_UINT16,1,20,1,"Speed");
            break;
        case 3:
            fezui_flyout_numberic_dialog_init(&dialog,&fezui.screensaver_timeout,FEZUI_TYPE_UINT16,0,600,1,"Timeout");
            break;
        default:
            break;
    }
    fezui_flyout_numberic_dialog_show(&dialog);
}

void displayconfigpage_init()
{
    fezui_animated_menu_init(&displayconfig_menu, displayconfig_menu_items, sizeof(displayconfig_menu_items)/sizeof(fezui_menuitem_t), displayconfig_menu_cb);
}

static void displayconfigpage_logic(void *page)
{
    fezui_animated_menu_update(&fezui,&displayconfig_menu);
    fezui_flyout_numberic_dialog_update(&fezui,&dialog);
}

static void displayconfigpage_draw(void *page)
{
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_6x13_mr);
    fezui_draw_animated_menu(&fezui,0,0,WIDTH,HEIGHT,&displayconfig_menu,16,3);
    fezui_animated_menu_get_cursor(&fezui,0,0,WIDTH,HEIGHT,&displayconfig_menu,16,&target_cursor);
    fezui_draw_cursor(&fezui, &cursor);
    fezui_veil(&fezui,0,0,WIDTH,HEIGHT,dialog.animation*5,fezui.invert?1:0);
    fezui_draw_flyout_numberic_dialog(&fezui,&dialog);
    fezui.speed = ((float)speed)/100;
    fezui_apply(&fezui);
}

static void key_up_cb(void *k)
{

    if(configing)
    {
        switch (displayconfig_menu.selected_index)
        {
            case 0:
                VAR_LOOP_INCREMENT(fezui.invert,0,1,1);
                break;
            case 1:
                VAR_LOOP_INCREMENT(fezui.contrast,0,255,1);
                break;
            case 2:
                VAR_LOOP_INCREMENT(speed,1,20,1);
                break;
            case 3:
                VAR_LOOP_INCREMENT(fezui.screensaver_timeout,0,600,1);
                break;
            default:
                break;
        }
    }
    else
    {
        fezui_animated_menu_index_increase(&displayconfig_menu, 1);
    }
}

static void key_down_cb(void *k)
{
    if(configing)
    {
        switch (displayconfig_menu.selected_index)
        {
            case 0:
                VAR_LOOP_DECREMENT(fezui.invert,0,1,1);
                break;
            case 1:
                VAR_LOOP_DECREMENT(fezui.contrast,0,255,1);
                break;
            case 2:
                VAR_LOOP_DECREMENT(speed,1,20,1);
                break;
            case 3:
                VAR_LOOP_DECREMENT(fezui.screensaver_timeout,0,600,1);
                break;
            default:
                break;
        }
    }
    else
    {
        fezui_animated_menu_index_increase(&displayconfig_menu, -1);
    }
}

static void displayconfigpage_load(void *page)
{
    fezui_animated_menu_begin(&displayconfig_menu);
    speed = fezui.speed*100;
    key_attach(&KEY_FN_K6, KEY_EVENT_DOWN, LAMBDA(void,(void*k)
    {
        if(configing)
        {
            fezui_flyout_numberic_dialog_close(&dialog);
            configing=false;
        }
        else
        {
            //fezui_save();
            fezui_link_frame_go_back(&mainframe);
        }
    }));
    key_attach(&KEY_FN_K5, KEY_EVENT_DOWN, LAMBDA(void,(void*k)
    {
        if(configing)
        {
            fezui_flyout_numberic_dialog_close(&dialog);
        }
        else
        {
            displayconfig_menu_cb(&displayconfig_menu);
        }
        configing = !configing;
    }));
    key_attach(&KEY_KNOB, KEY_EVENT_DOWN, LAMBDA(void,(void*k)
    {
        if(configing)
        {
            fezui_flyout_numberic_dialog_close(&dialog);
        }
        else
        {
            displayconfig_menu_cb(&displayconfig_menu);
        }
        configing = !configing;
    }));
    key_attach(&KEY_KNOB_CLOCKWISE, KEY_EVENT_DOWN, key_up_cb);
    key_attach(&KEY_KNOB_ANTICLOCKWISE, KEY_EVENT_DOWN, key_down_cb);
}

fezui_link_page_t displayconfigpage={displayconfigpage_logic,displayconfigpage_draw, displayconfigpage_load};

