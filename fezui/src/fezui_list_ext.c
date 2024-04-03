#include"fezui.h"
static void string_item_draw(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, void *item, uint16_t index)
{
    u8g2_font_calc_vref_fnptr fnptr_bk = fezui_ptr->u8g2.font_calc_vref;
    u8g2_SetFontPosBottom(&(fezui_ptr->u8g2));
    u8g2_DrawStr(&fezui_ptr->u8g2,x+1,y+h,item);
    fezui_ptr->u8g2.font_calc_vref = fnptr_bk;
}

static void string_item_get_cursor(fezui_t *fezui_ptr, fezui_cursor_t *cursor, void *item)
{
    cursor->w = u8g2_GetStrWidth(&fezui_ptr->u8g2,item) + 1;
}

void fezui_animated_string_listbox_init(fezui_animated_listbox_t *listbox, const char **items, uint8_t len, void (*cb)(void *listbox))
{
    fezui_listbox_init(&listbox->listbox,(void**)items,len,cb,string_item_draw,string_item_get_cursor);
    listbox->scroll_animation.begin_time=fezui_tick;
    listbox->scroll_animation.duration = DEFAULT_ANIMATION_DURATION;
    listbox->scroll_animation.easing_func = DEFAULT_ANIMATION_EASE_FUNCTION;
    listbox->scroll_animation.mode = DEFAULT_ANIMATION_MODE;
}

void fezui_animated_listbox_init(fezui_animated_listbox_t *listbox, void **items, uint8_t len, void (*cb)(void *listbox),item_draw_fn item_draw_cb,item_cursor_fn item_cursor_cb)
{
    fezui_listbox_init(&listbox->listbox,(void**)items,len,cb,item_draw_cb,item_cursor_cb);
    listbox->scroll_animation.begin_time=fezui_tick;
    listbox->scroll_animation.duration = DEFAULT_ANIMATION_DURATION;
    listbox->scroll_animation.easing_func = DEFAULT_ANIMATION_EASE_FUNCTION;
    listbox->scroll_animation.mode = DEFAULT_ANIMATION_MODE;
}
void fezui_animated_listbox_index_increase(fezui_animated_listbox_t *listbox, int8_t delta)
{
    fezui_animation_begin(&listbox->scroll_animation);
    listbox->listbox.offset = FEZUI_ANIMATION_GET_VALUE(&listbox->scroll_animation,listbox->listbox.offset,listbox->targetoffset);
    fezui_listbox_index_increase(&listbox->listbox,delta);
}

void fezui_animated_listbox_click(fezui_animated_listbox_t *listbox)
{
    if (listbox->listbox.list.list_cb != NULL)
        listbox->listbox.list.list_cb(listbox);
}

void fezui_animated_listbox_get_cursor(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, fezui_animated_listbox_t *listbox, u8g2_uint_t item_height, fezui_cursor_t *c)
{
    fezui_cursor_t cursor;
    cursor.x = x;
    cursor.y = item_height * (listbox->listbox.list.selected_index) - FEZUI_ANIMATION_GET_VALUE(&listbox->scroll_animation,listbox->listbox.offset,listbox->targetoffset);
    if (cursor.y + item_height > y + h)
    {
        cursor.y = y + h - item_height;
    }
    if (cursor.y < y)
    {
        cursor.y = y;
    }
    cursor.w = w;
    cursor.h = item_height;
    if(listbox->listbox.item_cursor_cb)
    {
        listbox->listbox.item_cursor_cb(fezui_ptr,&cursor,listbox->listbox.list.items[listbox->listbox.list.selected_index]);
    }
    *c=cursor;
}
void fezui_animated_listbox_begin(fezui_animated_listbox_t *listbox)
{
    listbox->start_animation.begin_time=fezui_tick;
    listbox->start_animation.duration = DEFAULT_ANIMATION_DURATION;
    listbox->start_animation.easing_func = DEFAULT_ANIMATION_EASE_FUNCTION;
    listbox->start_animation.mode = DEFAULT_ANIMATION_MODE;
}
void fezui_draw_animated_listbox(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, fezui_animated_listbox_t *listbox, u8g2_uint_t item_height)
{
    fezui_animation_calculate(&listbox->start_animation);
    fezui_animation_calculate(&listbox->scroll_animation);
    if ((listbox->listbox.list.selected_index + 1) * item_height - listbox->targetoffset > h)
    {
        listbox->targetoffset = (listbox->listbox.list.selected_index + 1) * item_height - h;
    }
    if ((listbox->listbox.list.selected_index) * item_height < listbox->targetoffset)
    {
        listbox->targetoffset = (listbox->listbox.list.selected_index) * item_height;
    }
    u8g2_SetClipWindow(&(fezui_ptr->u8g2), x, y, x + w, y + h);
    for (uint16_t i = 0; i < listbox->listbox.list.len; i++)
    {
        listbox->listbox.item_draw_cb(fezui_ptr, x, (u8g2_int_t)floorf(y+(item_height * (i) - FEZUI_ANIMATION_GET_VALUE(&listbox->scroll_animation,listbox->listbox.offset,listbox->targetoffset)) * listbox->start_animation.value - item_height*(1-listbox->start_animation.value) + 0.5), w,item_height,listbox->listbox.list.items[i],i);
    }
    if (listbox->listbox.show_scrollbar)
    {
        fezui_draw_scrollbar(fezui_ptr, x + w - 5, y, 5, (u8g2_int_t)(FEZUI_ANIMATION_GET_VALUE(&listbox->start_animation,0,h)  + 0.5), (float)h / (float)(item_height * listbox->listbox.list.len), FEZUI_ANIMATION_GET_VALUE(&listbox->scroll_animation,listbox->listbox.offset,listbox->targetoffset) / (float)(item_height * listbox->listbox.list.len - h), ORIENTATION_VERTICAL);
    }
    u8g2_SetMaxClipWindow(&(fezui_ptr->u8g2));
}

static void menu_item_draw(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, void *menu_item, uint16_t index)
{
    u8g2_font_calc_vref_fnptr fnptr_bk = fezui_ptr->u8g2.font_calc_vref;
    u8g2_SetFontPosBottom(&(fezui_ptr->u8g2));
    fezui_menuitem_t* item = menu_item;
    char *_Format=strrchr(item->header,'%');
    char _FormatStr[16];
    if(_Format)
    {
        memcpy(g_fezui_printf_buffer,item->header+1,_Format-item->header-1);
        u8g2_DrawStr(&(fezui_ptr->u8g2), x, y+h, g_fezui_printf_buffer);
        sprintf(_FormatStr,"[%s]",_Format);
    }
    else
    {
        u8g2_DrawStr(&(fezui_ptr->u8g2), x+1, y+h, item->header + 1);
    }
    switch (*(item->header))
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
    fezui_printf_right_aligned(fezui_ptr, x + w, y+h, g_fezui_printf_buffer);
    fezui_ptr->u8g2.font_calc_vref = fnptr_bk;
}

static void menu_item_get_cursor(fezui_t *fezui_ptr, fezui_cursor_t *cursor, void *item)
{
    cursor->w = u8g2_GetStrWidth(&fezui_ptr->u8g2, ((fezui_menuitem_t*)item)->header + 1) + 1;
}

void fezui_animated_menu_list_init(fezui_animated_listbox_t *menu,const fezui_menuitem_t **items, uint8_t len, void (*cb)(void *menu))
{
    fezui_listbox_init(&menu->listbox,(void**)items,len,cb,menu_item_draw,menu_item_get_cursor);
    menu->scroll_animation.begin_time=fezui_tick;
    menu->scroll_animation.duration = DEFAULT_ANIMATION_DURATION;
    menu->scroll_animation.easing_func = DEFAULT_ANIMATION_EASE_FUNCTION;
    menu->scroll_animation.mode = DEFAULT_ANIMATION_MODE;
}

void fezui_animated_menu_init(fezui_animated_menu_t *menu,const fezui_menuitem_t *items, uint8_t len, void (*cb)(void *menu))
{
    menu->items = items;
    menu->len = len;
    menu->menu_cb = cb;
    menu->selected_index = 0;
}

void fezui_animated_menu_index_increase(fezui_animated_menu_t *menu, int8_t delta)
{
    menu->selected_index += delta;
    if (menu->selected_index >= menu->len)
    {
        menu->selected_index = 0;
    }
    if (menu->selected_index < 0)
    {
        menu->selected_index = menu->len - 1;
    }
}

void fezui_animated_menu_click(fezui_animated_menu_t *menu)
{
    if (menu->menu_cb != NULL)
        menu->menu_cb(menu);
}

void fezui_animated_menu_update(fezui_t *fezui_ptr, fezui_animated_menu_t *menu)
{
    CONVERGE_TO_ROUNDED(menu->offset, menu->targetoffset, fezui_ptr->speed);
    CONVERGE_TO(menu->animation, 1, fezui_ptr->speed);
}

void fezui_animated_menu_get_cursor(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, fezui_animated_menu_t *menu, u8g2_uint_t item_height, fezui_cursor_t *c)
{
    c->x = x;
    c->y = item_height * (menu->selected_index) - (u8g2_int_t)menu->offset;
    c->h = item_height;
    c->w = u8g2_GetStrWidth(&fezui_ptr->u8g2, menu->items[menu->selected_index].header + 1) + 1;
    if (c->y + item_height > y + h)
    {
        c->y = y + h - item_height;
    }
    if (c->y < y)
    {
        c->y = y;
    }
}
void fezui_animated_menu_begin(fezui_animated_menu_t *menu)
{
    menu->animation = 0;
}
void fezui_draw_animated_menu(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, fezui_animated_menu_t *menu, u8g2_uint_t item_height, u8g2_uint_t adjust)
{
    if ((menu->selected_index + 1) * item_height - menu->targetoffset > h)
    {
        menu->targetoffset = (menu->selected_index + 1) * item_height - h;
    }
    if ((menu->selected_index) * item_height < menu->targetoffset)
    {
        menu->targetoffset = (menu->selected_index) * item_height;
    }
    u8g2_SetClipWindow(&(fezui_ptr->u8g2), x, y, x + w, y + h);
    if (menu->show_scrollbar)
    {
        fezui_draw_scrollbar(fezui_ptr, x + w - 5, y, 5, (u8g2_int_t)(h * menu->animation + 0.5), (float)h / (float)(item_height * menu->len), (float)menu->offset / (float)(item_height * menu->len - h), ORIENTATION_VERTICAL);
        w-=5;
    }
    for (uint16_t i = 0; i < menu->len; i++)
    {
        char *_Format=strrchr(menu->items[i].header,'%');
        char _FormatStr[16];
        if(_Format)
        {
            memcpy(g_fezui_printf_buffer,menu->items[i].header+1,_Format-menu->items[i].header-1);
            u8g2_DrawStr(&(fezui_ptr->u8g2), x + 1, (u8g2_int_t)floorf(y+(item_height * (i + 1) - (u8g2_int_t)menu->offset - adjust) * menu->animation + 0.5), g_fezui_printf_buffer);
            sprintf(_FormatStr,"[%s]",_Format);
        }
        else
        {
            u8g2_DrawStr(&(fezui_ptr->u8g2), x + 1, (u8g2_int_t)floorf(y+(item_height * (i + 1) - (u8g2_int_t)menu->offset - adjust) * menu->animation + 0.5), menu->items[i].header + 1);
        }
        switch (*(menu->items[i].header))
        {
        case FEZUI_TYPE_FLOAT:
            sprintf(g_fezui_printf_buffer, _Format ? _FormatStr :  "[%f]", *(float *)menu->items[i].target);
            break;
        case FEZUI_TYPE_DOUBLE:
            sprintf(g_fezui_printf_buffer,  _Format ? _FormatStr : "[%lf]", *(double *)menu->items[i].target);
            break;
        case FEZUI_TYPE_INT16:
            sprintf(g_fezui_printf_buffer,  _Format ? _FormatStr : "[%d]", *(int16_t*)menu->items[i].target);
            break;
        case FEZUI_TYPE_INT32:
            sprintf(g_fezui_printf_buffer,  _Format ? _FormatStr :  "[%ld]", *(int32_t*)menu->items[i].target);
            break;
        case FEZUI_TYPE_UINT16:
            sprintf(g_fezui_printf_buffer,  _Format ? _FormatStr :  "[%u]", *(uint16_t*)menu->items[i].target);
            break;
        case FEZUI_TYPE_UINT32:
            sprintf(g_fezui_printf_buffer,  _Format ? _FormatStr :  "[%lu]", *(uint32_t*)menu->items[i].target);
            break;
        case FEZUI_TYPE_UINT8:
            sprintf(g_fezui_printf_buffer,  _Format ? _FormatStr :  "[%d]", *(uint8_t *)menu->items[i].target);
            break;
        case FEZUI_TYPE_BOOL:
        case 'B':
            sprintf(g_fezui_printf_buffer, "[%s]", *(bool*)menu->items[i].target?"ON":"OFF");
            break;
        default:
            break;
        }
        fezui_printf_right_aligned(fezui_ptr, x + w, (u8g2_int_t)floorf(y+(item_height * (i + 1) - (u8g2_int_t)menu->offset - adjust) * menu->animation + 0.5), g_fezui_printf_buffer);
    }
    u8g2_SetMaxClipWindow(&(fezui_ptr->u8g2));
}