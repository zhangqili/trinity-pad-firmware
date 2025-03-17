/*
 * Copyright (c) 2024 Zhangqi Li (@zhangqili)
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#ifndef STORAGE_H_
#define STORAGE_H_

#include "stdint.h"
#include "keyboard_def.h"
#include "advanced_key.h"

#ifndef STORAGE_CONFIG_FILE_NUM
#define STORAGE_CONFIG_FILE_NUM 4
#endif

typedef struct __AdvancedKeyConfigurationNormalized
{
    uint8_t mode;
    uint8_t calibration_mode;
    uint8_t reserved[2];
    float activation_value;
    float deactivation_value;
    float trigger_distance;
    float release_distance;
    float trigger_speed;
    float release_speed;
    float upper_deadzone;
    float lower_deadzone;
    float upper_bound;
    float lower_bound;
} AdvancedKeyConfigurationNormalized;

void advanced_key_config_normalize(AdvancedKeyConfigurationNormalized* buffer, AdvancedKeyConfiguration* config);
void advanced_key_config_anti_normalize(AdvancedKeyConfiguration* config, AdvancedKeyConfigurationNormalized* buffer);
int storage_mount(void);
void storage_unmount(void);
uint8_t storage_read_config_index(void);
void storage_save_config_index(uint8_t index);
void storage_read_config(uint8_t index);
void storage_save_config(uint8_t index);

#endif