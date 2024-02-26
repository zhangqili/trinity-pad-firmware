/*
 * keyboard.c
 *
 *  Created on: May 21, 2023
 *      Author: xq123
 */
#include "main.h"
#include "keyboard.h"
#include "keyboard_conf.h"
#include "usbd_user.h"
#include "lfs.h"
#include "rgb.h"
#include "fezui.h"
#include "fezui_var.h"

Key Keyboard_Keys[KEY_NUM];
AdvancedKey Keyboard_AdvancedKeys[ADVANCED_KEY_NUM];
uint8_t Keyboard_KNOB_Flag;
// lefl_bit_array_t Keyboard_KeyArray;
volatile bool Keybaord_SendReport_Enable = true;
volatile bool Keybaord_Alpha_Flag;
volatile bool Keybaord_Shift_Flag;
uint8_t Keyboard_CurrentLayer = 0;
Keyboard_6KROBuffer Keyboard_ReportBuffer;

const KeyBinding default_keymap[LAYER_NUM][ADVANCED_KEY_NUM + KEY_NUM] = {
    {
        {KEY_Z, KEY_NO_MODIFIER},
        {KEY_X, KEY_NO_MODIFIER},
        {KEY_C, KEY_NO_MODIFIER},
        {KEY_V, KEY_NO_MODIFIER},
        {KEY_NO_EVENT, KEY_NO_MODIFIER},
        {KEY_NO_EVENT, KEY_NO_MODIFIER},
        {KEY_NO_EVENT, KEY_NO_MODIFIER},
        {KEY_NO_EVENT, KEY_NO_MODIFIER},
        {KEY_NO_EVENT, KEY_NO_MODIFIER},
        {KEY_NO_EVENT, KEY_NO_MODIFIER},
        {KEY_ESC, KEY_NO_MODIFIER},
        {KEY_UP_ARROW, KEY_NO_MODIFIER},
        {KEY_DOWN_ARROW, KEY_NO_MODIFIER},
    },
};

KeyBinding keymap[LAYER_NUM][ADVANCED_KEY_NUM + KEY_NUM];

AdvancedKey Keyboard_AdvancedKeys[ADVANCED_KEY_NUM] =
    {
        {.key.id = 0},
        {.key.id = 1},
        {.key.id = 2},
        {.key.id = 3},
};

int keyboard_6KRObuffer_add(Keyboard_6KROBuffer *buf, KeyBinding key)
{
    buf->buffer[0] |= key.modifier;
    if (key.keycode != KEY_NO_EVENT && buf->keynum < 6)
    {
        buf->buffer[2 + buf->keynum] = key.keycode;
        buf->keynum++;
        return 0;
    }
    else
    {
        return 1;
    }
}
void keyboard_6KRObuffer_send(Keyboard_6KROBuffer *buf)
{
    hid_keyboard_send(buf->buffer);
}
void keyboard_6KRObuffer_clear(Keyboard_6KROBuffer *buf)
{
    memset(buf, 0, sizeof(Keyboard_6KROBuffer));
}

void keyboard_init()
{
    memcpy(keymap, default_keymap, sizeof(keymap));
}

void keyboard_factory_reset()
{
    memcpy(keymap, default_keymap, sizeof(keymap));
    for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        // advanced_key_set_range(Keyboard_AdvancedKeys+i, 4000, 0);
        // advanced_key_set_deadzone(Keyboard_AdvancedKeys+i, 0.01, 0.1);
        Keyboard_AdvancedKeys[i].mode = DEFAULT_ADVANCED_KEY_MODE;
        Keyboard_AdvancedKeys[i].trigger_distance = DEFAULT_TRIGGER_DISTANCE;
        Keyboard_AdvancedKeys[i].release_distance = DEFAULT_RELEASE_DISTANCE;
        Keyboard_AdvancedKeys[i].schmitt_parameter = DEFAULT_SCHMITT_PARAMETER;
        // Keyboard_AdvancedKeys[i].activation_value = 0.3;
        // Keyboard_AdvancedKeys[i].lower_deadzone = 0.32;
        advanced_key_set_deadzone(Keyboard_AdvancedKeys + i, DEFAULT_UPPER_DEADZONE, DEFAULT_LOWER_DEADZONE);
        // Keyboard_AdvancedKeys[i].phantom_lower_deadzone = 0.32;
        // Keyboard_AdvancedKeys[i].key.keycode = default_keymap[0][Keyboard_AdvancedKeys[i].key.id];
    }
    rgb_recovery();
    keyboard_save();
    keyboard_system_reset();
}
void keyboard_system_reset()
{
    //__set_FAULTMASK(1);
    __disable_irq();
    NVIC_SystemReset();
}

void key_update1(Key *key, bool state)
{
    if ((!(key->state)) && state)
    {
        if (Keybaord_SendReport_Enable)
        {
            Keybaord_Alpha_Flag = !Keybaord_Alpha_Flag;
        }
        if (key->key_cb[KEY_EVENT_DOWN])
        {
            key->key_cb[KEY_EVENT_DOWN](key);
        }
    }
    if ((key->state) && (!state))
    {
        if (key->key_cb[KEY_EVENT_UP])
            key->key_cb[KEY_EVENT_UP](key);
    }
    key->state = state;
}

void key_update2(Key *key, bool state)
{
    if ((!(key->state)) && state)
    {
        if (Keybaord_SendReport_Enable)
        {
            Keybaord_Shift_Flag = !Keybaord_Shift_Flag;
        }
        if (key->key_cb[KEY_EVENT_DOWN])
        {
            key->key_cb[KEY_EVENT_DOWN](key);
        }
    }
    if ((key->state) && (!state))
    {
        if (key->key_cb[KEY_EVENT_UP])
            key->key_cb[KEY_EVENT_UP](key);
    }
    key->state = state;
}

void keyboard_scan()
{
    key_update(&KEY_FN_K1, FN_K1_READ);
    key_update(&KEY_FN_K2, FN_K2_READ);
    key_update(&KEY_FN_K3, FN_K3_READ);
    key_update(&KEY_FN_K4, FN_K4_READ);
    key_update1(&KEY_FN_K5, FN_K5_READ);
    key_update2(&KEY_FN_K6, FN_K6_READ);
    key_update(&KEY_KNOB, KNOB_READ);
    if (Keyboard_KNOB_Flag)
    {
        Keyboard_KNOB_Flag--;
    }
    if (!Keyboard_KNOB_Flag)
    {
        key_update(&KEY_KNOB_CLOCKWISE, false);
        key_update(&KEY_KNOB_ANTICLOCKWISE, false);
    }
    if (Keybaord_Shift_Flag && Keybaord_Alpha_Flag)
    {
        fezui_link_frame_navigate(&mainframe, &menupage);
        Keybaord_Shift_Flag = false;
        Keybaord_Alpha_Flag = false;
    }
}

void keyboard_recovery()
{
    // mount the filesystem
    int err = lfs_mount(&lfs_w25qxx, &cfg);
    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
    if (err)
    {
        lfs_format(&lfs_w25qxx, &cfg);
        lfs_mount(&lfs_w25qxx, &cfg);
    }
    lfs_file_open(&lfs_w25qxx, &lfs_file_w25qxx, "config1.dat", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_rewind(&lfs_w25qxx, &lfs_file_w25qxx);
    for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        lfs_file_read(&lfs_w25qxx, &lfs_file_w25qxx, &(Keyboard_AdvancedKeys[i].key.id), sizeof(Keyboard_AdvancedKeys[i].key.id));
        lfs_file_read(&lfs_w25qxx, &lfs_file_w25qxx, ((void*)(&Keyboard_AdvancedKeys[i])) + sizeof(Key), sizeof(AdvancedKey) - sizeof(Key));
    }
    lfs_file_read(&lfs_w25qxx, &lfs_file_w25qxx, keymap, sizeof(keymap));
    lfs_file_read(&lfs_w25qxx, &lfs_file_w25qxx, &RGB_GlobalConfig, sizeof(RGB_GlobalConfig));
    lfs_file_read(&lfs_w25qxx, &lfs_file_w25qxx, &RGB_Configs, sizeof(RGB_Configs));
    // remember the storage is not updated until the file is closed successfully
    lfs_file_close(&lfs_w25qxx, &lfs_file_w25qxx);
    printf("recovery = %d", err);
    // release any resources we were using
    lfs_unmount(&lfs_w25qxx);
    // print the boot count
}

void keyboard_save()
{

    // mount the filesystem
    int err = lfs_mount(&lfs_w25qxx, &cfg);
    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
    if (err)
    {
        lfs_format(&lfs_w25qxx, &cfg);
        lfs_mount(&lfs_w25qxx, &cfg);
    }
    // read current count
    lfs_file_open(&lfs_w25qxx, &lfs_file_w25qxx, "config1.dat", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_rewind(&lfs_w25qxx, &lfs_file_w25qxx);
    for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        lfs_file_write(&lfs_w25qxx, &lfs_file_w25qxx, &(Keyboard_AdvancedKeys[i].key.id), sizeof(Keyboard_AdvancedKeys[i].key.id));
        lfs_file_write(&lfs_w25qxx, &lfs_file_w25qxx, ((void*)(&Keyboard_AdvancedKeys[i])) + sizeof(Key), sizeof(AdvancedKey) - sizeof(Key));
    }
    lfs_file_write(&lfs_w25qxx, &lfs_file_w25qxx, keymap, sizeof(keymap));
    lfs_file_write(&lfs_w25qxx, &lfs_file_w25qxx, &RGB_GlobalConfig, sizeof(RGB_GlobalConfig));
    lfs_file_write(&lfs_w25qxx, &lfs_file_w25qxx, &RGB_Configs, sizeof(RGB_Configs));
    // remember the storage is not updated until the file is closed successfully
    err = lfs_file_close(&lfs_w25qxx, &lfs_file_w25qxx);
    printf("save = %d", err);
    // release any resources we were using
    lfs_unmount(&lfs_w25qxx);
    // print the boot count
}

void keyboard_send_report()
{
    keyboard_6KRObuffer_clear(&Keyboard_ReportBuffer);
    Keyboard_CurrentLayer = 0;
    if (Keybaord_Shift_Flag)
    {
        Keyboard_CurrentLayer = 1;
    }
    if (Keybaord_Alpha_Flag)
    {
        Keyboard_CurrentLayer = 2;
    }
    // keyboard_6KRObuffer_add(&Keyboard_ReportBuffer,(KeyBinding){KEY_E,KEY_NO_MODIFIER});
    for (int i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        if (Keyboard_AdvancedKeys[i].key.state)
        {
            keyboard_6KRObuffer_add(&Keyboard_ReportBuffer, keymap[Keyboard_CurrentLayer][i]);
        }
    }
    for (int i = 0; i < KEY_NUM; i++)
    {
        if (Keyboard_Keys[i].state)
        {
            keyboard_6KRObuffer_add(&Keyboard_ReportBuffer, keymap[Keyboard_CurrentLayer][i + ADVANCED_KEY_NUM]);
        }
    }
    if (Keybaord_SendReport_Enable)
    {
        keyboard_6KRObuffer_send(&Keyboard_ReportBuffer);
    }
    // uint8_t mousebuffer[4]={0};
    // mousebuffer[0]&=(~BIT(0));
    // mousebuffer[0]|=Keyboard_AdvancedKeys[0].key.state?1:0;
    // hid_mouse_send(mousebuffer);
    //     Keyboard_ReportBuffer[0] = 1
    //     for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
    //     {
    //         //lefl_bit_array_set_or(&Keyboard_KeyArray, Keyboard_AdvancedKeys[i].key.id & 0xFF, Keyboard_AdvancedKeys[i].key.state);
    //     }
    //
    //     USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS,Keyboard_ReportBuffer,USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);
    //   int16_t index, bitIndex, keycode, layer;
    //   layer = Keyboard_AdvancedKeys[49].key.state?1:0; //Fn key
    //   memset(Keyboard_ReportBuffer, 0, USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);
    //   Keyboard_ReportBuffer[0] = 1;
    //   for (int i = 0; i < ADVANCED_KEY_NUM; i++)
    //   {
    //   	keycode = keymap[layer][Keyboard_AdvancedKeys[i].key.id];
    //   	index = (int16_t)(keycode/8 + 1);// +1 for modifier
    //   	bitIndex = (int16_t)(keycode%8);
    //       if (bitIndex < 0)
    //       {
    //           index -= 1;
    //           bitIndex += 8;
    //       } else if (keycode > 100)
    //           continue;
    //       if(Keyboard_AdvancedKeys[i].key.state)Keyboard_ReportBuffer[index + 1] |= 1 << (bitIndex); // +1 for Report-ID
    //   }
    //   //debug
    //     memset(Keyboard_ReportBuffer, 0, USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);
    //   USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS,Keyboard_ReportBuffer,16+1);
}
