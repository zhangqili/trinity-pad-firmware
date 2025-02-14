/*
 * fezui_snakepage.c
 *
 *  Created on: Jun 25, 2023
 *      Author: xq123
 */
#include "fezui.h"
#include "fezui_var.h"
#include "snake.h"

static void snakepage_tick(void *page)
{

}

static void snakepage_draw(void *page)
{
    snake_move(&g_snake);
    draw_snake(&g_snake);
    u8g2_SetFont(&fezui.u8g2, u8g2_font_5x8_mr);
}

static void snakepage_load(void *page)
{
    snake_launch(&g_snake);
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