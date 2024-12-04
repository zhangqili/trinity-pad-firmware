/*
 * analog.c
 *
 *  Created on: 2023骞�5鏈�21鏃�
 *      Author: xq123
 */
#include "stdlib.h"
#include "stdio.h"
#include "rgb.h"
#include "analog.h"
#include "record.h"
#include "advanced_key.h"

uint16_t g_ADC_Conversion_Count;
uint16_t g_ADC_Buffer[ANALOG_BUFFER_LENGTH];
float g_ADC_Averages[ADVANCED_KEY_NUM];

AdaptiveSchimidtFilter g_analog_filters[ADVANCED_KEY_NUM];

RingBuf adc_ringbuf[ADVANCED_KEY_NUM];

uint8_t g_analog_active_channel;

void analog_init()
{
}

__WEAK void analog_channel_select(uint8_t x)
{
    //x=BCD_TO_GRAY(x);
    //HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, x&0x01);
    //HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, x&0x02);
    //HAL_GPIO_WritePin(C_GPIO_Port, C_Pin, x&0x04);
    //HAL_GPIO_WritePin(D_GPIO_Port, D_Pin, x&0x08);
}

void analog_scan()
{
}

void analog_average()
{
    uint32_t ADC_sum;
    for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        ADC_sum = 0;
        for (uint8_t j = 0; j < 64; j++)
        {
            ADC_sum += g_ADC_Buffer[i + j * ADVANCED_KEY_NUM];
        }
        g_ADC_Averages[i] = ADC_sum/64.0f;
#ifdef ENABLE_FILTER
        g_ADC_Averages[i] = adaptive_schimidt_filter(g_analog_filters+i,g_ADC_Averages[i]);
#endif
    }
}

void analog_check()
{
    bool state;
    for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        state = g_keyboard_advanced_keys[i].key.state;
        if (g_keyboard_advanced_keys[i].mode != KEY_DIGITAL_MODE)
        {
            advanced_key_update_raw(g_keyboard_advanced_keys + i, g_ADC_Averages[i]);
        }
        if (g_keyboard_advanced_keys[i].key.state && !state)
        {
#ifdef ENABLE_KPS
            record_kps_tick();
#endif
#ifdef ENABLE_COUNTER
            g_key_counts[i]++;
#endif
        }
    }
}

void analog_reset_range()
{
    analog_average();
    for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        advanced_key_reset_range(g_keyboard_advanced_keys + i, g_ADC_Averages[i]);
    }
}

void ringbuf_push(RingBuf* ringbuf, uint32_t data)
{
    ringbuf->pointer++;
    if (ringbuf->pointer >= RING_BUF_LEN)ringbuf->pointer = 0;
    ringbuf->datas[ringbuf->pointer] = data;
}

float ringbuf_avg(RingBuf* ringbuf)
{
    uint32_t avg = 0;
    for (int i = 0; i < RING_BUF_LEN; i++)
        avg += ringbuf->datas[i];

    avg = ((avg >> 2) & 0x01) + (avg >> 3);

    return (float)avg;
}
