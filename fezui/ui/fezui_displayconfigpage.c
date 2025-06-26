/*
 * fezui_displayconfigpage.c
 *
 *  Created on: Aug 24, 2023
 *      Author: xq123
 */
#include "fezui.h"
#include "fezui_var.h"

#define ROW_HEIGHT 16

typedef struct __fezui_i18n_menuitem_t
{
    const char *header[LANG_NUM];
    void *target;
} fezui_i18n_menuitem_t;

static fezui_animated_listbox_t displayconfig_menu;
static uint16_t speed;
static const fezui_i18n_menuitem_t *displayconfig_menu_ptr_items[] =
{
    &(fezui_i18n_menuitem_t){{"bInvert","b反色"}, &fezui.invert},
    &(fezui_i18n_menuitem_t){{"8Contrast","8对比度"}, &fezui.contrast},
    &(fezui_i18n_menuitem_t){{"uSpeed","u动画速度"}, &speed},
    &(fezui_i18n_menuitem_t){{"uTimeout","u屏幕超时"}, &fezui.screensaver_timeout},
    &(fezui_i18n_menuitem_t){{"bShow FPS","b显示FPS"}, &fezui.show_fps},
    &(fezui_i18n_menuitem_t){{"8Language","8语言"}, &fezui.lang},
};


#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})
static void menu_item_draw(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, fezui_list_base_t *list, uint16_t index)
{
    u8g2_font_calc_vref_fnptr fnptr_bk = fezui_ptr->u8g2.font_calc_vref;
    u8g2_SetFontPosBottom(&(fezui_ptr->u8g2));
    fezui_i18n_menuitem_t* item = (fezui_i18n_menuitem_t*)list->items[index];
    char *_Format=strrchr(item->header[fezui_ptr->lang],'%');
    char _FormatStr[16];
    if(_Format)
    {
        memcpy(g_fezui_printf_buffer,item->header[fezui_ptr->lang]+1,_Format-item->header[fezui_ptr->lang]-1);
        u8g2_DrawUTF8(&(fezui_ptr->u8g2), x, y+h, g_fezui_printf_buffer);
        sprintf(_FormatStr,"[%s]",_Format);
    }
    else
    {
        u8g2_DrawUTF8(&(fezui_ptr->u8g2), x+1, y+h, item->header[fezui_ptr->lang] + 1);
    }
    switch (*(item->header[fezui_ptr->lang]))
    {
    case FEZUI_TYPE_FLOAT:
        sprintf(g_fezui_printf_buffer, _Format ? _FormatStr :  "[%f]", *(float *)item->target);
        break;
    case FEZUI_TYPE_DOUBLE:
        sprintf(g_fezui_printf_buffer,  _Format ? _FormatStr : "[%lf]", *(double *)item->target);
        break;
    case FEZUI_TYPE_INT16:
        sprintf(g_fezui_printf_buffer,  _Format ? _FormatStr : "[%d]", *(int16_t*)item->target);
        break;
    case FEZUI_TYPE_INT32:
        sprintf(g_fezui_printf_buffer,  _Format ? _FormatStr :  "[%ld]", *(int32_t*)item->target);
        break;
    case FEZUI_TYPE_UINT16:
        sprintf(g_fezui_printf_buffer,  _Format ? _FormatStr :  "[%u]", *(uint16_t*)item->target);
        break;
    case FEZUI_TYPE_UINT32:
        sprintf(g_fezui_printf_buffer,  _Format ? _FormatStr :  "[%lu]", *(uint32_t*)item->target);
        break;
    case FEZUI_TYPE_UINT8:
        sprintf(g_fezui_printf_buffer,  _Format ? _FormatStr :  "[%d]", *(uint8_t *)item->target);
        break;
    case FEZUI_TYPE_BOOL:
    case 'B':
        sprintf(g_fezui_printf_buffer, "[%s]", *(bool*)item->target?"ON":"OFF");
        break;
    default:
        break;
    }
    fezui_animated_listbox_t * listbox = container_of(list,fezui_animated_listbox_t,listbox.list);
    
    fezui_printf_right_aligned(fezui_ptr, x + w - (listbox->listbox.show_scrollbar?5:0), y+h, g_fezui_printf_buffer);
    fezui_ptr->u8g2.font_calc_vref = fnptr_bk;
}

static void menu_item_get_cursor(fezui_t *fezui_ptr, fezui_list_base_t *list, fezui_cursor_t *cursor)
{
    cursor->w = u8g2_GetUTF8Width(&fezui_ptr->u8g2, ((fezui_i18n_menuitem_t*)list->items[list->selected_index])->header[fezui_ptr->lang] + 1) + 2;
}

//static const fezui_menuitem_t *displayconfig_menu_ptr_items[4];
void sliderdialog_show(void *target, uint8_t type, float min, float max, float interval, const char *title); 
static void displayconfig_menu_cb(void *m)
{
    extern fezui_page_t sliderdialog;
    switch (displayconfig_menu.listbox.list.selected_index)
    {
    case 0:
        sliderdialog_show(&fezui.invert, FEZUI_TYPE_BOOL, 0, 1, 1, displayconfig_menu_ptr_items[0]->header[fezui.lang]+1);
        break;
    case 1:
        sliderdialog_show(&fezui.contrast, FEZUI_TYPE_UINT8, 0, 255, 1, displayconfig_menu_ptr_items[1]->header[fezui.lang]+1);
        break;
    case 2:
        sliderdialog_show(&speed, FEZUI_TYPE_UINT16, 1, 20, 1, displayconfig_menu_ptr_items[2]->header[fezui.lang]+1);
        break;
    case 3:
        sliderdialog_show(&fezui.screensaver_timeout, FEZUI_TYPE_UINT16, 0, 600, 1, displayconfig_menu_ptr_items[3]->header[fezui.lang]+1);
        break;
    case 4:
        sliderdialog_show(&fezui.show_fps, FEZUI_TYPE_BOOL, 0, 1, 1, displayconfig_menu_ptr_items[4]->header[fezui.lang]+1);
        break;
    case 5:
        sliderdialog_show(&fezui.lang, FEZUI_TYPE_UINT16, 0, 1, 1, displayconfig_menu_ptr_items[5]->header[fezui.lang]+1);
        break;
    default:
        break;
    }
    
    fezui_frame_show_dialog(&g_mainframe, &sliderdialog);
}

void displayconfigpage_init()
{   
    fezui_animated_listbox_init(&displayconfig_menu, (void**)displayconfig_menu_ptr_items, 6, displayconfig_menu_cb, menu_item_draw, menu_item_get_cursor);
    displayconfig_menu.listbox.show_scrollbar = true;
}

static void displayconfigpage_tick(void *page)
{
    //fezui_animated_listbox_update(&fezui, &displayconfig_menu);
}

static void displayconfigpage_draw(void *page)
{
    switch (fezui.lang)
    {
    case LANG_EN:
        u8g2_SetFont(&(fezui.u8g2), u8g2_font_6x13_mr);
        break;
    case LANG_ZH:
        u8g2_SetFont(&(fezui.u8g2), u8g2_font_wqy13_t_custom);
        break;
    default:
        break;
    }
    fezui_draw_animated_listbox(&fezui, 0, 0, WIDTH, HEIGHT, &displayconfig_menu, 16);
    fezui_animated_listbox_get_cursor(&fezui, 0, 0, WIDTH, HEIGHT, &displayconfig_menu, 16, &g_target_cursor);
    fezui_draw_cursor(&fezui, &g_fezui_cursor);
    fezui.speed = ((float)speed) / 100;
    fezui_apply(&fezui);
}

static void displayconfigpage_load(void *page)
{
    fezui_animated_listbox_begin(&displayconfig_menu);
    speed = fezui.speed * 100;
}

static void displayconfigpage_event_handler(void *e)
{
    switch (*(uint16_t *)e)
    {
    case KEY_UP_ARROW:
        fezui_animated_listbox_index_increase(&displayconfig_menu, 1);
        break;
    case KEY_DOWN_ARROW:
        fezui_animated_listbox_index_increase(&displayconfig_menu, -1);
        break;
    case KEY_SPACEBAR:
    case KEY_ENTER:
        displayconfig_menu_cb(&displayconfig_menu);
        break;
    case KEY_ESC:
        fezui_frame_go_back(&g_mainframe);
        break;
    default:
        break;
    }
}

fezui_page_t displayconfigpage = {displayconfigpage_tick, displayconfigpage_draw, displayconfigpage_load,displayconfigpage_event_handler};
