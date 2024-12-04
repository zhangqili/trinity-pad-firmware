#include "fezui.h"

void fezui_draw_notification(fezui_t *fezui_ptr, fezui_notification_t *n)
{
    if (n->offset > 0.5)
    {
        uint8_t color = u8g2_GetDrawColor(&fezui_ptr->u8g2);
        u8g2_SetDrawColor(&(fezui_ptr->u8g2), 0);
        u8g2_DrawBox(&(fezui_ptr->u8g2), 0, 0, WIDTH, n->offset);
        u8g2_SetDrawColor(&(fezui_ptr->u8g2), color);
        u8g2_DrawHLine(&fezui_ptr->u8g2, 0, floorf(n->offset), WIDTH);
        fezui_draw_scrolling_text(fezui_ptr, 0, floorf(n->offset - 1), &(n->text));
    }
}
void fezui_notification_begin(fezui_t *fezui_ptr, fezui_notification_t *n, const char *str, u8g2_int_t time, float speed)
{
    n->offset = 0;
    n->time = time;
    fezui_scrolling_text_init(fezui_ptr, &n->text, WIDTH, speed, u8g2_font_6x12_tf, str);
    fezui_scrolling_text_begin_once(&n->text);
}
void fezui_notification_update(fezui_t *fezui_ptr, fezui_notification_t *n)
{
    switch (n->text.status)
    {
    case SCROLLING_TEXT_PLAYING_ONCE:
        if (n->time)
        {
            n->time--;
        }
        CONVERGE_TO(n->offset, 12, fezui_ptr->speed);
        fezui_scrolling_text_update(&n->text);
        break;
    case SCROLLING_TEXT_PAUSE:
        if (n->time)
        {
            CONVERGE_TO(n->offset, 12, fezui_ptr->speed);
            fezui_scrolling_text_update(&n->text);
            n->time--;
            break;
        }
        CONVERGE_TO(n->offset, 0, fezui_ptr->speed);
        break;
    default:
        break;
    }
}

void fezui_draw_messagebox(fezui_t *fezui_ptr, uint8_t x, uint8_t y, uint8_t w, uint8_t h, fezui_messagebox_t *messagebox)
{
    uint8_t color_backup = u8g2_GetDrawColor(&(fezui_ptr->u8g2));
    u8g2_SetDrawColor(&(fezui_ptr->u8g2), 1);
    u8g2_DrawBox(&(fezui_ptr->u8g2), x, y, w, h);

    u8g2_SetDrawColor(&(fezui_ptr->u8g2), 0);
    u8g2_DrawFrame(&(fezui_ptr->u8g2), x, y, w, h);

    u8g2_SetDrawColor(&(fezui_ptr->u8g2), color_backup);
}

void fezui_numberic_dialog_init(fezui_numberic_dialog_t *dialog, void *target, uint8_t type, float min, float max, float interval, char *title)
{
    fezui_rangebase_init(&dialog->rangebase, target, type, min, max, interval);
    dialog->title = title;
}

void fezui_numberic_dialog_increase(fezui_numberic_dialog_t *dialog, int8_t n)
{
    fezui_rangebase_increase(&dialog->rangebase, n);
}

void fezui_draw_numberic_dialog(fezui_t *fezui_ptr, u8g2_int_t x, u8g2_int_t y, u8g2_int_t w, u8g2_int_t h, fezui_numberic_dialog_t *dialog)
{
    uint8_t color_backup = u8g2_GetDrawColor(&(fezui_ptr->u8g2));
    u8g2_SetDrawColor(&(fezui_ptr->u8g2), fezui_ptr->invert?1:0);
    u8g2_DrawBox(&(fezui_ptr->u8g2), x, y, w, h);
    u8g2_SetDrawColor(&(fezui_ptr->u8g2), fezui_ptr->invert?0:1);
    u8g2_DrawFrame(&(fezui_ptr->u8g2), x, y, w, h);

    uint8_t char_height = u8g2_GetMaxCharHeight(&fezui_ptr->u8g2);
    float temp = fezui_generics_convert_to_float(dialog->rangebase.target,dialog->rangebase.type);

    u8g2_DrawUTF8(&fezui_ptr->u8g2,x+2,y+char_height,dialog->title);

    switch (dialog->rangebase.type)
    {
        case FEZUI_TYPE_FLOAT:
        case FEZUI_TYPE_DOUBLE:
            sprintf(g_fezui_printf_buffer,"%.1f",temp);
            break;
        case FEZUI_TYPE_BOOL:
            sprintf(g_fezui_printf_buffer,"%s",temp?"ON":"OFF");
            break;
        default:
            sprintf(g_fezui_printf_buffer,"%.0f",temp);
            break;
    }
    uint8_t width = u8g2_GetStrWidth(&fezui_ptr->u8g2,g_fezui_printf_buffer);
    u8g2_DrawUTF8(&fezui_ptr->u8g2,x+w-2-width,y+char_height,g_fezui_printf_buffer);

    fezui_draw_slider(fezui_ptr, x + 2, y + h - 7, w - 4, 5, &dialog->rangebase, ORIENTATION_HORIZAIONTAL);

    u8g2_SetDrawColor(&(fezui_ptr->u8g2), color_backup);
}

void fezui_flyout_numberic_dialog_init(fezui_flyout_numberic_dialog_t *dialog, void *target, uint8_t type, float min, float max, float interval, char *title)
{
    fezui_numberic_dialog_init(&dialog->dialog, target, type, min, max, interval, title);
}
void fezui_flyout_numberic_dialog_update(fezui_t *fezui_ptr, fezui_flyout_numberic_dialog_t *dialog)
{
    if(dialog->show)
    {
        CONVERGE_TO(dialog->animation,1,fezui_ptr->speed);
    }
    else
    {
        CONVERGE_TO(dialog->animation,0,fezui_ptr->speed);
    }
}
void fezui_draw_flyout_numberic_dialog(fezui_t *fezui_ptr, fezui_flyout_numberic_dialog_t *dialog)
{
    if(dialog->animation>0.01)
    {
        uint8_t height = u8g2_GetMaxCharHeight(&fezui_ptr->u8g2) + 8;
        fezui_draw_numberic_dialog(fezui_ptr, WIDTH/6, -height + floorf(dialog->animation*(height+(HEIGHT-height)/2)), WIDTH/3*2, height, &dialog->dialog);
    }
}