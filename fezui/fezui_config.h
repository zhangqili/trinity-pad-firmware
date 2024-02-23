/*
 * fezui_config.h
 *
 *  Created on: 2023骞�6鏈�19鏃�
 *      Author: xq123
 */

#ifndef FEZUI_CONFIG_H_
#define FEZUI_CONFIG_H_
#include "keyboard.h"
#define WIDTH                 128
#define HEIGHT                64
#define BUFFER_LENGTH         64
#define NUMBER_STRING_LENGTH  BUFFER_LENGTH
#define REFRESH_RATE          144
#define KPS_HISTORY_LENGTH    65
#define ITEM_HEIGHT           14
#define HISTORY_LENGTH        129

//#define _FPS_ON

#define roll()                rand()%2;


void Keyboard_ID_Save();
void keyboard_id_recovery();
void keyid_prase(KeyBinding id,char* str,uint16_t str_len);
void Analog_Read();
void analog_save();
void fezui_read_counts();
void fezui_save_counts();
void fezui_read();
void fezui_save();
void fezui_reset();
void fezui_POST();



#endif /* FEZUI_CONFIG_H_ */
