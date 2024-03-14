/*
 * analog.c
 *
 *  Created on: 2023骞�5鏈�21鏃�
 *      Author: xq123
 */
#include "main.h"
#include "stdlib.h"
#include "stdio.h"
#include "rgb.h"
#include "analog.h"
#include "record.h"

uint8_t ADC_Conversion_Count=0;
int16_t ADC_Values[ADVANCED_KEY_NUM][64];

int16_t ADC_Value_List[ADVANCED_KEY_NUM]={0};
uint16_t ADC_Buffer[ADVANCED_KEY_NUM*ANALOG_BUFFER_LENGTH];
uint8_t ADC_Conversion_Flag=0;

float ADC_Sums[ADVANCED_KEY_NUM]={0.0};
uint32_t ADC_Averages[ADVANCED_KEY_NUM];

float Analog_Values[ADVANCED_KEY_NUM]={0.0};



void analog_init()
{
    rgb_flash();
    for (uint16_t i = 0; i < ANALOG_INIT_SCAN_COUNT; i++)
    {
        for (uint8_t j = 0; j < ADVANCED_KEY_NUM; j++)
        {
        }
    }

    for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        Keyboard_AdvancedKeys[i].upper_bound=ADC_Sums[i]/(float)ANALOG_INIT_SCAN_COUNT;
        ADC_Sums[i]=0.0;
    }

    RGB_Colors[0].g=127;
    RGB_Colors[1].b=127;
    RGB_Colors[2].r=127;
    RGB_Colors[3].r=127;
    RGB_Colors[3].g=127;
    rgb_set(RGB_Colors[0].r,RGB_Colors[0].g,RGB_Colors[0].b,0);
    rgb_set(RGB_Colors[1].r,RGB_Colors[1].g,RGB_Colors[1].b,1);
    rgb_set(RGB_Colors[2].r,RGB_Colors[2].g,RGB_Colors[2].b,2);
    rgb_set(RGB_Colors[3].r,RGB_Colors[3].g,RGB_Colors[3].b,3);
    Delay_Ms(1000);
    for (uint16_t i = 0; i < ANALOG_INIT_SCAN_COUNT; i++)
    {
        for (uint8_t j = 0; j < ADVANCED_KEY_NUM; j++)
        {
        }
    }
    for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        Keyboard_AdvancedKeys[(i+3)%ADVANCED_KEY_NUM].lower_bound=ADC_Sums[i]/(float)ANALOG_INIT_SCAN_COUNT;
        ADC_Sums[i]=0.0;
    }

    RGB_Colors[0].g=0;
    RGB_Colors[1].b=0;
    RGB_Colors[2].r=0;
    RGB_Colors[3].r=0;
    RGB_Colors[3].g=0;
    rgb_set(RGB_Colors[0].r,RGB_Colors[0].g,RGB_Colors[0].b,0);
    rgb_set(RGB_Colors[1].r,RGB_Colors[1].g,RGB_Colors[1].b,1);
    rgb_set(RGB_Colors[2].r,RGB_Colors[2].g,RGB_Colors[2].b,2);
    rgb_set(RGB_Colors[3].r,RGB_Colors[3].g,RGB_Colors[3].b,3);
    for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        advanced_key_set_range(Keyboard_AdvancedKeys+i,Keyboard_AdvancedKeys[i].upper_bound,Keyboard_AdvancedKeys[i].lower_bound);
    }

    for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        advanced_key_set_deadzone(Keyboard_AdvancedKeys+i,Keyboard_AdvancedKeys[i].upper_deadzone,Keyboard_AdvancedKeys[i].lower_deadzone);
    }
}

void analog_scan()
{
    for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
    {
    }
    ADC_Conversion_Count++;
}

void analog_average()
{
    for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        ADC_Averages[i]=0;
        for (uint8_t j = 0; j < ANALOG_BUFFER_LENGTH; j++)
        {
            ADC_Averages[i]+=ADC_Buffer[i+j*ADVANCED_KEY_NUM];
        }
        ADC_Averages[i]/=ANALOG_BUFFER_LENGTH;
    }
}

void analog_check()
{
    bool state;
    RGBArgument a;
    for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        state=Keyboard_AdvancedKeys[i].key.state;
        if(Keyboard_AdvancedKeys[i].mode!=KEY_DIGITAL_MODE)
        {
            if(ADC_Averages[i]<Keyboard_AdvancedKeys[i].lower_bound)
                Keyboard_AdvancedKeys[i].lower_bound = ADC_Averages[i];
            advanced_key_update_raw(Keyboard_AdvancedKeys+i, ADC_Averages[i]);
        }
        if(Keyboard_AdvancedKeys[i].key.state&&!state)
        {
            #ifdef ENABLE_KPS
            record_kps_tick();
            #endif
            #ifdef ENABLE_COUNTER
            g_key_counts[i]++;
            #endif
            a.rgb_ptr = RGB_Mapping[i];
            a.argument=0.0;
            rgb_loop_queue_enqueue(&RGB_Argument_Queue, a);
        }
    }
}


void analog_recovery()
{

}

void analog_save()
{
}

void analog_reset_range()
{
    for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        ADC_Averages[i]=0;
        for (uint8_t j = 0; j < ANALOG_BUFFER_LENGTH; j++)
        {
            ADC_Averages[i]+=ADC_Buffer[i+j*ADVANCED_KEY_NUM];
        }
        ADC_Averages[i]/=ANALOG_BUFFER_LENGTH;
        advanced_key_set_range(Keyboard_AdvancedKeys + i, ADC_Averages[i], 200);
    }
}