/*
 * fezui_config.c
 *
 *      Author: xq123
 */

#include "fezui.h"
#include "fezui_var.h"
#include "lefl.h"
#include "display.h"
#include "sfud.h"
#include "fram.h"
#include "lfs.h"
#include "record.h"


uint8_t tempuint;

#define U8LOG_WIDTH 32
#define U8LOG_HEIGHT 10
uint8_t u8log_buffer[U8LOG_WIDTH*U8LOG_HEIGHT];
void fezui_init()
{
    MD_OLED_RST_Set();
    u8g2_Setup_ssd1306_128x64_noname_f(&(fezui.u8g2), U8G2_R0, u8x8_byte_4wire_hw_spi,
            u8x8_riscv_gpio_and_delay);
    u8g2_InitDisplay(&(fezui.u8g2));
    u8g2_SetPowerSave(&(fezui.u8g2), 0);
    //u8g2_SetBitmapMode(&(fezui.u8g2), 1);
    u8g2_SetFontMode(&(fezui.u8g2), 1);
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_tom_thumb_4x6_mf);
    u8log_Init(&u8log,U8LOG_WIDTH,U8LOG_HEIGHT,u8log_buffer);

    fezui_reset();
    fezui_POST();

    debugpage_init();
    menupage_init();
    settingspage_init();
    calibrationpage_init();
    keyconfigpage_init();
    advancedconfigpage_init();
    keymappage_init();
    keylistpage_init();
    knobconfigpage_init();
    rgbconfigpage_init();
    displayconfigpage_init();
    panelpage_init();
    advancedsettingspage_init();

    fezui_link_frame_navigate(&mainframe, &homepage);
    //Keybaord_SendReport_Enable=false;
    //fezui_link_frame_navigate(&mainframe, &debugpage);
    //fezui_link_frame_navigate(&mainframe, &oscilloscopepage);

}

void fezui_timer_handler()
{
    fezui_link_frame_logic(&mainframe);
    fezui_notification_update(&fezui,&fezui_notification);
    fezui_cursor_move(&fezui ,&cursor, &target_cursor);
    //fezui_animated_cursor_update(&animated_cursor);
    fezui_kps = record_get_kps();
    if (fezui_kps > UI_KPSMaximumPerSecond)
    {
        UI_KPSMaximumPerSecond = fezui_kps;
    }
    KPS_history_max=loop_array_max(&g_kps_history);
    //fezui_save_counts();

    uint8_t key_pressed_num=0;
    for (uint8_t i = 0; i < KEY_NUM; i++)
    {
        key_pressed_num += Keyboard_Keys[i].state;
    }
    
    if (KPS_history_max || key_pressed_num)
    {
        fezui.screensaver_countdown = fezui.screensaver_timeout;
    }
}

void fezui_render_handler()
{
    u8g2_ClearBuffer(&(fezui.u8g2));
    u8g2_SetDrawColor(&(fezui.u8g2),1);
    fezui_link_frame_draw(&mainframe);
    fezui_draw_notification(&fezui, &fezui_notification);
    if(fezui.invert)
    {
        u8g2_SetDrawColor(&(fezui.u8g2), 2);
        u8g2_DrawBox(&(fezui.u8g2), 0,0,WIDTH,HEIGHT);    
    }
    if(fezui.screensaver_timeout)
    {
        fezui_veil_full_screen(&(fezui),(7-fezui.screensaver_countdown)>256?0:7-fezui.screensaver_countdown);
        u8g2_SetPowerSave(&(fezui.u8g2),!fezui.screensaver_countdown);
    }
#ifdef _FPS_ON
    u8g2_SetDrawColor(&(fezui.u8g2), 2);
    u8g2_SetFont(&(fezui.u8g2), fez_font_6x10_m);
    u8g2_DrawStr(&(fezui.u8g2),95+15,10,fpsstr);
#endif
    u8g2_SendBuffer(&(fezui.u8g2));
    fezui_fps++;
}

#define CHAR_HEIGHT 8
#define CHAR_WIDTH 5
void fezui_POST()
{
    u8g2_SetFont(&fezui.u8g2,u8g2_font_5x7_tf);
    u8g2_DrawStr(&fezui.u8g2,0,CHAR_HEIGHT-1,"FEZUI");
    u8g2_DrawHLine(&fezui.u8g2,0,CHAR_HEIGHT,WIDTH);
    u8log_WriteString(&u8log,"Mounting Flash...");
    u8g2_DrawLog(&fezui.u8g2,0,CHAR_HEIGHT*2,&u8log);
    u8g2_SendBuffer(&fezui.u8g2);
    // mount the filesystem
    int err = lfs_mount(&lfs_w25qxx, &cfg);
    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
    if (err)
    {
        err = lfs_format(&lfs_w25qxx, &cfg);
        lfs_mount(&lfs_w25qxx, &cfg);
        u8log_WriteString(&u8log," [Failed]\n");
        u8log_WriteString(&u8log,"Formating Flash...");
        u8g2_DrawLog(&fezui.u8g2,0,CHAR_HEIGHT*2,&u8log);
        u8g2_SendBuffer(&fezui.u8g2);
        if(err)
        {
            u8log_WriteString(&u8log," [Failed]\n");
        }
        else
        {
            u8log_WriteString(&u8log," [OK]\n");
        }
    }
    else
    {
        u8log_WriteString(&u8log," [OK]\n");
    }
    u8g2_DrawLog(&fezui.u8g2,0,CHAR_HEIGHT*2,&u8log);
    u8g2_SendBuffer(&fezui.u8g2);
    // remember the storage is not updated until the file is closed successfully
    lfs_file_close(&lfs_w25qxx, &lfs_file_w25qxx);
    // release any resources we were using
    lfs_unmount(&lfs_w25qxx);
    u8log_WriteString(&u8log,"Testing FRAM...");
    u8g2_SendBuffer(&fezui.u8g2);
	uint32_t fram_id = fram_read_device_id();
    if((fram_id)&&(~fram_id));
    {
        u8log_WriteString(&u8log," [OK]\n");
    }
    u8g2_DrawLog(&fezui.u8g2,0,CHAR_HEIGHT*2,&u8log);
    u8g2_SendBuffer(&fezui.u8g2);
}


void keyid_prase(KeyBinding id,char* str,uint16_t str_len)
{
    bool key_found = false;
    memset(str,0,str_len);
    for (uint8_t i = 0; i < 8;i++)
    {
        if(id.modifier&BIT(i))
        {
            if(key_found)
            {
                strcat(str, " + ");
                strcat(str, hid_usage_names[i]);
            }
            else
            {
                strcat(str, hid_usage_names[i]);
                key_found=true;
            }
        }
    }

    if(id.keycode)
    {
        if(key_found)
        {
            strcat(str, " + ");
            strcat(str, hid_usage_names[(id.keycode)+8]);
        }
        else
        {
            strcat(str, hid_usage_names[(id.keycode)+8]);
            key_found=true;
        }
    }
    else
    {
        if(!key_found)
        {
            strcat(str, "None");
        }
    }
}



void fezui_reset()
{
    fezui.invert=0;
    fezui.contrast=255;
    fezui.screensaver_countdown=60;
    fezui.screensaver_timeout=60;
    fezui.speed=0.05;
    fezui_apply(&fezui);
}
