/*
 * fezui_launcherpage.c
 *
 *  Created on: 2023骞�6鏈�19鏃�
 *      Author: xq123
 */

#include "fezui.h"
#include "keyboard.h"
#include "fezui_var.h"

#define LEFT 16
#define TOP 16
#define TILE_HEIGHT 32
#define TILE_WIDTH 64
#define MARGIN 4

#define DRAW_IN_TEMP_BUFFER(code) {memset(tile_buffer, 0, sizeof(tile_buffer));\
            uint8_t *buf_bak = fezui.u8g2.tile_buf_ptr;\
            fezui.u8g2.tile_buf_ptr = tile_buffer;\
            code\
            fezui.u8g2.tile_buf_ptr = buf_bak;\
            }

static enum {
    TILE_NONE,
    TILE_SHOWING,
    TILE_OPENING,
    TILE_CLOSING
} state;


static void main_menu_cb(void *menu);
typedef struct
{
    float x;
    float y;
    float z;
} Point;

typedef struct
{
    Point centerFrom;
    Point centerTo;
    float widthFrom;
    float widthTo;
    float heightFrom;
    float heightTo;
    float angleTo;
    uint8_t * icon;
    fezui_animation_base_t animation;
} Frame;

static fezui_animated_listbox_t mainmenu;
static const char *mainmenu_items[4][LANG_NUM] =
    {
        {"Home", "主页"},
        {"Oscilloscope", "示波器"},
        {"Statistic", "统计数据"},
        {"Settings", "设置"}};
static bool isPageOpen = true;
static Frame frames[4];
static fezui_animation_base_t open_animation;
static fezui_animation_base_t open_menu_animation;
static fezui_animation_base_t icon_animation;

static float offset_y;
static fezui_page_t *pages[] = {&homepage, &oscilloscopepage, &statisticpage, &settingspage};
static uint8_t tile_buffer[1024];

static const unsigned char home_bitmap_e80f [] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x80, 0x01, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xE0, 0x07, 0x00, 
  0x00, 0x70, 0x0E, 0x00, 0x00, 0x38, 0x1C, 0x00, 0x00, 0x1C, 0x38, 0x00, 
  0x00, 0x0E, 0x70, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x80, 0x03, 0xC0, 0x01, 
  0xC0, 0x01, 0x80, 0x03, 0xE0, 0x00, 0x00, 0x07, 0x70, 0x00, 0x00, 0x0E, 
  0x38, 0x00, 0x00, 0x1C, 0x3C, 0x00, 0x00, 0x3C, 0x3C, 0x00, 0x00, 0x3C, 
  0x30, 0xF0, 0x0F, 0x0C, 0x30, 0xF0, 0x0F, 0x0C, 0x30, 0x30, 0x0C, 0x0C, 
  0x30, 0x30, 0x0C, 0x0C, 0x30, 0x30, 0x0C, 0x0C, 0x30, 0x30, 0x0C, 0x0C, 
  0x30, 0x30, 0x0C, 0x0C, 0x30, 0x30, 0x0C, 0x0C, 0x30, 0x30, 0x0C, 0x0C, 
  0x30, 0x30, 0x0C, 0x0C, 0xF0, 0x3F, 0xFC, 0x0F, 0xF0, 0x3F, 0xFC, 0x0F, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static const unsigned char oscilloscope_bitmap_e80f [] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 
  0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 
  0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0xC0, 0x00, 
  0x0C, 0x04, 0xE0, 0x01, 0x0C, 0x1E, 0xF0, 0x03, 0x8C, 0x7F, 0x38, 0x07, 
  0xCC, 0xE1, 0x1D, 0x0E, 0xCC, 0x80, 0x0F, 0x1C, 0xCC, 0x00, 0x06, 0x38, 
  0xCC, 0x00, 0x00, 0x30, 0xCC, 0x00, 0xC0, 0x30, 0xCC, 0x00, 0xF0, 0x31, 
  0xCC, 0x00, 0xBC, 0x33, 0xCC, 0x00, 0x1F, 0x37, 0xCC, 0xFF, 0x07, 0x3E, 
  0xCC, 0xFF, 0x00, 0x3C, 0xCC, 0x00, 0x00, 0x38, 0xCC, 0x00, 0x00, 0x30, 
  0xCC, 0xFF, 0xFF, 0x3F, 0xCC, 0xFF, 0xFF, 0x3F, 0x0C, 0x00, 0x00, 0x00, 
  0x0C, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x3F, 0xFC, 0xFF, 0xFF, 0x3F, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};
static const unsigned char statistic_bitmap_e80f [] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xFF, 0xFF, 0xFF, 0x03, 0xFF, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 
  0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x0F, 0x00, 0xFF, 0xFF, 0x0F, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};
static const unsigned char setting_bitmap_e80f [] U8X8_PROGMEM = {
  0x00, 0x10, 0x08, 0x00, 0x00, 0x3E, 0x3C, 0x00, 0x80, 0x7F, 0xFC, 0x01, 
  0x80, 0x61, 0xC6, 0x01, 0x80, 0xC1, 0x83, 0x01, 0x00, 0xC1, 0xC3, 0x00, 
  0x00, 0x03, 0xC0, 0x00, 0x38, 0x03, 0xC0, 0x39, 0xFC, 0x03, 0x80, 0x3F, 
  0xCC, 0x00, 0x00, 0x73, 0x06, 0xC0, 0x03, 0x60, 0x06, 0xF0, 0x0F, 0x60, 
  0x07, 0x38, 0x1C, 0xE0, 0x0E, 0x18, 0x18, 0x78, 0x38, 0x0C, 0x30, 0x1C, 
  0x30, 0x0C, 0x30, 0x0C, 0x30, 0x0C, 0x30, 0x0C, 0x3C, 0x0C, 0x30, 0x3C, 
  0x0E, 0x18, 0x18, 0x70, 0x07, 0x38, 0x1C, 0xE0, 0x06, 0xF0, 0x0F, 0x60, 
  0x06, 0xC0, 0x03, 0x60, 0xEE, 0x00, 0x00, 0x77, 0xFC, 0x01, 0x80, 0x3F, 
  0x2C, 0x03, 0xC0, 0x38, 0x00, 0x03, 0xC0, 0x00, 0x00, 0xC3, 0xC3, 0x01, 
  0x80, 0xC1, 0x83, 0x01, 0x80, 0x63, 0x86, 0x01, 0x80, 0x3F, 0xFC, 0x01, 
  0x00, 0x3E, 0x7C, 0x00, 0x00, 0x10, 0x08, 0x00,
};


static void tile_float_in(Frame * frame, int32_t duration, int32_t offset)
{
    frame->centerFrom.x = WIDTH / 2;
    frame->centerFrom.y = HEIGHT / 2;
    frame->widthFrom = 0;
    frame->widthTo = WIDTH / 2;
    frame->heightFrom = 0;
    frame->heightTo = HEIGHT / 2;
    frame->animation.duration = duration;
    frame->angleTo = 0;
    //frame->state = TILE_SHOWING;
    frame->animation.duration = duration;
    fezui_animation_begin(&frame->animation, offset);
}

static void tile_float_out(Frame * frame, int32_t duration, int32_t offset)
{
    frame->centerFrom.x = FEZUI_ANIMATION_GET_VALUE(&frame->animation, frame->centerFrom.x, frame->centerTo.x);
    frame->centerFrom.y = FEZUI_ANIMATION_GET_VALUE(&frame->animation, frame->centerFrom.y, frame->centerTo.y);
    frame->centerTo.x = WIDTH / 2;
    frame->centerTo.y = HEIGHT / 2;
    frame->widthFrom = FEZUI_ANIMATION_GET_VALUE(&frame->animation, frame->widthFrom, frame->widthTo);
    frame->widthTo = 0;
    frame->heightFrom = FEZUI_ANIMATION_GET_VALUE(&frame->animation, frame->heightFrom, frame->heightTo);
    frame->heightTo = 0;
    frame->angleTo = 0;
    //frame->state = TILE_CLOSING;
    frame->animation.duration = duration;
    fezui_animation_begin(&frame->animation, offset);
}

static void tile_open(Frame * frame, int32_t duration, int32_t offset)
{
    frame->centerFrom.x = FEZUI_ANIMATION_GET_VALUE(&frame->animation, frame->centerFrom.x, frame->centerTo.x);
    frame->centerFrom.y = FEZUI_ANIMATION_GET_VALUE(&frame->animation, frame->centerFrom.y, frame->centerTo.y);
    frame->centerTo.x = WIDTH/2;
    frame->centerTo.y = HEIGHT/2;
    frame->widthFrom = FEZUI_ANIMATION_GET_VALUE(&frame->animation, frame->widthFrom, frame->widthTo);
    frame->widthTo = WIDTH;
    frame->heightFrom = FEZUI_ANIMATION_GET_VALUE(&frame->animation, frame->heightFrom, frame->heightTo);
    frame->heightTo = HEIGHT;
    frame->angleTo = M_PI;
    //frame->state = TILE_NONE;
    frame->animation.duration = duration;
    fezui_animation_begin(&frame->animation, offset);
}
void launcherpage_open();
void launcherpage_init()
{
    fezui_animated_listbox_init(&mainmenu, (void **)mainmenu_items, 4, main_menu_cb, i18n_item_draw, i18n_item_get_cursor);
    for (int i = 0; i < 4; i++)
    {
        frames[i].animation.easing_func = fezui_animation_cubic_ease;
        frames[i].animation.mode = EASE_OUT;
        frames[i].widthFrom = TILE_WIDTH;
        frames[i].widthTo = TILE_WIDTH;
        frames[i].heightFrom = TILE_HEIGHT;
        frames[i].heightTo = TILE_HEIGHT;
    }
    open_menu_animation.easing_func = fezui_animation_cubic_ease;
    open_menu_animation.mode = EASE_OUT;
    open_menu_animation.duration = 250;
    open_animation.easing_func = fezui_animation_cubic_ease;
    open_animation.mode = EASE_OUT;
    open_animation.duration = 250;
    icon_animation.easing_func = fezui_animation_cubic_ease;
    icon_animation.mode = EASE_OUT;
    icon_animation.duration = 500;
    frames[0].icon = (uint8_t*)home_bitmap_e80f;
    frames[1].icon = (uint8_t*)oscilloscope_bitmap_e80f;
    frames[2].icon = (uint8_t*)statistic_bitmap_e80f;
    frames[3].icon = (uint8_t*)setting_bitmap_e80f;

    mainmenu.listbox.list.selected_index = 0;
    isPageOpen = true;
    for (int i = 0; i < 4; i++)
    {
        if (i != mainmenu.listbox.list.selected_index)
        {
            tile_float_out(&frames[i],10,0);
        }
    }
    Frame *frame = &frames[mainmenu.listbox.list.selected_index];
    fezui_animation_begin(&frame->animation, 0);
    tile_open(frame,10,0);
    fezui_animation_begin(&open_animation, 250);
    icon_animation.duration = 10;
    fezui_animation_begin(&icon_animation, 0);
    pages[mainmenu.listbox.list.selected_index]->page_load_cb(pages[mainmenu.listbox.list.selected_index]);
    state = TILE_OPENING;
}

void draw_line(u8g2_t *u8g2, u8g2_int_t x1, u8g2_int_t y1, u8g2_int_t x2, u8g2_int_t y2)
{
    Point p;
    p.y = 0;
    if (y1 < 0 && y2 < 0)
    {
        return;
    }
    if (y1 < 0)
    {
        p.x = x1 + ((float)(x2 - x1)) * ((float)y1 / (float)(y2 - y1));
        u8g2_DrawLine(&fezui.u8g2, (u8g2_int_t)p.x, (u8g2_int_t)p.y, x2, y2);
        return;
    }
    if (y2 < 0)
    {
        p.x = x1 + ((float)(x2 - x1)) * ((float)y1 / (float)(y2 - y1));
        u8g2_DrawLine(&fezui.u8g2, x1, y1, (u8g2_int_t)p.x, (u8g2_int_t)p.y);
        return;
    }
    u8g2_DrawLine(&fezui.u8g2, x1, y1, x2, y2);
}

static void draw_tile_icon(Frame *frame)
{
    fezui_animation_calculate(&frame->animation);
    const float centerX = FEZUI_ANIMATION_GET_VALUE(&frame->animation, frame->centerFrom.x, frame->centerTo.x);
    const float centerY = FEZUI_ANIMATION_GET_VALUE(&frame->animation, frame->centerFrom.y, frame->centerTo.y);

    u8g2_DrawXBMP(&fezui.u8g2,(u8g2_int_t)(centerX-16),
    (u8g2_int_t)(centerY-16),32,32,frame->icon);
}

static void draw_tile(Frame *frame)
{
    Point vertices[4];
    Point rotatedVertices[4];
    const float angle = FEZUI_ANIMATION_GET_VALUE(&frame->animation, 0.0, frame->angleTo);
    const float cosTheta = cosf(angle);
    const float sinTheta = sinf(angle);
    fezui_animation_calculate(&frame->animation);
    const float centerX = FEZUI_ANIMATION_GET_VALUE(&frame->animation, frame->centerFrom.x, frame->centerTo.x);
    const float centerY = FEZUI_ANIMATION_GET_VALUE(&frame->animation, frame->centerFrom.y, frame->centerTo.y);
    vertices[0].x = -FEZUI_ANIMATION_GET_VALUE(&frame->animation, frame->widthFrom, frame->widthTo)/2;
    vertices[0].y = -FEZUI_ANIMATION_GET_VALUE(&frame->animation, frame->heightFrom, frame->heightTo)/2;
    vertices[1].x = FEZUI_ANIMATION_GET_VALUE(&frame->animation, frame->widthFrom, frame->widthTo)/2;
    vertices[1].y = -FEZUI_ANIMATION_GET_VALUE(&frame->animation, frame->heightFrom, frame->heightTo)/2;
    vertices[2].x = FEZUI_ANIMATION_GET_VALUE(&frame->animation, frame->widthFrom, frame->widthTo)/2;
    vertices[2].y = FEZUI_ANIMATION_GET_VALUE(&frame->animation, frame->heightFrom, frame->heightTo)/2;
    vertices[3].x = -FEZUI_ANIMATION_GET_VALUE(&frame->animation, frame->widthFrom, frame->widthTo)/2;
    vertices[3].y = FEZUI_ANIMATION_GET_VALUE(&frame->animation, frame->heightFrom, frame->heightTo)/2;
    
    for (int i = 0; i < 4; i++)
    {
        vertices[i].z = 0;
        const float x = vertices[i].x * cosTheta - vertices[i].z * sinTheta;
        const float y = vertices[i].y;
        const float z = vertices[i].x * sinTheta + vertices[i].z * cosTheta;

        const float perspective = 200 / (200 + z); // 简单透视
        const float screenX = centerX + x * perspective;
        const float screenY = centerY - y * perspective;

        rotatedVertices[i].x = screenX;
        rotatedVertices[i].y = screenY;
    }

    u8g2_DrawTriangle(&fezui.u8g2,
                      (u8g2_uint_t)rotatedVertices[0].x,
                      (u8g2_uint_t)rotatedVertices[0].y,
                      (u8g2_uint_t)rotatedVertices[1].x,
                      (u8g2_uint_t)rotatedVertices[1].y,
                      (u8g2_uint_t)rotatedVertices[2].x,
                      (u8g2_uint_t)rotatedVertices[2].y);
    u8g2_DrawTriangle(&fezui.u8g2,
                      (u8g2_uint_t)rotatedVertices[1].x,
                      (u8g2_uint_t)rotatedVertices[1].y,
                      (u8g2_uint_t)rotatedVertices[2].x,
                      (u8g2_uint_t)rotatedVertices[2].y,
                      (u8g2_uint_t)rotatedVertices[3].x,
                      (u8g2_uint_t)rotatedVertices[3].y);
    u8g2_DrawTriangle(&fezui.u8g2,
                      (u8g2_uint_t)rotatedVertices[2].x,
                      (u8g2_uint_t)rotatedVertices[2].y,
                      (u8g2_uint_t)rotatedVertices[3].x,
                      (u8g2_uint_t)rotatedVertices[3].y,
                      (u8g2_uint_t)rotatedVertices[0].x,
                      (u8g2_uint_t)rotatedVertices[0].y);
    u8g2_DrawTriangle(&fezui.u8g2,
                      (u8g2_uint_t)rotatedVertices[3].x,
                      (u8g2_uint_t)rotatedVertices[3].y,
                      (u8g2_uint_t)rotatedVertices[0].x,
                      (u8g2_uint_t)rotatedVertices[0].y,
                      (u8g2_uint_t)rotatedVertices[1].x,
                      (u8g2_uint_t)rotatedVertices[1].y);

    //u8g2_SetDrawColor(&fezui.u8g2,0);
    //u8g2_DrawXBMP(&fezui.u8g2,(u8g2_int_t)(centerX-16),
    //(u8g2_int_t)(centerY-16),32,32,frame->icon);
    //u8g2_SetDrawColor(&fezui.u8g2,1);

}

static void launcherpage_tick(void *page)
{
    // offset_y = ;
    if (isPageOpen)
    {
        pages[mainmenu.listbox.list.selected_index]->page_tick_cb(pages[mainmenu.listbox.list.selected_index]);
    }
    if (open_animation.value == 1 && state == TILE_OPENING)
    {
        return;
    }
    CONVERGE_TO_ROUNDED(offset_y, -(mainmenu.listbox.list.selected_index * (MARGIN + TILE_HEIGHT)), fezui.speed);

    if (!isPageOpen)
    {
        for (int i = 0; i < 4; i++)
        {
            frames[i].centerTo.x = LEFT + TILE_WIDTH/2;
            frames[i].centerTo.y = (offset_y + TOP + (TILE_HEIGHT + MARGIN) * i) + TILE_HEIGHT/2;
        }
    }
}

static void launcherpage_draw(void *page)
{
    if (!isPageOpen)
    {
        g_keyboard_send_report_enable = false;
        g_keyboard_current_layer = 0;
        // fezui_animated_listbox_begin(&mainmenu);
    }

    switch (fezui.lang)
    {
    case LANG_EN:
        u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x8_mr);
        break;
    case LANG_ZH:
        u8g2_SetFont(&(fezui.u8g2), u8g2_font_wqy13_t_gb2312b);
        break;
    default:
        break;
    }
    //const char *(*i18n_items)[LANG_NUM] = (const char *(*)[LANG_NUM])mainmenu.listbox.list.items;
    fezui_animation_calculate(&open_menu_animation);
    fezui_animation_calculate(&open_animation);
    fezui_animation_calculate(&icon_animation);
    if (open_animation.value == 1 && state == TILE_OPENING)
    {
        pages[mainmenu.listbox.list.selected_index]->page_draw_cb(pages[mainmenu.listbox.list.selected_index]);
        return;
    }
    

    if (state == TILE_SHOWING)
    {
        if (open_menu_animation.value < 0.5)
        {
            pages[mainmenu.listbox.list.selected_index]->page_draw_cb(pages[mainmenu.listbox.list.selected_index]);
            fezui_veil_full_screen(&fezui, open_menu_animation.value * 14);
        }
        else
        {
            fezui_veil_full_screen(&fezui, open_menu_animation.value * 14);
        }
    }
    

    DRAW_IN_TEMP_BUFFER(
        u8g2_SetDrawColor(&fezui.u8g2,1);
        switch (state)
        {
        case TILE_SHOWING:
            for (int i = 0; i < mainmenu.listbox.list.len; i++)
            {
                draw_tile_icon(&frames[i]);
            }
            fezui_veil(&fezui, 0, 0, WIDTH, HEIGHT, 7 - icon_animation.value*7, 0);
            break;
        case TILE_OPENING:
            if (icon_animation.value*14 < 7)
            {
                for (int i = 0; i < mainmenu.listbox.list.len; i++)
                {
                    draw_tile_icon(&frames[i]);
                }
                fezui_veil(&fezui, 0, 0, WIDTH, HEIGHT, icon_animation.value*14 , 0);
            }
            else
            {
                draw_tile_icon(&frames[mainmenu.listbox.list.selected_index]);
                fezui_veil(&fezui, 0, 0, WIDTH, HEIGHT, (14 - icon_animation.value*14), 0);
                fezui_veil(&fezui, 0, 0, WIDTH, HEIGHT, (open_animation.value*7), 0);
            }
            break;
        
        default:
            break;
        }
    );
    u8g2_SetDrawColor(&fezui.u8g2,1);
    for (int i = 0; i < mainmenu.listbox.list.len; i++)
    {
        draw_tile(&frames[i]);
    }
    //fezui_veil_full_screen(&fezui, 7-open_menu_animation.value * 7);
    for (int i = 0; i < 1024; i++)
    {
        fezui.u8g2.tile_buf_ptr[i] ^= tile_buffer[i];
    }

    if(state == TILE_OPENING)
    {
        u8g2_SetDrawColor(&(fezui.u8g2), 1);
        DRAW_IN_TEMP_BUFFER(
            pages[mainmenu.listbox.list.selected_index]->page_draw_cb(pages[mainmenu.listbox.list.selected_index]);
            fezui_veil(&fezui, 0, 0, WIDTH, HEIGHT, 7 - open_animation.value * 7, 1);
        );
        for (int i = 0; i < 1024; i++)
        {
            fezui.u8g2.tile_buf_ptr[i] &= tile_buffer[i];
        }
    }
}

static void main_menu_cb(void *menu)
{
    switch (((fezui_animated_listbox_t *)menu)->listbox.list.selected_index)
    {
    case 0:
        fezui_frame_go_back(&g_mainframe);
        break;
    case 1:
        fezui_frame_navigate(&g_mainframe, &oscilloscopepage);
        break;
    case 2:
        fezui_frame_navigate(&g_mainframe, &statisticpage);
        break;
    case 3:
        fezui_frame_navigate(&g_mainframe, &settingspage);
        break;
    default:
        break;
    }
}

static void launcherpage_load(void *page)
{
    g_keyboard_send_report_enable = false;
    g_keyboard_current_layer = 0;
    fezui_animated_listbox_begin(&mainmenu);
}

void launcherpage_open_menu()
{
    for (int i = 0; i < 4; i++)
    {
        tile_float_in(&frames[i],500 ,50*i);
    }
    isPageOpen = false;
    icon_animation.duration = 500;
    fezui_animation_begin(&icon_animation, 250);
    open_menu_animation.duration = 500;
    fezui_animation_begin(&open_menu_animation, 0);
    state = TILE_SHOWING;
}

void launcherpage_open()
{
    isPageOpen = true;
    for (int i = 0; i < 4; i++)
    {
        if (i != mainmenu.listbox.list.selected_index)
        {
            tile_float_out(&frames[i],500,0);
        }
    }
    Frame *frame = &frames[mainmenu.listbox.list.selected_index];
    fezui_animation_begin(&frame->animation, 0);
    tile_open(frame,500,0);
    fezui_animation_begin(&open_animation, 250);
    icon_animation.duration = 500;
    fezui_animation_begin(&icon_animation, 0);
    pages[mainmenu.listbox.list.selected_index]->page_load_cb(pages[mainmenu.listbox.list.selected_index]);
    state = TILE_OPENING;
}

static void launcherpage_event_handler(void *e)
{
    switch (*(uint16_t *)e)
    {
    case KEY_UP_ARROW:
        if (isPageOpen)
        {
            pages[mainmenu.listbox.list.selected_index]->event_handler(e);
        }
        else
        {
            fezui_animated_listbox_index_increase(&mainmenu, -1);
        }
        break;
    case KEY_DOWN_ARROW:
        if (isPageOpen)
        {
            pages[mainmenu.listbox.list.selected_index]->event_handler(e);
        }
        else
        {
            fezui_animated_listbox_index_increase(&mainmenu, 1);
        }
        break;
    case KEY_ENTER:
        if (isPageOpen)
        {
            pages[mainmenu.listbox.list.selected_index]->event_handler(e);
        }
        else
        {
            launcherpage_open();
        }
        break;
    case KEY_ESC:
        if (isPageOpen)
        {
            launcherpage_open_menu();
        }
        else
        {
            mainmenu.listbox.list.selected_index = 0;
            launcherpage_open();
        }
        break;
    default:
        break;
    }
}


fezui_page_t launcherpage = {launcherpage_tick, launcherpage_draw, launcherpage_load, launcherpage_event_handler};
