#include "fezui.h"

void fezui_list_base_init(fezui_list_base_t* list, void* *items,uint8_t len,void (*cb)(void* list))
{
    list->items=items;
    list->len=len;
    list->list_cb=cb;
    list->selected_index=0;
}

void fezui_list_base_index_increase(fezui_list_base_t *list, int8_t delta)
{
    list->selected_index+=delta;
    if(list->selected_index>=list->len)
    {
        list->selected_index=0;
    }
    if(list->selected_index<0)
    {
        list->selected_index=list->len-1;
    }
}
void fezui_list_base_click(fezui_list_base_t *list)
{
    if(list->list_cb!=NULL)
        list->list_cb(list);
}

void fezui_listbox_init(fezui_listbox_t* list, void* *items,uint8_t len,void (*cb)(void* list),item_draw_fn item_draw_cb,item_cursor_fn item_cursor_cb)
{
    fezui_list_base_init(&list->list,items,len,cb);
    list->item_draw_cb = item_draw_cb;
    list->item_cursor_cb = item_cursor_cb;
}
void fezui_listbox_index_increase(fezui_listbox_t* list, int8_t delta)
{
    fezui_list_base_index_increase(&list->list,delta);
}
void fezui_listbox_click(fezui_listbox_t* list)
{
    if(list->list.list_cb!=NULL)
        list->list.list_cb(list);
}

void fezui_listbox_get_cursor(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, fezui_listbox_t *listbox, u8g2_uint_t item_height, fezui_cursor_t *c)
{
    fezui_cursor_t cursor;
    cursor.x = x;
    cursor.y = item_height * (listbox->list.selected_index)  - (u8g2_int_t)listbox->offset;
    if (cursor.y + item_height > y + h)
    {
        cursor.y = y + h - item_height;
    }
    if (cursor.y < y)
    {
        cursor.y = y;
    }
    cursor.h = item_height;
    cursor.w = w;
    if(listbox->item_cursor_cb)
    {
        listbox->item_cursor_cb(fezui_ptr,&cursor,listbox->list.items[listbox->list.selected_index]);
    }
    *c=cursor;
}

void fezui_draw_listbox(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, fezui_listbox_t *listbox, u8g2_uint_t item_height)
{
    u8g2_font_calc_vref_fnptr fnptr_bk = fezui_ptr->u8g2.font_calc_vref;
    u8g2_SetFontPosBottom(&(fezui_ptr->u8g2));
    if ((listbox->list.selected_index + 1) * item_height - listbox->offset > h)
    {
        listbox->offset = (listbox->list.selected_index + 1) * item_height - h;
    }
    if ((listbox->list.selected_index) * item_height < listbox->offset)
    {
        listbox->offset = (listbox->list.selected_index) * item_height;
    }
    u8g2_SetClipWindow(&(fezui_ptr->u8g2), x, y, x + w, y + h);
    for (uint16_t i = 0; i < listbox->list.len; i++)
    {
        listbox->item_draw_cb(fezui_ptr, x, (u8g2_int_t)floorf(y+(item_height * (i + 1) - listbox->offset) + 0.5), w,item_height,listbox->list.items[i],i);
    }
    if (listbox->show_scrollbar)
    {
        fezui_draw_scrollbar(fezui_ptr, x + w - 5, y, 5, h, (float)h / (float)(item_height * listbox->list.len), listbox->offset / (float)(item_height * listbox->list.len - h), ORIENTATION_VERTICAL);
    }
    fezui_ptr->u8g2.font_calc_vref = fnptr_bk;
    u8g2_SetMaxClipWindow(&(fezui_ptr->u8g2));
}