/*
 * Copyright (c) 2024 Zhangqi Li (@zhangqili)
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "command.h"
#include "keyboard.h"
#include "keyboard_def.h"
#include "rgb.h"
#include "string.h"
#include "packet.h"

void unload_cargo(uint8_t *buf)
{
    PacketData* packet = (PacketData*)buf;
    switch (packet->type)
    {
    case PACKET_DATA_ADVANCED_KEY: // Advanced Key
        {
            PacketAdvancedKey* packet = (PacketAdvancedKey*)buf;
            uint16_t key_index = g_keyboard_advanced_keys_inverse_mapping[packet->index];
            g_keyboard_advanced_keys[key_index].config.mode = packet->mode;
            g_keyboard_advanced_keys[key_index].config.activation_value = packet->activation_value;
            g_keyboard_advanced_keys[key_index].config.deactivation_value = packet->deactivation_value;
            g_keyboard_advanced_keys[key_index].config.trigger_distance = packet->trigger_distance;
            g_keyboard_advanced_keys[key_index].config.release_distance = packet->release_distance;
            g_keyboard_advanced_keys[key_index].config.trigger_speed = packet->trigger_speed;
            g_keyboard_advanced_keys[key_index].config.release_speed = packet->release_speed;
            g_keyboard_advanced_keys[key_index].config.upper_deadzone = packet->upper_deadzone;
            g_keyboard_advanced_keys[key_index].config.lower_deadzone = packet->lower_deadzone;
            //g_keyboard_advanced_keys[key_index].config.upper_bound = packet->config.upper_bound);
            //g_keyboard_advanced_keys[key_index].config.lower_bound = packet->config.lower_bound);
        }
        break;
    case PACKET_DATA_RGB_SWITCH: // Global LED
        {
            PacketRGBSwitch* packet = (PacketRGBSwitch*)buf;
            g_rgb_switch = packet->state;
        }
        break;
    case PACKET_DATA_RGB_CONFIG: // LED
        {
            PacketRGBConfigs* packet = (PacketRGBConfigs*)buf;
            for (uint8_t i = 0; i < packet->length; i++)
            {
                uint16_t key_index = g_rgb_mapping[packet->data[i].index];
                if (key_index < RGB_NUM)
                {
                    g_rgb_configs[key_index].mode  = packet->data[i].mode;
                    g_rgb_configs[key_index].rgb.r = packet->data[i].r;
                    g_rgb_configs[key_index].rgb.g = packet->data[i].g;
                    g_rgb_configs[key_index].rgb.b = packet->data[i].b;
                    rgb_to_hsv(&g_rgb_configs[key_index].hsv, &g_rgb_configs[key_index].rgb);
                    g_rgb_configs[key_index].speed = packet->data[i].speed;
                }
            }
        }
        break;
    case PACKET_DATA_KEYMAP: // Keymap
        {
            PacketKeymap* packet = (PacketKeymap*)buf;
            if (packet->layer < LAYER_NUM && packet->start + packet->length <= (ADVANCED_KEY_NUM + KEY_NUM))
            {
                memcpy(&g_keymap[packet->layer][packet->start], packet->keymap, packet->length * sizeof(Keycode));
            }
        }
        break;
    case PACKET_DATA_DYNAMIC_KEY: // Dynamic Key
        {
            PacketDynamicKey *packet = (PacketDynamicKey *)buf;
            if (packet->index<DYNAMIC_KEY_NUM)
            {
                memcpy(&g_keyboard_dynamic_keys[packet->index], &packet->dynamic_key, sizeof(DynamicKey));
            }
        }
        break;
    default:
        break;
    }
}

static uint16_t page_index;
void start_load_cargo(void)
{
    page_index = 0;
}

int load_cargo(void)
{
    uint8_t buf[64] = {0};
    PacketData *packet = (PacketData *)buf;
    packet->code = 0xFF;
    // max 62 remain
    switch ((page_index >> 8) & 0xFF)
    {
    case PACKET_DATA_ADVANCED_KEY: // Advanced Key
        {
            PacketAdvancedKey *packet = (PacketAdvancedKey *)buf;
            packet->type = PACKET_DATA_ADVANCED_KEY;
            uint8_t key_index = page_index & 0xFF;
            packet->index = g_keyboard_advanced_keys[key_index].key.id;
            packet->mode = g_keyboard_advanced_keys[key_index].config.mode;
            memcpy(&packet->activation_value, &g_keyboard_advanced_keys[key_index].config.activation_value, sizeof(AnalogValue) * 10);
            if (!hid_send(buf, 63))
            {
                page_index++;
                if ((page_index & 0xFF) >= ADVANCED_KEY_NUM)
                {
                    page_index = 0x0100;
                }
            }
        }
        return 1;
        break;
    case PACKET_DATA_RGB_SWITCH: // Global LED
        {
            PacketRGBSwitch *packet = (PacketRGBSwitch *)buf;
            packet->type = PACKET_DATA_RGB_SWITCH;
            packet->state = g_rgb_switch;
            if (!hid_send(buf,63))
            {
                page_index = 0x0200;
            }
        }
        return 1;
        break;
    case PACKET_DATA_RGB_CONFIG: // LED
        {
            PacketRGBConfigs *packet = (PacketRGBConfigs *)buf;
            uint8_t key_base_index = (page_index & 0xFF)*6;
            packet->type = PACKET_DATA_RGB_CONFIG;
            packet->length = key_base_index + 6 <= RGB_NUM ? 6 : RGB_NUM - key_base_index;
            for (uint8_t i = 0; i < packet->length; i++)
            {
                uint8_t key_index = key_base_index + i;
                if (key_index < RGB_NUM)
                {
                    uint8_t rgb_index = g_rgb_mapping[g_keyboard_advanced_keys[g_keyboard_advanced_keys_inverse_mapping[key_index]].key.id];
                    packet->data[i].index = key_index;
                    packet->data[i].mode = g_rgb_configs[rgb_index].mode;
                    packet->data[i].r = g_rgb_configs[rgb_index].rgb.r;
                    packet->data[i].g = g_rgb_configs[rgb_index].rgb.g;
                    packet->data[i].b = g_rgb_configs[rgb_index].rgb.b;
                    memcpy(&packet->data[i].speed, &g_rgb_configs[rgb_index].speed, sizeof(float));
                }
            }
            if (!hid_send(buf,63))
            {
                page_index++;
                if ((page_index & 0xFF)*6>=RGB_NUM)
                {
                    page_index = 0x0300;
                }
            }
        }
        return 1;
        break;
#define LAYER_PAGE_LENGTH 16
#define LAYER_PAGE_EXPECTED_NUM ((ADVANCED_KEY_NUM + KEY_NUM + 15) / 16)
    case PACKET_DATA_KEYMAP: // Keymap
        {
            PacketKeymap *packet = (PacketKeymap *)buf;
            packet->type = PACKET_DATA_KEYMAP;
            uint8_t layer_index = (page_index & 0xFF) / LAYER_PAGE_EXPECTED_NUM;
            uint8_t layer_page_index = (page_index & 0xFF) % LAYER_PAGE_EXPECTED_NUM;
            packet->layer = layer_index;
            packet->start = layer_page_index*LAYER_PAGE_LENGTH;
            packet->length = (layer_page_index + 1)*LAYER_PAGE_LENGTH <= (ADVANCED_KEY_NUM + KEY_NUM) ? 
                LAYER_PAGE_LENGTH : 
                (ADVANCED_KEY_NUM + KEY_NUM) - layer_page_index*LAYER_PAGE_LENGTH;
            memcpy(&packet->keymap, &g_keymap[layer_index][packet->start], packet->length*sizeof(Keycode));
            if (!hid_send(buf,63))
            {
                page_index++;
                if ((page_index & 0xFF)>=LAYER_NUM*LAYER_PAGE_EXPECTED_NUM)
                {
                    page_index = 0x0400;
                }
            }
        }
        return 1;
        break;
    case PACKET_DATA_DYNAMIC_KEY: // Dynamic Key
        PacketDynamicKey *packet = (PacketDynamicKey *)buf;
        packet->type = PACKET_DATA_DYNAMIC_KEY;
        uint8_t dk_index = (page_index & 0xFF);
        if (dk_index >= DYNAMIC_KEY_NUM)
        {
            page_index=0x8000;
            return 1;
        }
        packet->index = dk_index;
        memcpy(&packet->dynamic_key,&g_keyboard_dynamic_keys[dk_index],sizeof(DynamicKey));
        if (!hid_send(buf,63))
        {
            page_index++;
        }
        return 1;
        break;
    case 0x80: // config index
        buf[1] = 0x80;
        buf[2] = g_current_config_index;
        if (!hid_send(buf,63))
        {
            page_index = 0xFFFF;
        }
        return 1;
        break;
    default:
        return 0;
        break;
    }
}

void command_prase(uint8_t *buf, uint8_t len)
{
    UNUSED(len);
    PacketBase *packet = (PacketBase *)buf;
    switch (packet->code)
    {
    case 0x80: // Save
        keyboard_save();
        break;
    case 0x81: // System Reset
        keyboard_reboot();
        break;
    case 0x82: // Factory Reset
        keyboard_factory_reset();
        break;
    case 0x90:
        keyboard_set_config_index(buf[1]);
        break;
    case 0xB0: // Set Debug
        g_keyboard_state = buf[1];
        break;
    case 0xB1: // upload config
        g_keyboard_state = KEYBOARD_UPLOAD_CONFIG;
        start_load_cargo();
        break;
    case 0xFF: // Legacy
        unload_cargo(buf);
        break;
    default:
        break;
    }
}

__WEAK int hid_send(uint8_t *report, uint16_t len)
{
    UNUSED(report);
    UNUSED(len);
    return 0;
}