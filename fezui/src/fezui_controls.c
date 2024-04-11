/*
 * fez_ui_controls.c
 *
 *  Created on: 2023年6月19日
 *      Author: xq123
 */

#include "fezui.h"


void fezui_draw_flowingwater(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w,
                             u8g2_uint_t h, uint8_t *bits)
{
    for (uint8_t i = 0; i < w; i++)
    {
        if (bits[i/8]&BIT(i%8))
            u8g2_DrawVLine(&(fezui_ptr->u8g2), x + i, y + 1, h - 2);
        if (((bool)(bits[i/8]&BIT(i%8))) ^ ((bool)(bits[(i+1)/8]&BIT((i+1)%8))))
            u8g2_DrawVLine(&(fezui_ptr->u8g2), x + i, y, h);
        
    }
}

void fezui_veil(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w,
                u8g2_uint_t h, uint8_t level, uint8_t color)
{
    uint8_t color_backup;
    color_backup = u8g2_GetDrawColor(&(fezui_ptr->u8g2));
    u8g2_SetDrawColor(&(fezui_ptr->u8g2), color);
    switch (level)
    {
    case 0:
        break;
    case 2:
        for (u8g2_uint_t i = y + 2; i < y + h; i += 4)
        {
            for (u8g2_uint_t j = x + 2; j < x + w; j += 4)
            {
                u8g2_DrawPixel(&(fezui_ptr->u8g2), j, i);
            }
        }
    case 1:
        for (u8g2_uint_t i = y; i < y + h; i += 4)
        {
            for (u8g2_uint_t j = x; j < x + w; j += 4)
            {
                u8g2_DrawPixel(&(fezui_ptr->u8g2), j, i);
            }
        }
        break;
    case 6:
        for (u8g2_uint_t i = y; i < y + h; i += 2)
        {
            for (u8g2_uint_t j = x + 1; j < x + w; j += 2)
            {
                u8g2_DrawPixel(&(fezui_ptr->u8g2), j, i);
            }
        }
    case 5:
        for (u8g2_uint_t i = y + 1; i < y + h; i += 2)
        {
            for (u8g2_uint_t j = x; j < x + w; j += 2)
            {
                u8g2_DrawPixel(&(fezui_ptr->u8g2), j, i);
            }
        }
    case 4:
        for (u8g2_uint_t i = y + 1; i < y + h; i += 2)
        {
            for (u8g2_uint_t j = x + 1; j < x + w; j += 2)
            {
                u8g2_DrawPixel(&(fezui_ptr->u8g2), j, i);
            }
        }
    case 3:
        for (u8g2_uint_t i = y; i < y + h; i += 2)
        {
            for (u8g2_uint_t j = x; j < x + w; j += 2)
            {
                u8g2_DrawPixel(&(fezui_ptr->u8g2), j, i);
            }
        }
        break;
    case 7:
        u8g2_DrawBox(&(fezui_ptr->u8g2), x, y, w, h);
        break;
    default:
        break;
    }
    u8g2_SetDrawColor(&(fezui_ptr->u8g2), color_backup);
}

void fezui_veil_full_screen(fezui_t *fezui_ptr, uint8_t level)
{
    uint8_t*p = fezui_ptr->u8g2.tile_buf_ptr;
    uint8_t w = WIDTH;
    uint8_t h = HEIGHT/8;
    switch (level)
    {
    case 0:
        break;
    case 1:
        for (u8g2_uint_t i = 0; i < w; i += 4)
        {
            for (u8g2_uint_t j = 0; j < h; j++)
            {
                *(p+j*w+i)&=(~0x11);
            }
        }
        break;
    case 2:
        for (u8g2_uint_t i = 0; i < w; i += 2)
        {
            for (u8g2_uint_t j = 0; j < h; j++)
            {
                *(p+j*w+i)&=(i%4)?(~0x44):(~0x11);
            }
        }
        break;
    case 3:
        for (u8g2_uint_t i = 0; i < w; i += 2)
        {
            for (u8g2_uint_t j = 0; j < h; j++)
            {
                *(p+j*w+i)&=(~0x55);
            }
        }
        break;
    case 4:
        for (u8g2_uint_t i = 0; i < w; i ++)
        {
            for (u8g2_uint_t j = 0; j < h; j++)
            {
                *(p+j*w+i)&=(i%2)?(~0xAA):(~0x55);
            }
        }
        break;
    case 5:
        for (u8g2_uint_t i = 0; i < w; i ++)
        {
            for (u8g2_uint_t j = 0; j < h; j++)
            {
                *(p+j*w+i)&=(i%2)?(~0xAA):(~0xFF);
            }
        }
        break;
    case 6:
    case 7:
        for (u8g2_uint_t i = 0; i < w; i ++)
        {
            for (u8g2_uint_t j = 0; j < h; j++)
            {
                *(p+j*w+i)&=0;
            }
        }
        break;
    default:
        break;
    }
}

void fezui_draw_scrollbar(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, float size, float value, fezui_orientation_t o)
{
    if (o == ORIENTATION_HORIZAIONTAL)
    {
        u8g2_DrawHLine(&(fezui_ptr->u8g2), x, y + 2, w);
        u8g2_DrawVLine(&(fezui_ptr->u8g2), x, y, h);
        u8g2_DrawVLine(&(fezui_ptr->u8g2), x + w - 1, y, h);
        u8g2_DrawBox(&(fezui_ptr->u8g2), x + ROUND((w - w * size) * value), y, w * size, h);
    }
    else
    {
        u8g2_DrawVLine(&(fezui_ptr->u8g2), x + 2, y, h);
        u8g2_DrawHLine(&(fezui_ptr->u8g2), x, y, w);
        u8g2_DrawHLine(&(fezui_ptr->u8g2), x, y + h - 1, w);
        u8g2_DrawBox(&(fezui_ptr->u8g2), x, y + ROUND((h - h * size) * value), w, h * size);
    }
}



#define TYPE_GENERIC_INCREASE(rangebase,type,n) \
if (n>0)\
{\
    if ((type)(r->max) - *(type*)(rangebase)->target > (type)((n)*(rangebase)->interval))\
        *(type*)(rangebase)->target+=(type)((n)*(rangebase)->interval);\
    else\
        *(type*)(rangebase)->target = r->max;\
}\
else\
{\
    if (*(type*)(rangebase)->target - (type)(r->min) > (type)((n)*(rangebase)->interval))\
        *(type*)(rangebase)->target+=(type)((n)*(rangebase)->interval);\
    else\
        *(type*)(rangebase)->target = r->min;\
}\

#define TYPE_GENERIC_UNSIGNED_INCREASE(rangebase,type,n)\
if (n>0)\
{\
    if ((u##type)(r->max) - *(u##type*)(rangebase)->target > (u##type)((n)*(rangebase)->interval))\
        *(u##type*)(rangebase)->target+=(type)((n)*(rangebase)->interval);\
    else\
        *(u##type*)(rangebase)->target = r->max;\
}\
else\
{\
    if (*(u##type*)(rangebase)->target - (u##type)(r->min) > (u##type)((n)*(rangebase)->interval))\
        *(u##type*)(rangebase)->target+=(type)((n)*(rangebase)->interval);\
    else\
        *(u##type*)(rangebase)->target = r->min;\
}\

void fezui_rangebase_increase(fezui_rangebase_t *r, int8_t n)
{
    switch (r->type)
    {
    case FEZUI_TYPE_FLOAT:
        TYPE_GENERIC_INCREASE(r,float,n);
        break;
    case FEZUI_TYPE_DOUBLE:
        TYPE_GENERIC_INCREASE(r,double,n);
        break;
    case FEZUI_TYPE_INT16:
        TYPE_GENERIC_INCREASE(r,int16_t,n);
        break;
    case FEZUI_TYPE_INT32:
        TYPE_GENERIC_INCREASE(r,int32_t,n);
        break;
    case FEZUI_TYPE_UINT16:
        TYPE_GENERIC_UNSIGNED_INCREASE(r,int16_t,n);
        break;
    case FEZUI_TYPE_UINT32:
        TYPE_GENERIC_UNSIGNED_INCREASE(r,int32_t,n);
        break;
    case FEZUI_TYPE_UINT8:
        TYPE_GENERIC_UNSIGNED_INCREASE(r,int8_t,n);
        break;
    case FEZUI_TYPE_BOOL:
        *(bool*)(r)->target=!(*(bool*)(r)->target);
        break;
    default:
        break;
    }
}
void fezui_rangebase_init(fezui_rangebase_t *r, void *target, uint8_t type, float min, float max, float interval)
{
    r->target=target;
    r->max=max;
    r->min=min;
    r->interval=fabsf(interval);
    r->type=type;
}

void fezui_draw_slider(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, fezui_rangebase_t *r, fezui_orientation_t orientation)
{
    float temp=fezui_generics_convert_to_float(r->target,r->type);
    if (orientation == ORIENTATION_HORIZAIONTAL)
    {
        u8g2_DrawHLine(&(fezui_ptr->u8g2), x + 1, y + h / 2, w - 2);
        u8g2_DrawBox(&(fezui_ptr->u8g2), x + ((temp - r->min) / (r->max - r->min)) * (w - 3), y, 3, h);
    }
    else
    {
        u8g2_DrawVLine(&(fezui_ptr->u8g2), x + w / 2, y + 1, h - 2);
        u8g2_DrawBox(&(fezui_ptr->u8g2), x, y + ((r->max - temp) / (r->max - r->min)) * (h - 3), w, 3);
    }
}

void fezui_draw_rolling_number(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, fezui_rolling_number_t *rolling_number)
{
    const uint8_t *temp_font = (fezui_ptr->u8g2).font;
    u8g2_SetFont(&(fezui_ptr->u8g2), rolling_number->font);
    u8g2_font_calc_vref_fnptr fnptr_bk = fezui_ptr->u8g2.font_calc_vref;
    u8g2_SetFontPosBottom(&(fezui_ptr->u8g2));
    uint8_t font_height = u8g2_GetMaxCharHeight(&(fezui_ptr->u8g2));
    uint8_t font_width = u8g2_GetMaxCharWidth(&(fezui_ptr->u8g2));
    u8g2_SetClipWindow(&(fezui_ptr->u8g2), x, y - font_height, x + rolling_number->digit * font_width, y + 1);
    for (uint8_t i = 0; i < rolling_number->digit; i++)
    {
        for (uint8_t j = 0; j < 10; j++)
        {
            fezui_printf(fezui_ptr, x + (rolling_number->digit - i - 1) * font_width, y + j * font_height - (u8g2_int_t)rolling_number->offsets[i], "%d",j);
        }
    }
    u8g2_SetMaxClipWindow(&(fezui_ptr->u8g2));
    u8g2_SetFont(&(fezui_ptr->u8g2), temp_font);
    fezui_ptr->u8g2.font_calc_vref = fnptr_bk;
}

void fezui_rolling_number_update(fezui_t *fezui_ptr, fezui_rolling_number_t *rolling_number)
{
    const uint8_t *temp_font = (fezui_ptr->u8g2).font;
    u8g2_SetFont(&(fezui_ptr->u8g2), rolling_number->font);
    u8g2_long_t num = rolling_number->number;
    CONVERGE_TO_ROUNDED(rolling_number->offsets[0], num % 10 * u8g2_GetMaxCharHeight(&(fezui_ptr->u8g2)), fezui_ptr->speed);
    for (uint8_t i = 1; i < rolling_number->digit; i++)
    {
        if (num /= 10)
        {
            CONVERGE_TO_ROUNDED(rolling_number->offsets[i], num % 10 * (u8g2_GetMaxCharHeight(&(fezui_ptr->u8g2))), fezui_ptr->speed);
        }
        else
        {
            CONVERGE_TO_ROUNDED(rolling_number->offsets[i], -1 - (u8g2_GetMaxCharHeight(&(fezui_ptr->u8g2))), fezui_ptr->speed);
        }
    }
    u8g2_SetFont(&(fezui_ptr->u8g2), temp_font);
}

void fezui_draw_progressbar(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, fezui_progressbar_t *bar)
{
    u8g2_DrawFrame(&(fezui_ptr->u8g2), x, y, w, h);
    u8g2_int_t length;
    if (bar->orientation == ORIENTATION_HORIZAIONTAL)
    {
        length = (u8g2_int_t)ROUND((bar->value - bar->min) / (bar->max - bar->min) * w);
        if (length > w)
            length = w;
        if (length < 0)
            length = 0;
        u8g2_DrawBox(&(fezui_ptr->u8g2), x, y, length, h);
    }
    else
    {
        length = (u8g2_int_t)ROUND((bar->value - bar->min) / (bar->max - bar->min) * h);
        if (length > h)
            length = h;
        if (length < 0)
            length = 0;
        u8g2_DrawBox(&(fezui_ptr->u8g2), x, y + h - length, w, length);
    }
}

void fezui_progressbar_update(fezui_t *fezui_ptr, fezui_progressbar_t *bar)
{
}

void fezui_draw_scrolling_text(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, fezui_scrolling_text_t *text)
{
    const uint8_t *temp_font = (fezui_ptr->u8g2).font;
    u8g2_SetFont(&(fezui_ptr->u8g2), text->font);
    // uint8_t font_height = u8g2_GetMaxCharHeight(&(fezui_ptr->u8g2));
    u8g2_SetClipWindow(&(fezui_ptr->u8g2), x, 0, x + text->width, y + 1);
    u8g2_DrawStr(&(fezui_ptr->u8g2), x + (u8g2_int_t)(text->offset), y, text->text);
    u8g2_SetMaxClipWindow(&(fezui_ptr->u8g2));
    u8g2_SetFont(&(fezui_ptr->u8g2), temp_font);
}

void fezui_scrolling_text_update(fezui_scrolling_text_t *text)
{
    switch (text->status)
    {
    case SCROLLING_TEXT_PAUSE:
        break;
    case SCROLLING_TEXT_PLAYING:
        if (text->content_width < text->width)
        {
            text->offset = 0;
            text->status = SCROLLING_TEXT_PAUSE;
        }
        else
        {
            text->offset -= text->speed;
            if (text->offset < -text->content_width)
                text->offset = text->width;
        }
        break;
    case SCROLLING_TEXT_PLAYING_ONCE:
        if (text->content_width < text->width)
        {
            text->offset = 0;
            text->status = SCROLLING_TEXT_PAUSE;
        }
        else
        {
            text->offset -= text->speed;
            if (text->offset < -text->content_width)
                text->status = SCROLLING_TEXT_PAUSE;
        }
        break;

    default:
        break;
    }
}

void u8g2_read_font_info(u8g2_font_info_t *font_info, const uint8_t *font);
void fezui_scrolling_text_init(fezui_t *fezui_ptr, fezui_scrolling_text_t *text, u8g2_int_t w, float speed, const uint8_t *font, const char *str)
{
    // uint8_t *temp_font=(fezui_ptr->u8g2).font;
    // u8g2_SetFont(&(fezui_ptr->u8g2), text->font);
    u8g2_font_info_t font_info;
    text->text = str;
    text->width = w;
    text->font = font;
    text->speed = speed;
    u8g2_read_font_info(&font_info, font);
    text->content_width = strlen(str) * font_info.max_char_width;
    // text->content_width = u8g2_GetStrWidth(&(fezui_ptr->u8g2),str);
    text->status = SCROLLING_TEXT_PAUSE;
    // u8g2_SetFont(&(fezui_ptr->u8g2), temp_font);
}
void fezui_scrolling_text_begin(fezui_scrolling_text_t *text)
{
    text->status = SCROLLING_TEXT_PLAYING;
    text->offset = text->width;
}
void fezui_scrolling_text_begin_once(fezui_scrolling_text_t *text)
{
    text->status = SCROLLING_TEXT_PLAYING_ONCE;
    text->offset = text->width;
}
