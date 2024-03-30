/*
 * fezui_aboutpage.c
 *
 *  Created on: 2023年6月26日
 *      Author: xq123
 */
#include "fezui.h"
#include "fezui_var.h"

#define PROJECT_URL "github.com/zhangqili/trinity-pad-firmware"

#define ROW_HEIGHT 6

#define CONSTELLATION_X ((u8g2_int_t)icon_x)
#define CONSTELLATION_Y 0

#define ZOOM (18.0/64.0)

#define ALPHA_X 8
#define ALPHA_Y 5

#define BETA_X 1
#define BETA_Y 15

#define GAMMA_X 42
#define GAMMA_Y 48

#define DELTA_X 15
#define DELTA_Y 32

#define EPSILON_X 38
#define EPSILON_Y 24

#define ZETA_X 25
#define ZETA_Y 37

#define ETA_X 56
#define ETA_Y 31

#define THETA_X 33
#define THETA_Y 0

#define LOTA_X 13
#define LOTA_Y 17

#define KAPPA_X 0
#define KAPPA_Y 25

#define LAMBDA_X 16
#define LAMBDA_Y 47

#define MU_X 51
#define MU_Y 31

#define NU_X 48
#define NU_Y 38

#define XI_X 37
#define XI_Y 58

#define TAU_X 22
#define TAU_Y 10

#define UPSILON_X 6
#define UPSILON_Y 19

#define ONE_X 63
#define ONE_Y 28

#define DRAW_LINE_BETWEEN(a,b) u8g2_DrawLine(&(fezui.u8g2), CONSTELLATION_X + a##_X*ZOOM, CONSTELLATION_Y + a##_Y*ZOOM, CONSTELLATION_X + b##_X*ZOOM, CONSTELLATION_Y+b##_Y*ZOOM);


static char single_char[2];

static const char* device_name = "Trinity Pad";
static float device_name_offsets[11] = {WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH};
static uint8_t device_name_width[11] = {10,6,3,9,3,5,5,6,10,8,10};

static const char* device_version = "Version: 1.0.0";
static float device_version_offsets[14] = {WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH};

static const char* device_information[]={
        "Copyright (c) 2023 Lzq12345",
        "",
        "MCU: CH32V307WCU6",
        "Date:" __DATE__ " " __TIME__,
};

static const char* device_project_url[]={
        "For more information, view"
        
};

#define target_icon_x 0
static float icon_x=WIDTH;

#define target_url_y 50
static float url_y=WIDTH;

static float text_box_height=(HEIGHT-18-12);
//static float text_height=(ROW_HEIGHT*sizeof(device_project_url)/sizeof(char*));
static float ordinate=0;
static float target_ordinate=0;

static fezui_scrolling_text_t url_text;

void draw_gemini()
{
    DRAW_LINE_BETWEEN(ALPHA,TAU);
    DRAW_LINE_BETWEEN(TAU,LOTA);
    DRAW_LINE_BETWEEN(LOTA,UPSILON);
    DRAW_LINE_BETWEEN(UPSILON,BETA);
    DRAW_LINE_BETWEEN(UPSILON,KAPPA);
    DRAW_LINE_BETWEEN(UPSILON,DELTA);
    DRAW_LINE_BETWEEN(DELTA,LAMBDA);
    DRAW_LINE_BETWEEN(LAMBDA,XI);
    DRAW_LINE_BETWEEN(DELTA,ZETA);
    DRAW_LINE_BETWEEN(ZETA,GAMMA);
    DRAW_LINE_BETWEEN(TAU,EPSILON);
    DRAW_LINE_BETWEEN(EPSILON,NU);
    DRAW_LINE_BETWEEN(EPSILON,MU);
    DRAW_LINE_BETWEEN(MU,ETA);
    DRAW_LINE_BETWEEN(ETA,ONE);
    DRAW_LINE_BETWEEN(TAU,THETA);
}

static void aboutpage_tick(void *page)
{
    CONVERGE_TO(icon_x,target_icon_x,fezui.speed);
    CONVERGE_TO(url_y,target_url_y,fezui.speed);
    CONVERGE_TO_ROUNDED(device_name_offsets[0],icon_x,fezui.speed);
    CONVERGE_TO_ROUNDED(device_version_offsets[0],icon_x,fezui.speed);
    for (uint8_t i = 1; i < 11; i++)
    {
        CONVERGE_TO(device_name_offsets[i],device_name_offsets[i-1]+device_name_width[i-1],fezui.speed);
    }
    for (uint8_t i = 1; i < 14; i++)
    {
        CONVERGE_TO(device_version_offsets[i],device_version_offsets[i-1]+4,fezui.speed);
    }
    target_ordinate+=0.1;
    CONVERGE_TO(ordinate,target_ordinate,fezui.speed);
    if(ordinate>text_box_height+10)
    {
        ordinate=-text_box_height;
        target_ordinate=-text_box_height;
    }
    fezui_scrolling_text_update(&url_text);
}
static void aboutpage_draw(void *page)
{
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_helvB12_tr);
    for (uint8_t i = 0; i < 11; i++)
    {
        single_char[0]=device_name[i];
        u8g2_DrawStr(&(fezui.u8g2), 19+device_name_offsets[i], 12, single_char);
    }
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_4x6_mr);
    for (uint8_t i = 0; i < 14; i++)
    {
        single_char[0]=device_version[i];
        u8g2_DrawStr(&(fezui.u8g2), 19+device_version_offsets[i], 18, single_char);
    }

    for (uint8_t i = 0; i < sizeof(device_project_url)/sizeof(char*); i++)
    {
        u8g2_DrawStr(&(fezui.u8g2), 0, url_y+6*(i+1), device_project_url[i]);
    }
    fezui_draw_scrolling_text(&fezui,0,url_y+6*2,&url_text);
    draw_gemini();
    u8g2_SetClipWindow(&(fezui.u8g2),19,19,WIDTH,50);
    for (uint8_t i = 0; i < sizeof(device_information)/sizeof(char*); i++)
    {
        u8g2_DrawStr(&(fezui.u8g2), 19, 18+ROW_HEIGHT*(i+1)-(u8g2_int_t)ordinate, device_information[i]);
    }
    u8g2_SetMaxClipWindow(&(fezui.u8g2));
}

static void aboutpage_event_handler(void *e)
{
    switch (*(uint16_t*)e)
    {
    case KEY_UP_ARROW:
        target_ordinate+=10;
        if(ordinate>text_box_height)
        {
            ordinate=-text_box_height;
            target_ordinate=-text_box_height;
        }
        break;
    case KEY_DOWN_ARROW:
        target_ordinate-=10;
        if(ordinate<-text_box_height)
        {
            ordinate=+text_box_height;
            target_ordinate=+text_box_height;
        }
        break;
    case KEY_ENTER:
        break;
    case KEY_ESC:
        fezui_link_frame_go_back(&g_mainframe);
        fezui_cursor_set(&g_fezui_cursor ,0 ,0 ,WIDTH ,HEIGHT);
        break;
    default:
        break;
    }
}

static void aboutpage_load(void *page)
{
    fezui_scrolling_text_init(&fezui, &url_text, 128, 0.2, u8g2_font_4x6_mr, PROJECT_URL);
    fezui_scrolling_text_begin(&url_text);
    ordinate=-text_box_height;
    target_ordinate=-text_box_height;
    icon_x=WIDTH;
    url_y=HEIGHT;
    for (uint8_t i = 0; i < 11; i++)
    {
        device_name_offsets[i]=WIDTH;
    }
    for (uint8_t i = 0; i < 14; i++)
    {
        device_version_offsets[i]=WIDTH;
    }

}

fezui_link_page_t aboutpage={aboutpage_tick,aboutpage_draw,aboutpage_load,aboutpage_event_handler};

