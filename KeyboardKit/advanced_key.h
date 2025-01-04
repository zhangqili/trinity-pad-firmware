/*
 * Copyright (c) 2024 Zhangqi Li (@zhangqili)
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#ifndef ADVANCED_KEY_H_
#define ADVANCED_KEY_H_

#include "key.h"
#include "keyboard_conf.h"

#ifdef USE_FLOAT_VALUE
typedef float AnalogValue;
#else
typedef uint AnalogValue;
#endif

typedef enum
{
    KEY_DIGITAL_MODE,
    KEY_ANALOG_NORMAL_MODE,
    KEY_ANALOG_RAPID_MODE,
    KEY_ANALOG_SPEED_MODE
} KeyMode;
typedef enum
{
    KEY_NO_CALIBRATION,
    KEY_AUTO_CALIBRATION_POSITIVE,
    KEY_AUTO_CALIBRATION_NEGATIVE,
    KEY_AUTO_CALIBRATION_UNDEFINED,
} CalibrationMode;

typedef struct __AdvancedKey
{
    Key key;
    AnalogValue value;
    AnalogValue raw;
    AnalogValue maximum;
    AnalogValue minimum;
    
    uint8_t mode;
    uint8_t calibration_mode;
    
    AnalogValue activation_value;
    AnalogValue deactivation_value;

    AnalogValue trigger_distance;
    AnalogValue release_distance;
    AnalogValue trigger_speed;
    AnalogValue release_speed;
    AnalogValue upper_deadzone;
    AnalogValue lower_deadzone;
    AnalogValue upper_bound;
    AnalogValue lower_bound;
} AdvancedKey;

void advanced_key_init(AdvancedKey *key);
void advanced_key_update(AdvancedKey *key, AnalogValue value);
void advanced_key_update_raw(AdvancedKey *key, AnalogValue value);
void advanced_key_update_state(AdvancedKey *key, bool state);
AnalogValue advanced_key_normalize(AdvancedKey *key, AnalogValue value);
void advanced_key_set_range(AdvancedKey *key, AnalogValue upper, AnalogValue lower);
void advanced_key_reset_range(AdvancedKey* key, AnalogValue value);
void advanced_key_set_deadzone(AdvancedKey *key, AnalogValue upper, AnalogValue lower);

#endif