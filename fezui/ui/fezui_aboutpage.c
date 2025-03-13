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

// width: 33, height: 33
static const unsigned char qr_code[] U8X8_PROGMEM = { 0x7f,0x58,0x9d,0xfc,0x01,0x41,0x8c,0x65,0x04,0x01,0x5d,0x4f,0x61,0x74,0x01,0x5d,0x19,0x4c,0x75,0x01,0x5d,0xdf,0xf1,0x74,0x01,0x41,0x49,0x0c,0x05,0x01,0x7f,0x55,0x55,0xfd,0x01,0x00,0x1d,0xb9,0x01,0x00,0x7d,0x60,0x0a,0x7c,0x00,0x8c,0x64,0xbe,0x6c,0x01,0xf3,0xed,0x43,0xd1,0x00,0x91,0xbe,0x6e,0x73,0x01,0x73,0x4c,0xd9,0xba,0x01,0xad,0x93,0x86,0x84,0x01,0x49,0xed,0x53,0xee,0x00,0x38,0xdd,0x24,0x6f,0x00,0x51,0xc6,0xd2,0x3b,0x01,0x2d,0x68,0xbd,0x6c,0x01,0x75,0x8c,0x34,0xdb,0x00,0xad,0xa3,0x79,0xf8,0x00,0x5a,0x6e,0x4c,0x31,0x01,0x3b,0x91,0xf1,0x64,0x01,0x51,0x48,0x0a,0xc3,0x00,0x39,0x4d,0xf9,0xf6,0x00,0xd9,0x7a,0x4a,0x3f,0x00,0x00,0x63,0xab,0x51,0x01,0x7f,0xf4,0x93,0xd5,0x00,0x41,0xaf,0xb6,0x71,0x01,0x5d,0x09,0x49,0x3f,0x00,0x5d,0xc5,0x84,0x93,0x01,0x5d,0xd9,0x55,0x6c,0x00,0x41,0xaa,0x70,0x56,0x00,0x7f,0xaf,0xc2,0x8b,0x00 };

static char single_char[2];

static const char* device_name = "Trinity Pad";
static float device_name_offsets[11] = {WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH};
static uint8_t device_name_width[11] = {10,6,3,9,3,5,5,6,10,8,10};

static const char* device_version = "Version: 0.1.0";
static float device_version_offsets[14] = {WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH,WIDTH};

static const char* device_information[]={
        "Copyright (c) 2023-2024",
        "Lzq12345",
        "",
        "",
        "MCU: CH32V307WCU6",
        "Date:" __DATE__,
        "Time:" __TIME__,
};

//static const char* device_project_url[]={
//        "For more information, view"
//        
//};

#define target_icon_x 0
static float icon_x=WIDTH;

#define target_url_y 23
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
    CONVERGE_TO(icon_x,target_icon_x,0.05);
    CONVERGE_TO(url_y,target_url_y,0.05);
    CONVERGE_TO_ROUNDED(device_name_offsets[0],target_icon_x,0.2);
    CONVERGE_TO_ROUNDED(device_version_offsets[0],target_icon_x,0.2);
    for (uint8_t i = 1; i < 11; i++)
    {
        CONVERGE_TO(device_name_offsets[i],device_name_offsets[i-1]+device_name_width[i-1],0.2);
    }
    for (uint8_t i = 1; i < 14; i++)
    {
        CONVERGE_TO(device_version_offsets[i],device_version_offsets[i-1]+4,0.2);
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
        u8g2_DrawUTF8(&(fezui.u8g2), 34+device_name_offsets[i], 16, single_char);
    }
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_4x6_mr);
    for (uint8_t i = 0; i < 14; i++)
    {
        single_char[0]=device_version[i];
        u8g2_DrawUTF8(&(fezui.u8g2), 34+device_version_offsets[i], 23, single_char);
    }

    //for (uint8_t i = 0; i < sizeof(device_project_url)/sizeof(char*); i++)
    //{
    //    u8g2_DrawUTF8(&(fezui.u8g2), 0, url_y+6*(i+1), device_project_url[i]);
    //}
    //fezui_draw_scrolling_text(&fezui,0,url_y+6*2,&url_text);
    
    //draw_gemini();
    //u8g2_SetClipWindow(&(fezui.u8g2),34,23,WIDTH,52);
    //for (uint8_t i = 0; i < sizeof(device_information)/sizeof(char*); i++)
    //{
    //    u8g2_DrawUTF8(&(fezui.u8g2), 34, 18+ROW_HEIGHT*(i+1)-(u8g2_int_t)ordinate, device_information[i]);
    //}
    //u8g2_SetMaxClipWindow(&(fezui.u8g2));

    //u8g2_SetClipWindow(&(fezui.u8g2),34,23,WIDTH,HEIGHT);
    for (uint8_t i = 0; i < sizeof(device_information)/sizeof(char*); i++)
    {
        u8g2_DrawUTF8(&(fezui.u8g2), 34, ROW_HEIGHT*(i+1)+url_y, device_information[i]);
    }
    //u8g2_SetMaxClipWindow(&(fezui.u8g2));

    u8g2_DrawXBMP(&fezui.u8g2,0,((u8g2_int_t)icon_x),33,33,qr_code);
    
    u8g2_DrawUTF8(&(fezui.u8g2), ((u8g2_int_t)icon_x+0), 42 + 6, "Powered");
    u8g2_DrawUTF8(&(fezui.u8g2), ((u8g2_int_t)icon_x+0), 42 + 6*2, "by");
    u8g2_SetFont(&fezui.u8g2, u8g2_font_3x3basic_tr);
    u8g2_DrawUTF8X2(&fezui.u8g2, ((u8g2_int_t)icon_x+11), 48 + 8, "FEZ");
    u8g2_DrawUTF8X2(&fezui.u8g2, ((u8g2_int_t)icon_x+19), 48 + 8 + 8, "UI");
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
    case KEY_SPACEBAR:
    case KEY_ENTER:
        break;
    case KEY_ESC:
        fezui_frame_go_back(&g_mainframe);
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
    icon_x=-33;
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

fezui_page_t aboutpage={aboutpage_tick,aboutpage_draw,aboutpage_load,aboutpage_event_handler};

