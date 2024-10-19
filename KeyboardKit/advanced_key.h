#ifndef ADVANCED_KEY_H_
#define ADVANCED_KEY_H_

#include "key.h"
#include "keyboard_conf.h"

#ifdef USE_FLOAT_VALUE
typedef float KeyValueType;
#else
typedef uint KeyValueType;
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
} CalibrationMode;

typedef struct __AdvancedKey
{
    Key key;
    uint8_t mode;
    uint8_t calibration_mode;
    float value;
    float raw;
    

    float activation_value;
    float phantom_lower_deadzone;

    float trigger_distance;
    float release_distance;
    float schmitt_parameter;
    float maximum;
    float minimum;
    float trigger_speed;
    float release_speed;
    float upper_deadzone;
    float lower_deadzone;
    float upper_bound;
    float lower_bound;
} AdvancedKey;

void advanced_key_init(AdvancedKey *key);
void advanced_key_update(AdvancedKey *key, float value);
void advanced_key_update_raw(AdvancedKey *key, float value);
void advanced_key_update_state(AdvancedKey *key, bool state);
float advanced_key_normalize(AdvancedKey *key, float value);
void advanced_key_set_range(AdvancedKey *key, float upper, float lower);
void advanced_key_reset_range(AdvancedKey* key, float value);
void advanced_key_set_deadzone(AdvancedKey *key, float upper, float lower);

#endif