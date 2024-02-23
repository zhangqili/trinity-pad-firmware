/*
 * rgb.c
 *
 *  Created on: May 21, 2023
 *      Author: xq123
 */
#include "main.h"
#include "rgb.h"
#include "string.h"
#include "math.h"

#define loop_queue_foreach(q,i) for(uint16_t (i)=(q)->front;(i)!=(q)->rear;(i)=(i+1)%(q)->len)

#ifdef USE_RGB
uint8_t RGB_Buffer[RGB_BUFFER_LENGTH];
#endif
RGBGlobalConfig RGB_GlobalConfig;
RGBIndividualConfig RGB_Configs[RGB_NUM];
ColorRGB RGB_Colors[RGB_NUM];
uint8_t RGB_TargetConfig;
RGBLoopQueue RGB_Argument_Queue;
const uint8_t RGB_Mapping[ADVANCED_KEY_NUM] = {0, 1, 2, 3};
const RGBLocation RGB_Locations[RGB_NUM] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}};

static RGBLoopQueueElm RGB_Argument_Buffer[ARGUMENT_BUFFER_LENGTH];
/*********************************************************************
 * @fn      TIM1_Init
 *
 * @brief   Initialize TIM1
 *
 * @return  none
 */
void TIM1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    TIM_OCInitTypeDef TIM_OCInitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_TimeBaseInitStructure.TIM_Period = 10 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = SystemCoreClock / 8000000 - 1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_DMACmd(TIM1, TIM_DMA_Update, ENABLE);
    TIM_Cmd(TIM1, ENABLE);
}

/*********************************************************************
 * @fn      DMA1_Init
 *
 * @brief   Initialize DMA for TIM1 ch1
 *
 * @return  none
 */
void DMA_TIM1_Init(void)
{
    DMA_InitTypeDef DMA_InitStructure = {0};
    //NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(TIM_DMA_CH1_CH);
    DMA_Cmd(TIM_DMA_CH1_CH, DISABLE);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIM1->CH1CVR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RGB_Buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = RGB_BUFFER_LENGTH;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(TIM_DMA_CH1_CH, &DMA_InitStructure);

    DMA_Cmd(TIM_DMA_CH1_CH, DISABLE);

    DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);

    // NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    // NVIC_Init(&NVIC_InitStructure);
}
#ifdef USE_RGB
void rgb_init()
{
    rgb_loop_queue_init(&RGB_Argument_Queue, RGB_Argument_Buffer, ARGUMENT_BUFFER_LENGTH);
    for (uint16_t i = 0; i < RGB_BUFFER_LENGTH; i++)
    {
        RGB_Buffer[i] = NONE_PULSE;
    }
    TIM1_Init();
    DMA_TIM1_Init();
}

#define COLOR_INTERVAL(key, low, up) (uint8_t)((key) < 0 ? (low) : ((key) > 1.0 ? (up) : (key) * (up)))
void rgb_update()
{
    ColorHSV temp_hsv;
    uint16_t tempint;
    float tempf;
    memcpy(&temp_hsv, &(RGB_GlobalConfig.hsv), sizeof(ColorHSV));
    loop_queue_foreach(&RGB_Argument_Queue, j)
    {
        RGB_Argument_Queue.data[j].argument += RGB_Configs[RGB_Argument_Queue.data[j].rgb_ptr].speed;
        if (RGB_Argument_Queue.data[j].argument > 20.0f)
        {
            rgb_loop_queue_dequeue(&RGB_Argument_Queue);
        }
    }
    switch (RGB_GlobalConfig.mode)
    {
    case RGB_GLOBAL_MODE_INDIVIDUAL:
        for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
        {
            uint8_t rgb_index = RGB_Mapping[i];
            switch (RGB_Configs[rgb_index].mode)
            {
            case RGB_MODE_REACT_LINEAR:
                RGB_Colors[rgb_index].r = COLOR_INTERVAL(Keyboard_AdvancedKeys[i].value, 0, (float)(RGB_Configs[rgb_index].rgb.r));
                RGB_Colors[rgb_index].g = COLOR_INTERVAL(Keyboard_AdvancedKeys[i].value, 0, (float)(RGB_Configs[rgb_index].rgb.g));
                RGB_Colors[rgb_index].b = COLOR_INTERVAL(Keyboard_AdvancedKeys[i].value, 0, (float)(RGB_Configs[rgb_index].rgb.b));
                break;
            case RGB_MODE_REACT_TRIGGER:
                RGB_Configs[rgb_index].argument = Keyboard_AdvancedKeys[i].key.state ? 1.0 : RGB_Configs[rgb_index].argument * (1.0 - fabsf(RGB_Configs[rgb_index].speed));
                RGB_Colors[rgb_index].r = (uint8_t)((float)(RGB_Configs[rgb_index].rgb.r) * RGB_Configs[rgb_index].argument);
                ;
                RGB_Colors[rgb_index].g = (uint8_t)((float)(RGB_Configs[rgb_index].rgb.g) * RGB_Configs[rgb_index].argument);
                ;
                RGB_Colors[rgb_index].b = (uint8_t)((float)(RGB_Configs[rgb_index].rgb.b) * RGB_Configs[rgb_index].argument);
                ;
                break;
            case RGB_MODE_STATIC:
                RGB_Colors[rgb_index].r = RGB_Configs[rgb_index].rgb.r;
                RGB_Colors[rgb_index].g = RGB_Configs[rgb_index].rgb.g;
                RGB_Colors[rgb_index].b = RGB_Configs[rgb_index].rgb.b;
                break;
            case RGB_MODE_CYCLE:
                RGB_Configs[i].argument += RGB_Configs[i].speed;
                if (RGB_Configs[i].argument > 359.9f)
                {
                    RGB_Configs[i].argument = 0;
                }
                if (RGB_Configs[i].argument < 0)
                {
                    RGB_Configs[i].argument = 360;
                }
                temp_hsv.s = RGB_Configs[i].hsv.s;
                temp_hsv.v = RGB_Configs[i].hsv.v;
                temp_hsv.h = (RGB_Configs[i].hsv.h + (uint16_t)(RGB_Configs[i].argument)) % 360;
                color_set_hsv(RGB_Colors + i, &temp_hsv);
                break;
            default:
                break;
            }
            RGB_Buffer[i] = NONE_PULSE;
        }
        break;
    case RGB_GLOBAL_MODE_WAVE:
        RGB_GlobalConfig.argument += RGB_GlobalConfig.speed;
        if (RGB_GlobalConfig.argument > 359.9f)
        {
            RGB_GlobalConfig.argument = 0;
        }
        if (RGB_GlobalConfig.argument < 0)
        {
            RGB_GlobalConfig.argument = 360;
        }
        for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
        {
            temp_hsv.h = ((uint16_t)RGB_GlobalConfig.argument + (uint16_t)(RGB_Locations[i].x * 15)) % 360;
            color_set_hsv(RGB_Colors + i, &temp_hsv);
        }
        break;
    case RGB_GLOBAL_MODE_OFF:
        for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
        {
            RGB_Colors[i].r = 0;
            RGB_Colors[i].g = 0;
            RGB_Colors[i].b = 0;
        }
        break;
    case RGB_GLOBAL_MODE_STRING:
    case RGB_GLOBAL_MODE_FADING_STRING:
        for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
        {
            RGB_Colors[i].r = 0;
            RGB_Colors[i].g = 0;
            RGB_Colors[i].b = 0;
        }
        loop_queue_foreach(&RGB_Argument_Queue, k)
        {
            for (int8_t j = 0; j < ADVANCED_KEY_NUM; j++)
            {
                if (RGB_Locations[RGB_Argument_Queue.data[k].rgb_ptr].row == RGB_Locations[j].row)
                {
                    tempf = (1.0f - fabsf(RGB_Argument_Queue.data[k].argument - fabsf(RGB_Locations[RGB_Argument_Queue.data[k].rgb_ptr].x - RGB_Locations[j].x)));
                    tempf = tempf > 0 ? tempf : 0;
                    if (RGB_GlobalConfig.mode == RGB_GLOBAL_MODE_FADING_STRING)
                    {
                        tempf /= RGB_Argument_Queue.data[k].argument;
                    }
                    // printf("%f\n",1.0 - fabsf(i+RGB_Configs[i].argument-j));
                    // printf("%.2f\t",tempf*((float)(RGB_Configs[i].rgb.r)));
                    tempint = RGB_Colors[j].r;
                    tempint += ((uint8_t)(tempf * ((float)(RGB_Configs[j].rgb.r)))) >> 1;
                    RGB_Colors[j].r = tempint < 256 ? tempint : 255;
                    // printf("%d\t",tempr);

                    tempint = RGB_Colors[j].g;
                    tempint += ((uint8_t)(tempf * ((float)(RGB_Configs[j].rgb.g)))) >> 1;
                    RGB_Colors[j].g = tempint < 256 ? tempint : 255;

                    tempint = RGB_Colors[j].b;
                    tempint += ((uint8_t)(tempf * ((float)(RGB_Configs[j].rgb.b)))) >> 1;
                    RGB_Colors[j].b = tempint < 256 ? tempint : 255;
                }
            }
        }

        break;
    case RGB_GLOBAL_MODE_DIAMOND_RIPPLE:
    case RGB_GLOBAL_MODE_FADING_DIAMOND_RIPPLE:
        for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
        {
            RGB_Colors[i].r = 0;
            RGB_Colors[i].g = 0;
            RGB_Colors[i].b = 0;
        }
        loop_queue_foreach(&RGB_Argument_Queue, k)
        {
            for (int8_t i = 0; i < ADVANCED_KEY_NUM; i++)
            {
                tempf = (1.0f - fabsf(RGB_Argument_Queue.data[k].argument * 2 - fabsf(RGB_Locations[RGB_Argument_Queue.data[k].rgb_ptr].row - RGB_Locations[i].row) - fabsf(RGB_Locations[RGB_Argument_Queue.data[k].rgb_ptr].x - RGB_Locations[i].x)));
                tempf = tempf > 0 ? tempf : 0;
                if (RGB_GlobalConfig.mode == RGB_GLOBAL_MODE_FADING_DIAMOND_RIPPLE)
                {
                    tempf /= RGB_Argument_Queue.data[k].argument;
                }
                // printf("%f\n",1.0 - fabsf(i+RGB_Configs[i].argument-j));
                // printf("%.2f\t",tempf*((float)(RGB_Configs[i].rgb.r)));
                tempint = RGB_Colors[i].r;
                tempint += ((uint8_t)(tempf * ((float)(RGB_Configs[i].rgb.r)))) >> 1;
                RGB_Colors[i].r = tempint < 256 ? tempint : 255;
                // printf("%d\t",tempr);

                tempint = RGB_Colors[i].g;
                tempint += ((uint8_t)(tempf * ((float)(RGB_Configs[i].rgb.g)))) >> 1;
                RGB_Colors[i].g = tempint < 256 ? tempint : 255;

                tempint = RGB_Colors[i].b;
                tempint += ((uint8_t)(tempf * ((float)(RGB_Configs[i].rgb.b)))) >> 1;
                RGB_Colors[i].b = tempint < 256 ? tempint : 255;
            }
        }

        break;
    case RGB_GLOBAL_MODE_JELLY:
        for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
        {
            RGB_Colors[i].r = 0;
            RGB_Colors[i].g = 0;
            RGB_Colors[i].b = 0;
        }
        for (int8_t i = 0; i < ADVANCED_KEY_NUM; i++)
        {
            for (int8_t j = 0; j < ADVANCED_KEY_NUM; j++)
            {
                tempf = (10.0f * Keyboard_AdvancedKeys[i].value - fabsf(RGB_Locations[j].row - RGB_Locations[RGB_Mapping[i]].row) - fabsf(RGB_Locations[j].x - RGB_Locations[RGB_Mapping[i]].x));
                tempf = tempf > 0 ? tempf > 1 ? 1 : tempf : 0;
                // printf("%f\n",1.0 - fabsf(i+RGB_Configs[i].argument-j));
                // printf("%.2f\t",tempf*((float)(RGB_Configs[i].rgb.r)));
                tempint = RGB_Colors[j].r;
                tempint += ((uint8_t)(tempf * ((float)(RGB_Configs[j].rgb.r)))) >> 1;
                RGB_Colors[j].r = tempint < 256 ? tempint : 255;
                // printf("%d\t",tempr);

                tempint = RGB_Colors[j].g;
                tempint += ((uint8_t)(tempf * ((float)(RGB_Configs[j].rgb.g)))) >> 1;
                RGB_Colors[j].g = tempint < 256 ? tempint : 255;

                tempint = RGB_Colors[j].b;
                tempint += ((uint8_t)(tempf * ((float)(RGB_Configs[j].rgb.b)))) >> 1;
                RGB_Colors[j].b = tempint < 256 ? tempint : 255;
            }
        }

        break;
    default:
        for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
        {
            RGB_Colors[i].r = 0;
            RGB_Colors[i].g = 0;
            RGB_Colors[i].b = 0;
        }
        break;
    }
    for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        rgb_set(RGB_Colors[i].r, RGB_Colors[i].g, RGB_Colors[i].b, i);
    }
}

void rgb_set(uint8_t r, uint8_t g, uint8_t b, uint16_t index)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        RGB_Buffer[RGB_RESET_LENGTH + index * 24 + i] = (g << i) & (0x80) ? ONE_PULSE : ZERO_PULSE;
        RGB_Buffer[RGB_RESET_LENGTH + index * 24 + i + 8] = (r << i) & (0x80) ? ONE_PULSE : ZERO_PULSE;
        RGB_Buffer[RGB_RESET_LENGTH + index * 24 + i + 16] = (b << i) & (0x80) ? ONE_PULSE : ZERO_PULSE;
    }
}

void rgb_flash()
{
    for (uint8_t i = 1; i < 64; i++)
    {
        for (uint8_t j = 0; j < RGB_NUM; j++)
        {
            rgb_set(i, i, i, j);
        }
        Delay_Ms(10);
    }
    for (uint8_t i = 64; i > 1; i--)
    {
        for (uint8_t j = 0; j < RGB_NUM; j++)
        {
            rgb_set(i, i, i, j);
        }
        Delay_Ms(10);
    }
    rgb_turn_off();
}

void rgb_turn_off()
{
    for (uint8_t i = 0; i < RGB_NUM; i++)
    {
        rgb_set(0, 0, 0, i);
    }
}
#endif

void rgb_recovery()
{
    ColorHSV temphsv = {273, 78, 99};
    for (uint8_t i = 0; i < RGB_NUM; i++)
    {
        RGB_Configs[i].mode = RGB_MODE_REACT_LINEAR;
        RGB_Configs[i].hsv = temphsv;
        RGB_Configs[i].speed = 0.15;
        color_set_hsv(&RGB_Configs[i].rgb, &temphsv);
    }
    ColorHSV hsv = {273, 78, 99};
    // RGB_GlobalConfig.mode=RGB_GLOBAL_MODE_WAVE;
    RGB_GlobalConfig.mode = RGB_GLOBAL_MODE_INDIVIDUAL;
    RGB_GlobalConfig.speed = 0.5;
    RGB_GlobalConfig.hsv = hsv;
    color_set_hsv(&RGB_GlobalConfig.rgb, &hsv);
}

void rgb_save()
{
}

void rgb_loop_queue_init(RGBLoopQueue *q, RGBLoopQueueElm *data, uint16_t len)
{
    q->data = data;
    q->front = 0;
    q->rear = 0;
    q->len = len;
}

RGBLoopQueueElm rgb_loop_queue_dequeue(RGBLoopQueue *q)
{
    RGBArgument a = {0, 0};
    if (q->front == q->rear)
        return a;
    q->front = (q->front + 1) % (q->len);
    return q->data[(q->front + q->len - 1) % (q->len)];
}

void rgb_loop_queue_enqueue(RGBLoopQueue *q, RGBLoopQueueElm t)
{
    if (((q->rear + 1) % (q->len)) == q->front)
        return;
    q->data[q->rear] = t;
    q->rear = (q->rear + 1) % (q->len);
}
