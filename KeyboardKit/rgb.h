/*
 * rgb.h
 *
 *  Created on: May 21, 2023
 *      Author: xq123
 */

#ifndef RGB_H_
#define RGB_H_

#include "color.h"
#include "keyboard.h"

#define RGB_NUM                 (ADVANCED_KEY_NUM)
#define ONE_PULSE               (7)
#define ZERO_PULSE              (3)
#define NONE_PULSE              (0)
#define RGB_RESET_LENGTH        (60)
#define RGB_BUFFER_LENGTH       (((RGB_NUM)*(3*8))+RGB_RESET_LENGTH)
#define TIM_DMA_CH1_CH   DMA1_Channel5

#define RGB_Start() HAL_TIM_PWM_Start_DMA(&htim2,TIM_CHANNEL_2,(uint32_t*)RGB_Buffer,RGB_BUFFER_LENGTH);

#define USE_RGB
//#define RGB_Start() HAL_TIMEx_PWMN_Start_DMA(&htim8,TIM_CHANNEL_3,RGB_buffer,RGB_BUFFER_LENGTH);

typedef enum __RGBMode
{
    RGB_MODE_STATIC,
    RGB_MODE_CYCLE,
    RGB_MODE_REACT_LINEAR,
    RGB_MODE_REACT_TRIGGER,
} RGBMode;

typedef enum _RGBGlobalMode
{
    RGB_GLOBAL_MODE_OFF,
    RGB_GLOBAL_MODE_INDIVIDUAL,
    RGB_GLOBAL_MODE_WAVE,
    RGB_GLOBAL_MODE_STRING,
    RGB_GLOBAL_MODE_FADING_STRING,
    RGB_GLOBAL_MODE_DIAMOND_RIPPLE,
    RGB_GLOBAL_MODE_FADING_DIAMOND_RIPPLE,
    RGB_GLOBAL_MODE_JELLY,

} RGBGlobalMode;

typedef struct __RGBIndividualConfig
{
    RGBMode mode;
    ColorRGB rgb;
    ColorHSV hsv;
    float speed;
    float argument;
} RGBIndividualConfig;

typedef struct __RGBGlobalConfig
{
    RGBGlobalMode mode;
    ColorRGB rgb;
    ColorHSV hsv;
    float speed;
    float argument;
} RGBGlobalConfig;

typedef struct __RGBLocation
{
    uint8_t row;
    float x;
}RGBLocation;

typedef struct __RGBArgument
{
    uint8_t rgb_ptr;
    float argument;
}RGBArgument;



typedef RGBArgument RGBLoopQueueElm;

typedef struct __RGBLoopQueue
{
    RGBLoopQueueElm *data;
    int16_t front;
    int16_t rear;
    int16_t len;
} RGBLoopQueue;

void rgb_loop_queue_init(RGBLoopQueue* q, RGBLoopQueueElm*data, uint16_t len);
RGBLoopQueueElm rgb_loop_queue_dequeue(RGBLoopQueue* q);
void rgb_loop_queue_enqueue(RGBLoopQueue* q, RGBLoopQueueElm t);
//#define lefl_loop_queue_foreach(q,i) for(uint16_t (i)=(q)->front;(i)!=(q)->rear;(i)=(i+1)%(q)->len)

#define ARGUMENT_BUFFER_LENGTH 16
#ifdef USE_RGB
extern uint8_t RGB_Buffer[RGB_BUFFER_LENGTH];
#endif
extern ColorRGB RGB_Colors[RGB_NUM];
extern RGBIndividualConfig RGB_Configs[RGB_NUM];
extern RGBGlobalConfig RGB_GlobalConfig;
extern uint8_t RGB_TargetConfig;
extern RGBLoopQueue RGB_Argument_Queue;
extern const uint8_t RGB_Mapping[ADVANCED_KEY_NUM];

#ifdef USE_RGB
void rgb_init();
void rgb_update();
void rgb_set(uint8_t r,uint8_t g,uint8_t b,uint16_t index);
void rgb_flash();
void rgb_turn_off();
#endif
void rgb_recovery();
void rgb_save();


#endif /* RGB_H_ */
