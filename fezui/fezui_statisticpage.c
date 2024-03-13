/*
 * fezui_statisticpage.c
 *
 *  Created on: Jun 25, 2023
 *      Author: xq123
 */
#include "fezui.h"
#include "fezui_var.h"
#include "record.h"

#define ROW_HEIGHT 10
#define KEY_WIDTH 5

static fezui_scrollview_t scrollview =
    {
        .content_height = 100,
        .content_width = 64,
        .ordinate = 0};

static float target_ordinate = 0;

static void statisticpage_logic(void *page)
{
    CONVERGE_TO_ROUNDED(scrollview.ordinate, target_ordinate, fezui.speed);
}
static void statisticpage_draw(void *page)
{

    u8g2_SetFont(&(fezui.u8g2), u8g2_font_helvB08_tr);
    u8g2_DrawStr(&(fezui.u8g2), 0, ROW_HEIGHT * 6 - (u8g2_int_t)(scrollview.ordinate), "- System");
    u8g2_DrawStr(&(fezui.u8g2), 0, ROW_HEIGHT * 1 - (u8g2_int_t)(scrollview.ordinate), "- Total hits");

    u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x8_mr);

    u8g2_DrawStr(&(fezui.u8g2), 0, ROW_HEIGHT * 2 - (u8g2_int_t)(scrollview.ordinate), "  KEY1");

    u8g2_DrawStr(&(fezui.u8g2), 0, ROW_HEIGHT * 3 - (u8g2_int_t)(scrollview.ordinate), "  KEY2");

    u8g2_DrawStr(&(fezui.u8g2), 0, ROW_HEIGHT * 4 - (u8g2_int_t)(scrollview.ordinate), "  KEY3");

    u8g2_DrawStr(&(fezui.u8g2), 0, ROW_HEIGHT * 5 - (u8g2_int_t)(scrollview.ordinate), "  KEY4");

    u8g2_DrawStr(&(fezui.u8g2), 0, ROW_HEIGHT * 7 - (u8g2_int_t)(scrollview.ordinate), "  Run time");

    sprintf(fezui_buffer, "%02ldh%02ldm%02lds", fezui_run_time / 3600, fezui_run_time / 60 % 60, fezui_run_time % 60);
    u8g2_DrawStr(&(fezui.u8g2), WIDTH - KEY_WIDTH * 10, ROW_HEIGHT * 7 - (u8g2_int_t)(scrollview.ordinate), fezui_buffer);

    // u8g2_DrawStr(&(fezui.u8g2),0,ROW_HEIGHT*8-(u8g2_int_t)(scrollview.ordinate),"  Temperature");

    // double temp = (((( ((int32_t)(((((((fezui_temp_raw))) << ((((((0x00000000UL))))) >> (((3U)) - 1UL))) >> (((0x00000000UL)) >> (((3U)) - 1UL))) * ((3324))) / (3000.0)) - (int32_t) *((uint16_t*) (0x1FFF75A8UL)))) * (int32_t)((130L) - (30L))) / (double)((int32_t)*((uint16_t*) (0x1FFF75CAUL)) - (int32_t)*((uint16_t*)(0x1FFF75A8UL)))) + (30L));
    // sprintf(fezui_buffer,"%4.2lfC", temp);
    // sprintf(fezui_buffer,"%2ldC",__HAL_ADC_CALC_TEMPERATURE(3324,fezui_temp_raw,ADC_RESOLUTION_12B));
    // sprintf(fezui_buffer,"%d",fezui_temp_raw);
    // u8g2_DrawStr(&(fezui.u8g2),WIDTH-KEY_WIDTH*4,ROW_HEIGHT*8-(u8g2_int_t)(scrollview.ordinate),fezui_buffer);

    sprintf(fezui_buffer, "%8ld", g_key_counts[0]);
    u8g2_DrawStr(&(fezui.u8g2), WIDTH - KEY_WIDTH * 9, ROW_HEIGHT * 2 - (u8g2_int_t)(scrollview.ordinate), fezui_buffer);

    sprintf(fezui_buffer, "%8ld", g_key_counts[1]);
    u8g2_DrawStr(&(fezui.u8g2), WIDTH - KEY_WIDTH * 9, ROW_HEIGHT * 3 - (u8g2_int_t)(scrollview.ordinate), fezui_buffer);

    sprintf(fezui_buffer, "%8ld", g_key_counts[2]);
    u8g2_DrawStr(&(fezui.u8g2), WIDTH - KEY_WIDTH * 9, ROW_HEIGHT * 4 - (u8g2_int_t)(scrollview.ordinate), fezui_buffer);

    sprintf(fezui_buffer, "%8ld", g_key_counts[3]);
    u8g2_DrawStr(&(fezui.u8g2), WIDTH - KEY_WIDTH * 9, ROW_HEIGHT * 5 - (u8g2_int_t)(scrollview.ordinate), fezui_buffer);

    fezui_draw_scrollview(&fezui, 0, 0, 128, 64, &scrollview);
}

static void statisticpage_load(void *page)
{
}

static void statisticpage_event_handler(void *e)
{
    switch (*(uint16_t *)e)
    {
    case KEY_UP_ARROW:
        target_ordinate -= 10;
        if (target_ordinate < 0)
            target_ordinate = 0;
        break;
    case KEY_DOWN_ARROW:
        target_ordinate += 10;
        if (target_ordinate + 64 > scrollview.content_height)
            target_ordinate = scrollview.content_height - 64;
        break;
    case KEY_ENTER:
        fezui_link_frame_go_back(&mainframe);
        fezui_cursor_set(&cursor, 0, 0, WIDTH, HEIGHT);
        break;
    case KEY_ESC:
        fezui_link_frame_go_back(&mainframe);
        fezui_cursor_set(&cursor, 0, 0, WIDTH, HEIGHT);
        break;
    default:
        break;
    }
}

fezui_link_page_t statisticpage = {statisticpage_logic, statisticpage_draw, statisticpage_load, statisticpage_event_handler};