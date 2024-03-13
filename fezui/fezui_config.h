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
#define REFRESH_RATE          144

//#define _FPS_ON

#define roll()                rand()%2;

void keyid_prase(KeyBinding id,char* str,uint16_t str_len);
void fezui_read_counts();
void fezui_save_counts();
void fezui_read();
void fezui_save();
void fezui_reset();
void fezui_POST();



#endif /* FEZUI_CONFIG_H_ */
