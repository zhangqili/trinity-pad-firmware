/*
 * fezui_config.c
 *
 *      Author: xq123
 */

#include "fezui.h"
#include "fezui_var.h"
#include "fram.h"
#include "lfs.h"
#include "record.h"
#include "main.h"

#define MD_OLED_RST_Clr() GPIO_WriteBit(DISP_RES_GPIO_Port, DISP_RES_Pin, 0)
#define MD_OLED_RST_Set() GPIO_WriteBit(DISP_RES_GPIO_Port, DISP_RES_Pin, 1)

static fezui_animation_base_t frame_animation = {.duration = DEFAULT_ANIMATION_DURATION, .easing_func = fezui_animation_linear_ease, .mode = EASE_INOUT};

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_riscv_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr);

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_BYTE_SEND:
        // GPIO_WriteBit(DISP_CS_GPIO_Port, DISP_CS_Pin, Bit_RESET);

        for (uint16_t i = 0; i < arg_int; i++)
        { // 写数据
            while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET)
                ;
            while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
                ;
            SPI_I2S_SendData(SPI1, ((uint8_t *)arg_ptr)[i]);
        }
        // GPIO_WriteBit(DISP_CS_GPIO_Port, DISP_CS_Pin, Bit_SET);
        break;
    case U8X8_MSG_BYTE_INIT:
        break;
    case U8X8_MSG_BYTE_SET_DC:
        GPIO_WriteBit(DISP_DC_GPIO_Port, DISP_DC_Pin, arg_int);
        break;
    case U8X8_MSG_BYTE_START_TRANSFER:
        break;
    case U8X8_MSG_BYTE_END_TRANSFER:
        break;
    default:
        return 0;
    }
    return 1;
}

uint8_t u8x8_riscv_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
                                  U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
                                  U8X8_UNUSED void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
        break;
    case U8X8_MSG_DELAY_MILLI:
        Delay_Ms(arg_int);
        break;
    case U8X8_MSG_GPIO_CS:
        break;
    case U8X8_MSG_GPIO_DC:
        GPIO_WriteBit(DISP_DC_GPIO_Port, DISP_DC_Pin, arg_int);
        break;
    case U8X8_MSG_GPIO_RESET:
        break;
    }
    return 1;
}

void fezui_input(uint16_t in)
{
    fezui_frame_input(&g_mainframe, (void *)&in);
    fezui.screensaver_countdown = fezui.screensaver_timeout;
}

static void key_up_cb()
{
    fezui_input(KEY_UP_ARROW);
}

static void key_down_cb()
{
    fezui_input(KEY_DOWN_ARROW);
}

static void key_enter_cb()
{
    fezui_input(KEY_ENTER);
}

static void key_space_cb()
{
    fezui_input(KEY_SPACEBAR);
}

static void key_esc_cb()
{
    fezui_input(KEY_ESC);
}

void fezui_init()
{
    MD_OLED_RST_Set();
    u8g2_Setup_ssd1306_128x64_noname_f(&(fezui.u8g2), U8G2_R0, u8x8_byte_4wire_hw_spi,
                                       u8x8_riscv_gpio_and_delay);
    u8g2_InitDisplay(&(fezui.u8g2));
    u8g2_SetPowerSave(&(fezui.u8g2), 0);
    // u8g2_SetBitmapMode(&(fezui.u8g2), 1);
    u8g2_SetFontMode(&(fezui.u8g2), 1);
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_tom_thumb_4x6_mf);
    u8g2_SetFontPosBottom(&fezui.u8g2);
    u8g2_SetBitmapMode(&fezui.u8g2, 1);

    fezui_POST();

    debugpage_init();
    menupage_init();
    launcherpage_init();
    settingspage_init();
    calibrationpage_init();
    advancedconfigpage_init();
    keymappage_init();
    keylistpage_init();
    rgbconfigpage_init();
    displayconfigpage_init();
    panelpage_init();
    advancedsettingspage_init();

    fezui_frame_init(&g_mainframe, &launcherpage, &frame_animation);
    key_attach(&KEY_KNOB, KEY_EVENT_DOWN, key_space_cb);
    key_attach(&KEY_KNOB_CLOCKWISE, KEY_EVENT_DOWN, key_up_cb);
    key_attach(&KEY_KNOB_ANTICLOCKWISE, KEY_EVENT_DOWN, key_down_cb);
    key_attach(&KEY_FN_K5, KEY_EVENT_DOWN, key_enter_cb);
    key_attach(&KEY_FN_K6, KEY_EVENT_DOWN, key_esc_cb);
    // Keybaord_SendReport_Enable=false;
    //fezui_frame_navigate(&g_mainframe, &calibrationpage);
    // fezui_frame_navigate(&g_mainframe, &oscilloscopepage);
}

static float screensaver_countdown = 0;
void fezui_timer_handler()
{
    fezui_frame_tick(&g_mainframe);
    CONVERGE_TO(screensaver_countdown, (float)fezui.screensaver_countdown,0.02);
    fezui_notification_update(&fezui, &fezui_notification);
    fezui_cursor_move(&fezui, &g_fezui_cursor, &g_target_cursor);
    // fezui_animated_cursor_update(&animated_cursor);
    g_kps_history_max = loop_array_max(&g_kps_history);
    // fezui_save_counts();

    uint8_t key_pressed_num = 0;
    for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        key_pressed_num += g_keyboard_advanced_keys[i].key.state;
    }
    for (uint8_t i = 0; i < KEY_NUM; i++)
    {
        key_pressed_num += g_keyboard_keys[i].state;
    }

    if (g_kps_history_max || key_pressed_num)
    {
        fezui.screensaver_countdown = fezui.screensaver_timeout;
    }
}

void fezui_render_handler()
{
    u8g2_ClearBuffer(&(fezui.u8g2));
    u8g2_SetDrawColor(&(fezui.u8g2), 1);
    fezui_frame_draw(&g_mainframe);
    fezui_draw_notification(&fezui, &fezui_notification);
    if (fezui.invert)
    {
        u8g2_SetDrawColor(&(fezui.u8g2), 2);
        u8g2_DrawBox(&(fezui.u8g2), 0, 0, WIDTH, HEIGHT);
    }
    if (fezui.screensaver_timeout)
    {
        fezui_veil_full_screen(&(fezui), (screensaver_countdown) < 7 ? 7 - screensaver_countdown : 0);
        u8g2_SetPowerSave(&(fezui.u8g2), (screensaver_countdown<1));
    }
    if (fezui.show_fps)
    {
        u8g2_SetDrawColor(&(fezui.u8g2), 1);
        u8g2_DrawBox(&fezui.u8g2, 95 + 14, 0, WIDTH - 95 - 14, 11);
        u8g2_SetDrawColor(&(fezui.u8g2), 2);
        u8g2_SetFont(&(fezui.u8g2), fez_font_6x10_m);
        u8g2_DrawUTF8(&(fezui.u8g2), 95 + 15, 10, g_fpsstr);
    }
    u8g2_SendBuffer(&(fezui.u8g2));
    g_fezui_fps++;
}

#define CHAR_HEIGHT 8
#define CHAR_WIDTH 5
void fezui_POST()
{
    u8g2_SetFont(&fezui.u8g2, u8g2_font_3x3basic_tr);
    u8g2_DrawUTF8X2(&fezui.u8g2, 0, CHAR_HEIGHT, "FEZUI");
    u8g2_SetFont(&fezui.u8g2, u8g2_font_5x7_tf);
    u8g2_DrawHLine(&fezui.u8g2, 0, CHAR_HEIGHT, WIDTH);
    u8log_WriteString(&g_u8log, "Mounting Flash...");
    u8g2_DrawLog(&fezui.u8g2, 0, CHAR_HEIGHT * 2, &g_u8log);
    u8g2_SendBuffer(&fezui.u8g2);
    // mount the filesystem
    int err = lfs_mount(&g_lfs, &g_lfs_config);
    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
    if (err)
    {
        err = lfs_format(&g_lfs, &g_lfs_config);
        lfs_mount(&g_lfs, &g_lfs_config);
        u8log_WriteString(&g_u8log, " [Failed]\n");
        u8log_WriteString(&g_u8log, "Formating Flash...");
        u8g2_DrawLog(&fezui.u8g2, 0, CHAR_HEIGHT * 2, &g_u8log);
        u8g2_SendBuffer(&fezui.u8g2);
        if (err)
        {
            u8log_WriteString(&g_u8log, " [Failed]\n");
        }
        else
        {
            u8log_WriteString(&g_u8log, " [OK]\n");
        }
    }
    else
    {
        u8log_WriteString(&g_u8log, " [OK]\n");
    }
    u8g2_DrawLog(&fezui.u8g2, 0, CHAR_HEIGHT * 2, &g_u8log);
    u8g2_SendBuffer(&fezui.u8g2);
    // remember the storage is not updated until the file is closed successfully
    lfs_file_close(&g_lfs, &g_lfs_file);
    // release any resources we were using
    lfs_unmount(&g_lfs);
    u8log_WriteString(&g_u8log, "Testing FRAM...");
    u8g2_SendBuffer(&fezui.u8g2);
    uint32_t fram_id = fram_read_device_id();
    if ((fram_id) && (~fram_id))
    {
        u8log_WriteString(&g_u8log, " [OK]\n");
    }
    u8log_WriteString(&g_u8log, "Recoverying data...");
    u8g2_SendBuffer(&fezui.u8g2);
    u8g2_DrawLog(&fezui.u8g2, 0, CHAR_HEIGHT * 2, &g_u8log);
    fezui_recovery();
    fezui_apply(&fezui);
    u8log_WriteString(&g_u8log, " [OK]\n");
    u8g2_SendBuffer(&fezui.u8g2);
    u8g2_DrawLog(&fezui.u8g2, 0, CHAR_HEIGHT * 2, &g_u8log);
}

#define ADD_STRING(str) strings[index++] = (str);
void keyid_prase(uint16_t id, char *str, uint16_t str_len)
{
    const char *strings[16] = {0};
    uint8_t index = 0;
    memset(str, 0, str_len);
    switch (id & 0xFF)
    {
    case MOUSE_COLLECTION:
        ADD_STRING(g_hid_usage_names[MOUSE_COLLECTION + 8 + KEY_MODIFIER(id)]);
        break;
    case LAYER_CONTROL:
        ADD_STRING("Layer Control");
        break;
    case KEY_USER:
        ADD_STRING("User Defined");
        break;
    case KEY_SYSTEM:
        ADD_STRING("System Defined");
        break;
    case KEY_TRANSPARENT:
        ADD_STRING("Transparent");
        break;
    default:
        for (uint8_t i = 0; i < 8; i++)
        {
            if ((id >> 8) & BIT(i))
            {
                ADD_STRING(g_hid_usage_names[i]);
            }
        }
        if (id & 0xFF)
        {
            
            ADD_STRING(g_hid_usage_names[(id & 0xFF) + 8]);
        }
        break;
    }
    if (index)
    {
        strcat(str, strings[0]);
        for (int i = 1; i < index; i++)
        {
            strcat(str, " + ");
            strcat(str, strings[i]);
        }
        
    }
    else
    {
        strcat(str, "None");
    }
}

void fezui_reset()
{
    fezui.invert = 0;
    fezui.contrast = 255;
    fezui.screensaver_countdown = 60;
    fezui.screensaver_timeout = 60;
    fezui.speed = 0.05;
    fezui.lang = LANG_EN;
    fezui_apply(&fezui);
}

void fezui_save()
{
    // mount the filesystem
    int err = lfs_mount(&g_lfs, &g_lfs_config);
    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
    if (err)
    {
        lfs_format(&g_lfs, &g_lfs_config);
        lfs_mount(&g_lfs, &g_lfs_config);
    }
    // read current count
    lfs_file_open(&g_lfs, &g_lfs_file, "fezui.dat", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_rewind(&g_lfs, &g_lfs_file);
    lfs_file_write(&g_lfs, &g_lfs_file, &fezui.contrast, sizeof(fezui.contrast));
    lfs_file_write(&g_lfs, &g_lfs_file, &fezui.invert, sizeof(fezui.invert));
    lfs_file_write(&g_lfs, &g_lfs_file, &fezui.speed, sizeof(fezui.speed));
    lfs_file_write(&g_lfs, &g_lfs_file, &fezui.screensaver_timeout, sizeof(fezui.screensaver_timeout));
    lfs_file_write(&g_lfs, &g_lfs_file, &fezui.show_fps, sizeof(fezui.show_fps));
    lfs_file_write(&g_lfs, &g_lfs_file, &fezui.lang, sizeof(fezui.lang));
    // remember the storage is not updated until the file is closed successfully
    err = lfs_file_close(&g_lfs, &g_lfs_file);
    printf("save = %d", err);
    // release any resources we were using
    lfs_unmount(&g_lfs);
    // print the boot count
}

void fezui_recovery()
{
    // mount the filesystem
    int err = lfs_mount(&g_lfs, &g_lfs_config);
    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
    if (err)
    {
        lfs_format(&g_lfs, &g_lfs_config);
        lfs_mount(&g_lfs, &g_lfs_config);
    }
    lfs_file_open(&g_lfs, &g_lfs_file, "fezui.dat", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_rewind(&g_lfs, &g_lfs_file);
    lfs_file_read(&g_lfs, &g_lfs_file, &fezui.contrast, sizeof(fezui.contrast));
    lfs_file_read(&g_lfs, &g_lfs_file, &fezui.invert, sizeof(fezui.invert));
    lfs_file_read(&g_lfs, &g_lfs_file, &fezui.speed, sizeof(fezui.speed));
    lfs_file_read(&g_lfs, &g_lfs_file, &fezui.screensaver_timeout, sizeof(fezui.screensaver_timeout));
    lfs_file_read(&g_lfs, &g_lfs_file, &fezui.show_fps, sizeof(fezui.show_fps));
    lfs_file_read(&g_lfs, &g_lfs_file, &fezui.lang, sizeof(fezui.lang));
    // remember the storage is not updated until the file is closed successfully
    lfs_file_close(&g_lfs, &g_lfs_file);
    printf("recovery = %d", err);
    // release any resources we were using
    lfs_unmount(&g_lfs);
    // print the boot count
}