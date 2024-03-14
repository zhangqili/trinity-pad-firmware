/*
 * fezui_homepage.c
 *
 *  Created on: 2023骞�6鏈�19鏃�
 *      Author: xq123
 */

#include "main.h"
#include "record.h"
#include "fezui.h"
#include "fezui_var.h"

#define MARGIN_LEFT           65
#define PADDING_UP            0
#define MARGIN_UP             11
#define MARGIN_DOWN           12
#define HALF_WIDTH            64
#define TILE_WIDTH            10
#define CHART_HEIGHT          (HEIGHT-MARGIN_DOWN-MARGIN_UP)
#define CHART_WIDTH           64
#define TILE1                 1
#define TILE2                 11
#define TILE3                 21
#define TILE4                 31


fezui_rolling_number_t key1_num=
{
    .number=0,
    .font=fez_font_6x10_m,
    .digit=5
};

fezui_rolling_number_t key2_num=
{
    .number=0,
    .font=fez_font_6x10_m,
    .digit=5
};

fezui_rolling_number_t key3_num=
{
    .number=0,
    .font=fez_font_6x10_m,
    .digit=5
};

fezui_rolling_number_t key4_num=
{
    .number=0,
    .font=fez_font_6x10_m,
    .digit=5
};

fezui_rolling_number_t kps_num=
{
    .number=0,
    .font=fez_font_6x10_m,
    .digit=2
};

fezui_rolling_number_t max_kps_num=
{
    .number=0,
    .font=fez_font_6x10_m,
    .digit=2
};


static void homepage_logic(void *page)
{
    fezui_rolling_number_set(&key1_num,g_key_counts[0]-g_key_init_counts[0]);
    fezui_rolling_number_set(&key2_num,g_key_counts[1]-g_key_init_counts[1]);
    fezui_rolling_number_set(&key3_num,g_key_counts[2]-g_key_init_counts[2]);
    fezui_rolling_number_set(&key4_num,g_key_counts[3]-g_key_init_counts[3]);
    fezui_rolling_number_set(&kps_num,g_kps);
    fezui_rolling_number_set(&max_kps_num,KPS_history_max);

    fezui_rolling_number_update(&fezui, &key1_num);
    fezui_rolling_number_update(&fezui, &key2_num);
    fezui_rolling_number_update(&fezui, &key3_num);
    fezui_rolling_number_update(&fezui, &key4_num);
    fezui_rolling_number_update(&fezui, &kps_num);
    fezui_rolling_number_update(&fezui, &max_kps_num);

}

static void draw_chart(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w,
                      u8g2_uint_t h, LoopArray *arr, uint8_t max)
{
    float y1;
    float y2;
    float ymax = (float)max;
    for (int8_t i = 0; i < w; i++)
    {
        y1 = loop_array_get(arr, i);
        y2 = loop_array_get(arr, i + 1);

        u8g2_DrawLine(&(fezui_ptr->u8g2), x + w - i,
                      h + y - (uint8_t)((float)(h - 1) * (y1 / ymax)),
                      x + w - i - 1,
                      h + y - (uint8_t)((float)(h - 1) * (y2 / ymax)));
    }
}
static void homepage_draw(void *page)
{
    uint8_t color = u8g2_GetDrawColor(&(fezui.u8g2));

    fezui_draw_flowingwater(&(fezui),MARGIN_LEFT,TILE1+MARGIN_UP,HALF_WIDTH,TILE_WIDTH,(uint8_t*)g_bit_stream_datas[0]);
    fezui_draw_flowingwater(&(fezui),MARGIN_LEFT,TILE2+MARGIN_UP,HALF_WIDTH,TILE_WIDTH,(uint8_t*)g_bit_stream_datas[1]);
    fezui_draw_flowingwater(&(fezui),MARGIN_LEFT,TILE3+MARGIN_UP,HALF_WIDTH,TILE_WIDTH,(uint8_t*)g_bit_stream_datas[2]);
    fezui_draw_flowingwater(&(fezui),MARGIN_LEFT,TILE4+MARGIN_UP,HALF_WIDTH,TILE_WIDTH,(uint8_t*)g_bit_stream_datas[3]);

    draw_chart(&(fezui), 0, MARGIN_UP, CHART_WIDTH, CHART_HEIGHT, &g_kps_history, KPS_history_max);
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_micro_tr);
    u8g2_DrawStr(&(fezui.u8g2),0,MARGIN_UP-1,"KPS:");
    u8g2_DrawStr(&(fezui.u8g2),32,MARGIN_UP-1,"MAX:");
    u8g2_DrawStr(&(fezui.u8g2),95,MARGIN_UP-1,"FPS:");


    u8g2_SetFont(&(fezui.u8g2), fez_font_6x10_m);

    fezui_draw_rolling_number(&fezui, 2, 63, &key1_num);
    fezui_draw_rolling_number(&fezui, 34, 63, &key2_num);
    fezui_draw_rolling_number(&fezui, 66, 63, &key3_num);
    fezui_draw_rolling_number(&fezui, 98, 63, &key4_num);

#ifndef SHOW_FPS
    u8g2_DrawStr(&(fezui.u8g2),95+15,MARGIN_UP-1,fpsstr);
#endif

    fezui_draw_rolling_number(&fezui, 15, MARGIN_UP-1, &kps_num);
    fezui_draw_rolling_number(&fezui, 15+32, MARGIN_UP-1, &max_kps_num);


    u8g2_SetFont(&(fezui.u8g2), u8g2_font_6x13_tf);

    u8g2_DrawHLine(&(fezui.u8g2),0,MARGIN_UP,128);
    u8g2_DrawHLine(&(fezui.u8g2),0,CHART_HEIGHT+MARGIN_UP,128);
    u8g2_DrawVLine(&(fezui.u8g2),64,MARGIN_UP,CHART_HEIGHT+MARGIN_DOWN);
    u8g2_DrawVLine(&(fezui.u8g2),32,HEIGHT-MARGIN_DOWN,MARGIN_DOWN);
    u8g2_DrawVLine(&(fezui.u8g2),96,HEIGHT-MARGIN_DOWN,MARGIN_DOWN);

    u8g2_SetFont(&(fezui.u8g2), u8g2_font_micro_tr);
    if(Keybaord_Shift_Flag)
    {
        u8g2_DrawBox(&(fezui.u8g2), 65 , 1 ,MARGIN_UP-2, MARGIN_UP-2);
        u8g2_SetDrawColor(&(fezui.u8g2), 2);
        u8g2_DrawStr(&(fezui.u8g2), 68 ,MARGIN_UP-2,"S");

    }
    if(Keybaord_Alpha_Flag)
    {
        u8g2_DrawBox(&(fezui.u8g2), 65 + MARGIN_UP - 2, 1 ,MARGIN_UP-2, MARGIN_UP-2);
        u8g2_SetDrawColor(&(fezui.u8g2), 2);
        u8g2_DrawStr(&(fezui.u8g2), 68 + MARGIN_UP - 2 , MARGIN_UP-2,"A");
    }

    u8g2_SetDrawColor(&(fezui.u8g2), color);
}

static void homepage_load(void *page)
{
    //fezui_scrolling_text_init(&fezui,&scrolling_text,15,0.1,u8g2_font_4x6_mr,"HELLO!");
    //fezui_scrolling_text_begin_once(&scrolling_text);
    Keybaord_SendReport_Enable=true;
    Keybaord_Shift_Flag=false;
    Keybaord_Alpha_Flag=false;
}

fezui_link_page_t homepage={homepage_logic,homepage_draw,homepage_load};
