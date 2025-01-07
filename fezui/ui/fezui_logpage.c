/*
 * fezui_logpage.c
 *
 *  Created on: 2023骞�6鏈�19鏃�
 *      Author: xq123
 */

#include "fezui.h"
#include "keyboard.h"
#include "fezui_var.h"

static fezui_scrollview_t scrollview;
static float target_ordinate = 0;
static const uint8_t charHeight = 7;

static void logpage_tick(void *page)
{
    CONVERGE_TO_ROUNDED(scrollview.ordinate, target_ordinate, fezui.speed);
}

static void logpage_draw(void *page)
{
    u8g2_SetFont(&fezui.u8g2, u8g2_font_5x7_tf);
    uint16_t h = (g_u8log.cursor_y*charHeight);
    scrollview.content_height = h;
    fezui_draw_scrollview(&fezui,0,0,WIDTH,HEIGHT,&scrollview);
    u8g2_DrawLog(&fezui.u8g2, 0, (u8g2_int_t)(charHeight+1-scrollview.ordinate), &g_u8log);
    
}

static void logpage_load(void *page)
{
}

static void logpage_event_handler(void *e)
{
    switch (*(uint16_t *)e)
    {
    case KEY_UP_ARROW:
        target_ordinate -= charHeight;
        if (target_ordinate < 0)
            target_ordinate = 0;
        break;
    case KEY_DOWN_ARROW:
        target_ordinate += charHeight;
        if (target_ordinate + 64 > scrollview.content_height)
            target_ordinate = scrollview.content_height - 64;
        break;
    case KEY_SPACEBAR:
    case KEY_ENTER:
        fezui_frame_go_back(&g_mainframe);
        break;
    case KEY_ESC:
        fezui_frame_go_back(&g_mainframe);
        break;
    default:
        break;
    }
}

fezui_page_t logpage={logpage_tick,logpage_draw,logpage_load,logpage_event_handler};
