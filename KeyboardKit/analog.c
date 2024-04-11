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

uint8_t g_ADC_Conversion_Count;
uint16_t g_ADC_Buffer[ADVANCED_KEY_NUM * ANALOG_BUFFER_LENGTH];
uint32_t g_ADC_Averages[ADVANCED_KEY_NUM];

void analog_init()
{
}

void analog_channel_select(uint8_t x)
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
    for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        g_ADC_Averages[i] = 0;
        for (uint8_t j = 0; j < ANALOG_BUFFER_LENGTH; j++)
        {
            g_ADC_Averages[i] += g_ADC_Buffer[i + j * ADVANCED_KEY_NUM];
        }
        g_ADC_Averages[i] /= ANALOG_BUFFER_LENGTH;
    }
}

void analog_check()
{
    bool state;
    RGBArgument a;
    for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        state = g_keyboard_advanced_keys[i].key.state;
        if (g_keyboard_advanced_keys[i].mode != KEY_DIGITAL_MODE)
        {
            if (g_ADC_Averages[i] < g_keyboard_advanced_keys[i].lower_bound)
                g_keyboard_advanced_keys[i].lower_bound = g_ADC_Averages[i];
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
            a.rgb_ptr = g_rgb_mapping[i];
            a.begin_time = RGB_Tick;
            rgb_loop_queue_enqueue(&g_rgb_argument_queue, a);
        }
    }
}

void analog_reset_range()
{
    for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        g_ADC_Averages[i] = 0;
        for (uint8_t j = 0; j < ANALOG_BUFFER_LENGTH; j++)
        {
            g_ADC_Averages[i] += g_ADC_Buffer[i + j * ADVANCED_KEY_NUM];
        }
        g_ADC_Averages[i] /= ANALOG_BUFFER_LENGTH;
        advanced_key_set_range(g_keyboard_advanced_keys + i, g_ADC_Averages[i], 200);
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
    //  avg = ringbuf->Datas[ringbuf->Pointer];
    if (avg - TOLERANCE > ringbuf->state)ringbuf->state = avg - TOLERANCE;
    if (avg + TOLERANCE < ringbuf->state)ringbuf->state = avg + TOLERANCE;

    //  return (float_t)avg;
    return (float)ringbuf->state;
}
