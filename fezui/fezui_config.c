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

    lefl_loop_array_init(&KPS_history, KPS_history_data, sizeof(KPS_history_data)/sizeof(lefl_array_elm_t));
    lefl_loop_array_init(&KPS_queue, KPS_queue_data, sizeof(KPS_queue_data)/sizeof(lefl_array_elm_t));
    lefl_loop_array_init(analog_historys+0, analog_history1_data, sizeof(analog_history1_data)/sizeof(lefl_array_elm_t));
    lefl_loop_array_init(analog_historys+1, analog_history2_data, sizeof(analog_history2_data)/sizeof(lefl_array_elm_t));
    lefl_loop_array_init(analog_historys+2, analog_history3_data, sizeof(analog_history3_data)/sizeof(lefl_array_elm_t));
    lefl_loop_array_init(analog_historys+3, analog_history4_data, sizeof(analog_history4_data)/sizeof(lefl_array_elm_t));

    lefl_bit_array_init(lines+0, lines1_data, sizeof(lines1_data)*8);
    lefl_bit_array_init(lines+1, lines2_data, sizeof(lines2_data)*8);
    lefl_bit_array_init(lines+2, lines3_data, sizeof(lines3_data)*8);
    lefl_bit_array_init(lines+3, lines4_data, sizeof(lines4_data)*8);

    key_attach(&(Keyboard_AdvancedKeys[0].key), KEY_EVENT_DOWN, LAMBDA(void,(void*k){fezui_keytotalcounts[0]++;Keyriggered_count[0]++;}));
    key_attach(&(Keyboard_AdvancedKeys[1].key), KEY_EVENT_DOWN, LAMBDA(void,(void*k){fezui_keytotalcounts[1]++;Keyriggered_count[1]++;}));
    key_attach(&(Keyboard_AdvancedKeys[2].key), KEY_EVENT_DOWN, LAMBDA(void,(void*k){fezui_keytotalcounts[2]++;Keyriggered_count[2]++;}));
    key_attach(&(Keyboard_AdvancedKeys[3].key), KEY_EVENT_DOWN, LAMBDA(void,(void*k){fezui_keytotalcounts[3]++;Keyriggered_count[3]++;}));

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
    lefl_loop_array_push_back(&KPS_queue, Keyriggered_count[0]+Keyriggered_count[1]+Keyriggered_count[2]+Keyriggered_count[3]);
    for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        lefl_bit_array_shift(lines+i, 1);
        if (Keyboard_AdvancedKeys[i].key.state||Keyriggered_count[i])
        {
            lefl_bit_array_set(lines+i, 0, true);
        }
        Keyriggered_count[i]=0;
    }
    fezui_kps = 0;
    for (uint8_t i = 0; i < REFRESH_RATE; i++)
    {
        fezui_kps += KPS_queue.data[i];
    }
    if (fezui_kps > UI_KPSMaximumPerSecond)
    {
        UI_KPSMaximumPerSecond = fezui_kps;
    }
    KPS_history_max=lefl_loop_array_max(&KPS_history);
    if(fezui_kps>KPS_history_max)
    {
        KPS_history_max=fezui_kps;
        KPS_history.data[KPS_history.index]=fezui_kps;
    }
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
