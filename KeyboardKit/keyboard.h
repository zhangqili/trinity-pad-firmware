/*
 * keyboard.h
 *
 *  Created on: May 21, 2023
 *      Author: xq123
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "key.h"
#include "usb_hid_keys.h"

#define LAYER_NUM             3
#define ADVANCED_KEY_NUM      4
#define KEY_NUM               (6+3)
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


#define KEY_FN_K1               Keyboard_Keys[0]
#define KEY_FN_K2               Keyboard_Keys[1]
#define KEY_FN_K3               Keyboard_Keys[2]
#define KEY_FN_K4               Keyboard_Keys[3]
#define KEY_FN_K5               Keyboard_Keys[4]
#define KEY_FN_K6               Keyboard_Keys[5]
#define KEY_KNOB                Keyboard_Keys[6]
#define KEY_KNOB_CLOCKWISE      Keyboard_Keys[7]
#define KEY_KNOB_ANTICLOCKWISE  Keyboard_Keys[8]

#define KEY_FN_K1_INDEX               0
#define KEY_FN_K2_INDEX               1
#define KEY_FN_K3_INDEX               2
#define KEY_FN_K4_INDEX               3
#define KEY_FN_K5_INDEX               4
#define KEY_FN_K6_INDEX               5
#define KEY_KNOB_INDEX                6
#define KEY_KNOB_CLOCKWISE_INDEX      7
#define KEY_KNOB_ANTICLOCKWISE_INDEX  8

typedef struct
{
    uint8_t keycode;
    uint8_t modifier;
}KeyBinding;

typedef struct
{
    uint8_t buffer[8];
    uint8_t keynum;
}Keyboard_6KROBuffer;

extern Key Keyboard_Keys[KEY_NUM];
extern AdvancedKey Keyboard_AdvancedKeys[ADVANCED_KEY_NUM];
extern uint8_t Keyboard_KNOB_Flag;
extern const KeyBinding default_keymap[LAYER_NUM][ADVANCED_KEY_NUM + KEY_NUM];
extern KeyBinding keymap[LAYER_NUM][ADVANCED_KEY_NUM + KEY_NUM];
extern volatile bool Keybaord_Alpha_Flag;
extern volatile bool Keybaord_Shift_Flag;
extern volatile bool Keybaord_SendReport_Enable;

int keyboard_6KRObuffer_add(Keyboard_6KROBuffer*buf,KeyBinding key);
void keyboard_6KRObuffer_send(Keyboard_6KROBuffer*buf);
void keyboard_6KRObuffer_clear(Keyboard_6KROBuffer*buf);
void keyboard_system_reset();
void keyboard_factory_reset();
void keyboard_init();
void keyboard_id_recovery();
void keyboard_recovery();
void keyboard_scan();
void keyboard_send_report();
void keyboard_save();

#endif /* KEYBOARD_H_ */
