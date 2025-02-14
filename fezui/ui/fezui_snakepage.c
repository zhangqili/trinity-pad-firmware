/*
 * fezui_snakepage.c
 *
 *  Created on: Jun 25, 2023
 *      Author: xq123
 */
#include "fezui.h"
#include "fezui_var.h"
#include "snake.h"


static fezui_rolling_number_t score_num=
{
    .number=0,
    .font=u8g2_font_5x8_mr,
    .digit=3
};

static fezui_rolling_number_t speed_interval_num=
{
    .number=0,
    .font=u8g2_font_5x8_mr,
    .digit=4
};


static fezui_animation_base_t open_animation =
{
    .easing_func = fezui_animation_elastic_ease,
    .mode = EASE_OUT,
    .duration = DEFAULT_ANIMATION_DURATION,
    .parameter1 = 1,
    .parameter2 = 3,
};

static fezui_animation_base_t close_animation =
{
    .easing_func = fezui_animation_bounce_ease,
    .mode = EASE_OUT,
    .duration = DEFAULT_ANIMATION_DURATION,
    .parameter1 = 3,
    .parameter2 = 3,
};

static void snakepage_tick(void *page)
{

    fezui_rolling_number_set(&score_num, (g_snake.rear + g_snake.len - g_snake.front)%g_snake.len);
    fezui_rolling_number_set(&speed_interval_num, g_snake.interval);

    fezui_rolling_number_update(&fezui, &score_num);
    fezui_rolling_number_update(&fezui, &speed_interval_num);
}

static void snakepage_draw(void *page)
{
    //u8g2_drawLine(&fezui.u8g2);
    //u8g2_DrawVLine(&fezui.u8g2,64,0,16);
    fezui_animation_calculate(&open_animation);
    fezui_animation_calculate(&close_animation);
    u8g2_DrawHLine(&fezui.u8g2,0,8*open_animation.value,128);
    snake_move(&g_snake);
    draw_snake(&g_snake);
    u8g2_SetFont(&fezui.u8g2, u8g2_font_open_iconic_arrow_1x_t);
    switch (g_snake.next_direction)
    {
    case SNAKE_LEFT:
        u8g2_DrawStr(&fezui.u8g2,WIDTH-8,8*open_animation.value,"\x41");
        break;
    
    case SNAKE_UP:
        u8g2_DrawStr(&fezui.u8g2,WIDTH-8,8*open_animation.value,"\x43");
        break;
    
    case SNAKE_RIGHT:
        u8g2_DrawStr(&fezui.u8g2,WIDTH-8,8*open_animation.value,"\x42");
        break;
    
    case SNAKE_DOWN:
        u8g2_DrawStr(&fezui.u8g2,WIDTH-8,8*open_animation.value,"\x40");
        break;
    default:
        break;
    }
    u8g2_SetFont(&fezui.u8g2, u8g2_font_5x8_mr);
    fezui_printf(&fezui,0,8*open_animation.value,"Score:");
    fezui_draw_rolling_number(&fezui, 30, 8*open_animation.value, &score_num);
    fezui_printf(&fezui,48,8*open_animation.value,"Interval:");
    fezui_draw_rolling_number(&fezui, 94, 8*open_animation.value, &speed_interval_num);

    u8g2_SetFont(&fezui.u8g2, u8g2_font_open_iconic_arrow_2x_t);
#define PANEL_HEIGHT 24
    u8g2_SetDrawColor(&fezui.u8g2,0);
    u8g2_DrawBox(&fezui.u8g2,0*WIDTH/4,HEIGHT-PANEL_HEIGHT*open_animation.value+PANEL_HEIGHT*close_animation.value,WIDTH,HEIGHT);
    u8g2_SetDrawColor(&fezui.u8g2,2);
    u8g2_DrawHLine(&fezui.u8g2,0*WIDTH/4,HEIGHT-PANEL_HEIGHT*open_animation.value+PANEL_HEIGHT*close_animation.value,WIDTH);
    u8g2_DrawStr(&fezui.u8g2,0*WIDTH/4+8,HEIGHT+PANEL_HEIGHT-4-PANEL_HEIGHT*open_animation.value+PANEL_HEIGHT*close_animation.value,"\x51");
    u8g2_DrawStr(&fezui.u8g2,1*WIDTH/4+8,HEIGHT+PANEL_HEIGHT-4-PANEL_HEIGHT*open_animation.value+PANEL_HEIGHT*close_animation.value,"\x50");
    u8g2_DrawStr(&fezui.u8g2,2*WIDTH/4+8,HEIGHT+PANEL_HEIGHT-4-PANEL_HEIGHT*open_animation.value+PANEL_HEIGHT*close_animation.value,"\x53");
    u8g2_DrawStr(&fezui.u8g2,3*WIDTH/4+8,HEIGHT+PANEL_HEIGHT-4-PANEL_HEIGHT*open_animation.value+PANEL_HEIGHT*close_animation.value,"\x52");
}

static void snakepage_load(void *page)
{
    snake_launch(&g_snake);
    fezui_animation_begin(&open_animation,500);
    fezui_animation_begin(&close_animation,1500);
}

static void snakepage_event_handler(void *e)
{
    switch (*(uint16_t *)e)
    {
    case KEY_UP_ARROW:
        break;
    case KEY_DOWN_ARROW:
        break;
    case KEY_SPACEBAR:
    case KEY_ENTER:
        break;
    case KEY_ESC:
        fezui_frame_go_back(&g_mainframe);
        fezui_cursor_set(&g_fezui_cursor, 0, 0, WIDTH, HEIGHT);
        break;
    default:
        break;
    }
}

fezui_page_t snakepage = {snakepage_tick, snakepage_draw, snakepage_load, snakepage_event_handler};