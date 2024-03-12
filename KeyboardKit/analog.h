/*
 * analog.h
 *
 *  Created on: 2023骞�5鏈�21鏃�
 *      Author: xq123
 */

#ifndef ANALOG_H_
#define ANALOG_H_
#include "keyboard.h"

#define Analog_Clean() (ADC_Conversion_Count=0)

#define ANALOG_INIT_SCAN_COUNT 32768
#define ANALOG_BUFFER_LENGTH 16

extern uint8_t ADC_Conversion_Count;
extern int16_t ADC_Values[ADVANCED_KEY_NUM][64];
extern int16_t ADC_Value_List[ADVANCED_KEY_NUM];
extern uint16_t ADC_Buffer[ADVANCED_KEY_NUM*ANALOG_BUFFER_LENGTH];
extern float ADC_Sums[ADVANCED_KEY_NUM];
extern uint32_t ADC_Averages[ADVANCED_KEY_NUM];
extern float Analog_Values[ADVANCED_KEY_NUM];

void analog_scan();
void analog_init();
void analog_average();
void analog_check();
void analog_recovery();
void analog_save();

#endif /* ANALOG_H_ */
