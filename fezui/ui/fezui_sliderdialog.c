#include "fezui.h"
#include "fezui_var.h"

static const char *dialogtitle;
static fezui_rangebase_t rangebase;
static float offset=HEIGHT+1;
//static fezui_animation_base_t frame_animation = {.duration = DEFAULT_ANIMATION_DURATION, .easing_func = fezui_animation_linear_ease, .mode = EASE_INOUT, .duration = DEFAULT_ANIMATION_DURATION / 2};
void sliderdialog_show(void *target, uint8_t type, float min, float max, float interval, const char *title)
{
    rangebase.target = target;
    rangebase.type = type;
    rangebase.min = min;
    rangebase.max = max;
    rangebase.interval = interval;
    dialogtitle = title;
}

void sliderdialog_close()
{
}

static void sliderdialog_tick(void *page)
{
    CONVERGE_TO_ROUNDED(offset,g_mainframe.dialog_state?(HEIGHT/2):(HEIGHT),fezui.speed);
}

static void sliderdialog_draw(void *page)
{
    switch (fezui.lang)
    {
    case LANG_EN:
        u8g2_SetFont(&(fezui.u8g2), u8g2_font_6x13_mr);
        break;
    case LANG_ZH:
        u8g2_SetFont(&(fezui.u8g2), u8g2_font_wqy13_t_gb2312a);
        break;
    default:
        break;
    }
    fezui_veil(&fezui, 0, 0, WIDTH, HEIGHT, (HEIGHT - offset)/7, 0);
    uint8_t color_backup = u8g2_GetDrawColor(&(fezui.u8g2));
    u8g2_SetDrawColor(&(fezui.u8g2), 0);
    u8g2_DrawBox(&(fezui.u8g2), 0, offset, WIDTH, HEIGHT / 2);
    u8g2_SetDrawColor(&(fezui.u8g2), color_backup);
    //u8g2_SetDrawColor(&(fezui.u8g2), fezui.invert ? 0 : 1);
    u8g2_DrawHLine(&(fezui.u8g2), 0, offset, WIDTH);

    uint8_t char_height = u8g2_GetMaxCharHeight(&fezui.u8g2);
    float temp = fezui_generics_convert_to_float(rangebase.target, rangebase.type);

    u8g2_DrawUTF8(&fezui.u8g2, 0 + 1, offset + char_height, dialogtitle);

    switch (rangebase.type)
    {
    case FEZUI_TYPE_FLOAT:
    case FEZUI_TYPE_DOUBLE:
        sprintf(g_fezui_printf_buffer, "%.1f", temp);
        break;
    case FEZUI_TYPE_BOOL:
        sprintf(g_fezui_printf_buffer, "%s", temp ? "ON" : "OFF");
        break;
    default:
        sprintf(g_fezui_printf_buffer, "%.0f", temp);
        break;
    }
    fezui_printf_right_aligned(&fezui, WIDTH, offset + char_height, g_fezui_printf_buffer);
    fezui_draw_slider(&fezui, 2, offset + HEIGHT / 4, WIDTH - 4, 5, &rangebase, ORIENTATION_HORIZAIONTAL);
}

static void sliderdialog_load(void *page)
{

}

static void sliderdialog_event_handler(void *e)
{
    switch (*(uint16_t *)e)
    {
    case KEY_UP_ARROW:
        fezui_rangebase_increase(&rangebase,1);
        break;
    case KEY_DOWN_ARROW:
        fezui_rangebase_increase(&rangebase,-1);
        break;
    case KEY_ENTER:
        fezui_frame_close_dialog(&g_mainframe);
        break;
    case KEY_ESC:
        fezui_frame_close_dialog(&g_mainframe);
        break;
    default:
        break;
    }
}

fezui_page_t sliderdialog = {sliderdialog_tick, sliderdialog_draw, sliderdialog_load, sliderdialog_event_handler};