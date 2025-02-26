/*
 * Copyright (c) 2024 Zhangqi Li (@zhangqili)
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#ifndef KEYBOARD_CONF_H_
#define KEYBOARD_CONF_H_

/********************/
/* Keyboard General */
/********************/
#define LAYER_NUM             3
#define ADVANCED_KEY_NUM      4
#define KEY_NUM               (6+3)
#define USE_FLOAT_VALUE
//#define NKRO_ENABLE
//#define CONTINUOUS_POLL

/********************/
/* Keyboard Default */
/********************/
#define DEFAULT_ADVANCED_KEY_MODE   KEY_ANALOG_RAPID_MODE
#define DEFAULT_CALIBRATION_MODE    KEY_AUTO_CALIBRATION_UNDEFINED
#define DEFAULT_TRIGGER_DISTANCE    0.08
#define DEFAULT_RELEASE_DISTANCE    0.08
#define DEFAULT_UPPER_DEADZONE      0.04
#define DEFAULT_LOWER_DEADZONE      0.2
#define DEFAULT_ACTIVATION_VALUE    0.5
#define DEFAULT_DEACTIVATION_VALUE  0.49
#define DEFAULT_ESTIMATED_RANGE     500


/**********/
/* Analog */
/**********/
#define RING_BUF_LEN            64

/*******/
/* RGB */
/*******/
#define ENABLE_RGB              
#define RGB_NUM                 (ADVANCED_KEY_NUM)
#define ONE_PULSE               (7)
#define ZERO_PULSE              (3)
#define NONE_PULSE              (0)
#define RGB_RESET_LENGTH        (60)
#define RGB_BUFFER_LENGTH       (((RGB_NUM)*(3*8))+RGB_RESET_LENGTH)
#define RGB_MAX_DURATION        1000
#define FADING_DISTANCE         5.0f
#define JELLY_DISTANCE          10.0f
#define PORT_LOCATION           {2, 2}
#define RGB_FLASH_MAX_DURATION  1000
#define RGB_FLASH_RIPPLE_SPEED  0.02
#define RGB_DEFAULT_MODE        RGB_MODE_LINEAR
#define RGB_DEFAULT_SPEED       0.015
#define RGB_DEFAULT_COLOR_HSV   {273, 78, 99}

/**********/
/* Filter */
/**********/
#define TOLERANCE               3
#define ENABLE_FILTER

/**********/
/* Record */
/**********/
#define STATIC_RECORD
#define ENABLE_ANALOG_HISTORY
#define ENABLE_KPS
#define ENABLE_KPS_HISTORY
#define ENABLE_BIT_STREAM
#define ENABLE_COUNTER

#define KPS_HISTORY_LENGTH      65
#define BIT_STREAM_LENGTH       128
#define ANALOG_HISTORY_LENGTH   129
#define RECORD_MAX_KEY_NUM      8
#define KPS_REFRESH_RATE        144

/*******/
/* USB */
/*******/
#define USB_POLLING_INTERVAL_MS 1
#define FIXED_CONTROL_ENDPOINT_SIZE 0x40
#define VENDOR_ID 0xffff
#define PRODUCT_ID 0xffff
#define DEVICE_VER 2
#define FIXED_NUM_CONFIGURATIONS 1

#define MANUFACTURER "Lzq12345"
#define PRODUCT "Trinity Keypad"
#define SERIAL_NUMBER "2022123456"

#define RAW_ENABLE
//#define KEYBOARD_SHARED_EP
#define SHARED_EP_ENABLE
#define NKRO_ENABLE
#define MOUSE_ENABLE
#define MOUSE_SHARED_EP
#define EXTRAKEY_ENABLE
#define JOYSTICK_ENABLE
#define JOYSTICK_SHARED_EP
//#define DIGITIZER_ENABLE
//#define DIGITIZER_SHARED_EP
//#define PROGRAMMABLE_BUTTON_ENABLE

/****************/
/* User Defines */
/****************/
#define PULSE 1000
#define PULSE_LEN_MS 40

#define GPIO_READ(x)          GPIO_ReadInputDataBit(x##_GPIO_Port,x##_Pin)
#define K1_READ               !GPIO_READ(K1)
#define K2_READ               !GPIO_READ(K2)
#define K3_READ               !GPIO_READ(K3)
#define K4_READ               !GPIO_READ(K4)
#define FN_K1_READ            !GPIO_READ(FN_K1)
#define FN_K2_READ            !GPIO_READ(FN_K2)
#define FN_K3_READ            !GPIO_READ(FN_K3)
#define FN_K4_READ            !GPIO_READ(FN_K4)
#define FN_K5_READ            !GPIO_READ(FN_K5)
#define FN_K6_READ            GPIO_READ(FN_K6)
#define KNOB_READ             GPIO_READ(KNOB)

#define KEY_FN_K1               g_keyboard_keys[0]
#define KEY_FN_K2               g_keyboard_keys[1]
#define KEY_FN_K3               g_keyboard_keys[2]
#define KEY_FN_K4               g_keyboard_keys[3]
#define KEY_FN_K5               g_keyboard_keys[4]
#define KEY_FN_K6               g_keyboard_keys[5]
#define KEY_KNOB                g_keyboard_keys[6]
#define KEY_KNOB_CLOCKWISE      g_keyboard_keys[7]
#define KEY_KNOB_ANTICLOCKWISE  g_keyboard_keys[8]

#define KEY_FN_K1_INDEX               0
#define KEY_FN_K2_INDEX               1
#define KEY_FN_K3_INDEX               2
#define KEY_FN_K4_INDEX               3
#define KEY_FN_K5_INDEX               4
#define KEY_FN_K6_INDEX               5
#define KEY_KNOB_INDEX                6
#define KEY_KNOB_CLOCKWISE_INDEX      7
#define KEY_KNOB_ANTICLOCKWISE_INDEX  8

enum USER_KEYCODE {
    USER_OPENMENU = 0,
    USER_SNAKE_LAUNCH = 0x10,
    USER_SNAKE_QUIT = 0x11,
    USER_SNAKE_PAUSE = 0x12,
    USER_SNAKE_SPEED_UP = 0x13,
    USER_SNAKE_SPEED_DOWN = 0x14,
    USER_SNAKE_RESTART = 0x15,
    USER_SNAKE_LEFT = 0x18,
    USER_SNAKE_UP = 0x19,
    USER_SNAKE_RIGHT = 0x1A,
    USER_SNAKE_DOWN = 0x1B,
};

#endif /* KEYBOARD_CONF_H_ */
