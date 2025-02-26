/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/06/06
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 USART Print debugging routine:
 USART1_Tx(PA9).
 This example demonstrates using USART1(PA9) as a print debug port output.

*/

#include "debug.h"
#include "main.h"
#include "analog.h"
#include "fezui.h"
#include "fezui_var.h"
#include "sfud.h"
#include "lfs.h"
#include "rgb.h"
#include "fram.h"
#include "record.h"
#include "mouse.h"
#include "keyboard.h"
#include "command.h"
#include "usbd_user.h"

/* Global typedef */

/* Global define */

/* Global Variable */

sfud_flash sfud_norflash0 = {
    .name = "norflash0",
    .spi.name = "SPI2",
    .chip = {"W25Q128JV", SFUD_MF_ID_WINBOND, 0x40, 0x18, 16L * 1024L * 1024L, SFUD_WM_PAGE_256B, 4096, 0x20},
};
static uint16_t ADC_Buffer[ADVANCED_KEY_NUM*64];

#define U8LOG_WIDTH 24
#define U8LOG_HEIGHT 30
u8log_t g_u8log;
static uint8_t u8log_buffer[U8LOG_WIDTH * U8LOG_HEIGHT];

void User_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = K1_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(K1_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = K2_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(K2_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = K3_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(K3_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = K4_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(K4_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = FN_K1_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(FN_K1_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = FN_K2_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(FN_K2_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = FN_K3_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(FN_K3_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = FN_K4_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(FN_K4_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = FN_K5_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(FN_K5_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = FN_K6_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(FN_K6_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = KNOB_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(KNOB_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = DISP_RES_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DISP_RES_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = DISP_CS_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DISP_CS_GPIO_Port, &GPIO_InitStructure);
    GPIO_WriteBit(DISP_CS_GPIO_Port, DISP_CS_Pin, Bit_RESET);

    GPIO_InitStructure.GPIO_Pin = DISP_DC_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DISP_DC_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SPI2_NSS_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI2_NSS_GPIO_Port, &GPIO_InitStructure);
    GPIO_WriteBit(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, Bit_SET);

    GPIO_InitStructure.GPIO_Pin = SPI3_NSS_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI3_NSS_GPIO_Port, &GPIO_InitStructure);
    GPIO_WriteBit(SPI3_NSS_GPIO_Port, SPI3_NSS_Pin, Bit_SET);

    GPIO_InitStructure.GPIO_Pin = ADC_CON_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ADC_CON_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = EC11_B_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(EC11_B_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = FRAM_HOLD_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(FRAM_HOLD_GPIO_Port, &GPIO_InitStructure);
    GPIO_WriteBit(FRAM_HOLD_GPIO_Port, FRAM_HOLD_Pin, Bit_SET);
}

int SPI1_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    SPI_InitTypeDef SPI_InitStructure = {0};

    // SPI1 Begin Init
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);
    return 0;
}

int SPI2_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    SPI_InitTypeDef SPI_InitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);

    SPI_Cmd(SPI2, ENABLE);

    return 0;
}

int SPI3_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    SPI_InitTypeDef SPI_InitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    AFIO->PCFR1 |= (1 << 28);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI3, &SPI_InitStructure);

    SPI_Cmd(SPI3, ENABLE);
    return 0;
}
void Encoder_Init_TIM8(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);  // 使能定时器8的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // 使能PC端口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // 端口配置 PC6， PC7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  // 浮空输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);                 // 根据设定参数初始化GPIOC

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 5;                    // 预分频器
    TIM_TimeBaseStructure.TIM_Period = 30;                      // 设定计数器自动重装值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 选择时钟分频：不分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 边沿计数模式
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);             // 初始化定时器8

    TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); // 使用编码器模式3

    TIM_ICStructInit(&TIM_ICInitStructure); // 把TIM_ICInitStruct 中的每一个参数按缺省值填入
    TIM_ICInitStructure.TIM_ICFilter = 3;   // 设置滤波器长度
    TIM_ICInit(TIM8, &TIM_ICInitStructure); // 根据 TIM_ICInitStruct 的参数初始化外设   TIMx

    NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;        // 使能中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 设置抢占优先级为1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        // 设置子优先级为2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);                           // 中断优先级分组初始化

    TIM_ClearFlag(TIM8, TIM_FLAG_Update);      // 清除TIM的更新标志位
    TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE); // 使能定时器中断

    TIM_SetCounter(TIM8, 0);
    TIM_Cmd(TIM8, ENABLE); // 使能定时器8
}
void I2C2_Init(u32 bound, u16 address)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    I2C_InitTypeDef I2C_InitTSturcture = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    // GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_InitTSturcture.I2C_ClockSpeed = bound;
    I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
    I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_InitTSturcture.I2C_OwnAddress1 = 0;
    I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
    I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C2, &I2C_InitTSturcture);

    I2C_Cmd(I2C2, ENABLE);

    I2C_AcknowledgeConfig(I2C2, ENABLE);
}

void ADC_Function_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure = {0};
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 4;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);

    ADC_BufferCmd(ADC1, DISABLE); // disable buffer
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1))
        ;
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1))
        ;
        // Calibrattion_Val = Get_CalibrationValue(ADC1);
#define ADC_SampleTime ADC_SampleTime_55Cycles5
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 2, ADC_SampleTime);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 3, ADC_SampleTime);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 4, ADC_SampleTime);
}

void DMA_ADC_Init(DMA_Channel_TypeDef *DMA_CHx, uint32_t memadr, uint16_t bufsize)
{
    DMA_InitTypeDef DMA_InitStructure = {0};

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(DMA_CHx);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->RDATAR;
    DMA_InitStructure.DMA_MemoryBaseAddr = memadr;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = bufsize;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA_CHx, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel1, ENABLE); // 使能DMA
}

void TIM6_INT_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); // 使能TIM1时钟

    TIM_TimeBaseStructure.TIM_Period = arr;                     // 指定下次更新事件时要加载到活动自动重新加载寄存器中的周期值。
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  // 指定用于划分TIM时钟的预分频器值。
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 时钟分频因子
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // TIM计数模式，向上计数模式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); // 根据指定的参数初始化TIMx的时间基数单位

    // 初始化TIM NVIC，设置中断优先级分组
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;           // TIM6中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // 设置抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // 设置响应优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 使能通道1中断
    NVIC_Init(&NVIC_InitStructure);                           // 初始化NVIC

    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE); // 使能TIM1中断，允许更新中断

    TIM_Cmd(TIM6, ENABLE); // TIM1使能
}

void TIM7_INT_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); // 使能TIM1时钟

    TIM_TimeBaseStructure.TIM_Period = arr;                     // 指定下次更新事件时要加载到活动自动重新加载寄存器中的周期值。
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  // 指定用于划分TIM时钟的预分频器值。
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 时钟分频因子
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // TIM计数模式，向上计数模式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); // 根据指定的参数初始化TIMx的时间基数单位

    // 初始化TIM NVIC，设置中断优先级分组
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;           // TIM7中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 设置抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // 设置响应优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 使能通道1中断
    NVIC_Init(&NVIC_InitStructure);                           // 初始化NVIC

    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE); // 使能TIM1中断，允许更新中断

    TIM_Cmd(TIM7, ENABLE); // TIM1使能
}
/*********************************************************************
 * @fn      DMA1_Tx_Init
 *
 * @brief   Initializes the DMAy Channelx configuration.
 *
 * @param   DMA_CHx - x can be 1 to 7.
 *          ppadr - Peripheral base address.
 *          memadr - Memory base address.
 *          bufsize - DMA channel buffer size.
 *
 * @return  none
 */
void DMA1_Tx_Init(DMA_Channel_TypeDef *DMA_CHx, u32 ppadr, u32 memadr, u16 bufsize)
{
    DMA_InitTypeDef DMA_InitStructure = {0};

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(DMA_CHx);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ppadr;
    DMA_InitStructure.DMA_MemoryBaseAddr = memadr;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = bufsize;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA_CHx, &DMA_InitStructure);

    // DMA_ITConfig ( DMA1_Channel1, DMA_IT_TC, ENABLE );

    // NVIC_InitTypeDef NVIC_InitStructure = { 0 };
    // NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    // NVIC_Init ( &NVIC_InitStructure );
}

void EXTI0_INT_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    EXTI_InitTypeDef EXTI_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* GPIOA ----> EXTI_Line0 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void usb_dc_low_level_init(void)
{
    RCC_USBCLK48MConfig(RCC_USBCLK48MCLKSource_USBPHY);
    RCC_USBHSPLLCLKConfig(RCC_HSBHSPLLCLKSource_HSE);
    RCC_USBHSConfig(RCC_USBPLL_Div2);
    RCC_USBHSPLLCKREFCLKConfig(RCC_USBHSPLLCKREFCLK_4M);
    RCC_USBHSPHYPLLALIVEcmd(ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_USBHS, ENABLE);
    NVIC_SetPriority(USBHS_IRQn, 0x00);
    NVIC_EnableIRQ(USBHS_IRQn);

    Delay_Us(100);
}

void EXTI_INT_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    EXTI_InitTypeDef EXTI_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = EC11_A_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(EC11_A_GPIO_Port, &GPIO_InitStructure);
    /* GPIOA ----> EXTI_Line6 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource6);
    EXTI_InitStructure.EXTI_Line = EXTI_Line6;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

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
    // NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(DMA1_Channel5);
    DMA_Cmd(DMA1_Channel5, DISABLE);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIM1->CH1CVR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)g_rgb_buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = RGB_BUFFER_LENGTH;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel5, DISABLE);

    // DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);

    // NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    // NVIC_Init(&NVIC_InitStructure);
}
/*********************************************************************
 * @fn      SYSTICK_Init_Config
 *
 * @brief   SYSTICK_Init_Config.
 *
 * @return  none
 */
void SYSTICK_Init_Config(uint64_t ticks)
{
    SysTick->SR &= ~(1 << 0); // clear State flag
    SysTick->CMP = ticks;
    SysTick->CNT = 0;
    SysTick->CTLR = 0xF;

    NVIC_SetPriority(SysTicK_IRQn, 15);
    NVIC_EnableIRQ(SysTicK_IRQn);
}

void (*SysMemBootJump)(void);
__IO uint32_t BootAddr = 0x1FFF8000; /* BootLoader 地址 */

void JumpToBootloader(void)
{
    uint32_t i = 0;

    /* 关闭全局中断 */
    __disable_irq();

    /* 关闭滴答定时器，复位到默认值 */
    // SysTick->CTRL = 0;
    // SysTick->LOAD = 0;
    // SysTick->VAL = 0;
    SysTick->CTLR = 0;
    SysTick->CMP = 0;
    SysTick->CNT = 0;
    SysTick->SR = 0;

    /* 设置所有时钟到默认状态，使用HSI时钟 */
    RCC_DeInit();

    /* 关闭所有中断，清除所有中断挂起标志 */
    for (i = 0; i < 8; i++)
    {
        NVIC->IPSR[i] = 0xFFFFFFFF;
        NVIC->IPRR[i] = 0xFFFFFFFF;
    }

    /* 使能全局中断 */
    __enable_irq();

    /* 跳转到系统BootLoader，首地址是MSP，地址+4是复位中断服务程序地址 */
    SysMemBootJump = (void (*)(void))(*((uint32_t *)(BootAddr + 4)));

    /* 设置主堆栈指针 */
    //__set_MSP(*(uint32_t *)BootAddr);
    //__asm("li  a6, 0x5z000");
    //__asm("jr  a6");

    /* 跳转到系统BootLoader */
    SysMemBootJump();

    /* 跳转成功的话，不会执行到这里，用户可以在这里添加代码 */
    while (1)
    {
    }
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    u8log_Init(&g_u8log, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%ld\r\n", SystemCoreClock);
    printf("ChipID:%08lx\r\n", DBGMCU_GetCHIPID());
    //prizntf("This is printf example\r\n");
    // SYSTICK_Init_Config(SystemCoreClock-1);
    User_GPIO_Init();
    SPI1_Init();
    SPI2_Init();
    SPI3_Init();
    ADC_Function_Init();
    // Encoder_Init_TIM8();
    sfud_device_init(&sfud_norflash0);
    keyboard_init();
    keyboard_scan();
    if (KEY_FN_K4.state && KEY_FN_K5.state)
    {
        keyboard_factory_reset();
    }
    fezui_init();
    record_init();
    rgb_init();
    TIM1_Init();
    DMA_TIM1_Init();
    rgb_factory_reset();
    EXTI_INT_INIT();
    DMA_Cmd(DMA1_Channel5, ENABLE);

    DMA1_Tx_Init(DMA1_Channel1, (u32)&ADC1->RDATAR, (u32)ADC_Buffer, ADVANCED_KEY_NUM*64);
    DMA_Cmd(DMA1_Channel1, ENABLE);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    usb_init();
    
    keyboard_recovery();
    Delay_Ms(100);//Delete this line forbidden!!!
    fram_read_bytes(0x400, g_key_counts, sizeof(g_key_counts));
    memcpy(g_key_init_counts, g_key_counts, sizeof(g_key_counts));

    TIM6_INT_Init(14400 / 144 - 1, 10000 - 1);
    TIM7_INT_Init(14400 / 8 - 1, 10 - 1);
    rgb_init_flash();
    analog_reset_range();
    while (1)
    {
        // extern void hid_keyboard_test(void);
        // hid_keyboard_test();
        fezui_render_handler();
        rgb_update();

        fram_write_bytes(0x400, g_key_counts, sizeof(g_key_counts));
        GPIO_WriteBit(LED_GPIO_Port, LED_Pin, !GPIO_ReadInputDataBit(LED_GPIO_Port, LED_Pin));
        // if(usb_state!=usb_device_is_configured())
        //{
        //     fezui_notification_begin(&fezui,&fezui_notification,"USB Connected",REFRESH_RATE*3,0.5);
        // }
        // usb_state=usb_device_is_configured;
    }
}

// static uint16_t temp_count = 0;
void TIM6_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM6_IRQHandler(void)
{
    static uint8_t count = 0;
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) // 检查TIM1中断是否发生。
    {
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update); // 清除TIM1的中断挂起位。
        count++;
        if (count == REFRESH_RATE)
        {
            // g_ADC_Conversion_Count = temp_count;
            // temp_count=0;
            sprintf(g_fpsstr, "%ld", g_fezui_fps);
            g_fezui_fps = 0;
            count = 0;
            if (fezui.screensaver_countdown)
                fezui.screensaver_countdown--;
            record_kps_history_timer();
            //g_usb_report_count1 = g_usb_report_count;
            //g_usb_report_count = 0;
            //g_usb_mouse_report_count1 = g_usb_mouse_report_count;
            //g_usb_mouse_report_count = 0;
            //g_usb_raw_report_count1 = g_usb_raw_report_count;
            //g_usb_raw_report_count = 0;
            g_fezui_run_time++;
        }
        fezui_timer_handler();
        record_bit_stream_timer();
        record_kps_timer();
    }
}
void analog_average()
{
    uint32_t ADC_sum;
    for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
    {
        ADC_sum = 0;
        for (uint8_t j = 0; j < 64; j++)
        {
            ADC_sum += ADC_Buffer[i + j * ADVANCED_KEY_NUM];
        }
        g_ADC_Averages[i] = ADC_sum/64.0f;
#ifdef ENABLE_FILTER
        g_ADC_Averages[i] = adaptive_schimidt_filter(g_analog_filters+i,g_ADC_Averages[i]);
#endif
    }
}

void TIM7_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM7_IRQHandler(void)
{
    static uint8_t count = 0;
    static uint8_t buffer[64];
    //static bool flag = true;
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
        count++;
        if (count == 8)
        {
            count = 0;
            fezui_tick++;
            g_keyboard_tick++;
        }
        // if(!HAL_GPIO_ReadPin(MENU_GPIO_Port, MENU_Pin))
        //for (uint8_t i = 0; i < ADVANCED_KEY_NUM; i++)
        //{
        //    for (uint8_t j = 0; j < 64; j++)
        //    {
        //        ringbuf_push(&adc_ringbuf[i], ADC_Buffer[i + j * ADVANCED_KEY_NUM]);
        //    }
        //}        
        keyboard_task();
        //if (flag)
        //{
        //    flag = false;
        //    keyboard_6KRObuffer_send(&g_keyboard_6kro_buffer);
        //    mouse_buffer_send(&g_mouse);
        //}
        
        switch (g_keyboard_state)
        {
        case KEYBOARD_DEBUG:
            buffer[0] = 0x02;
            buffer[1] = 0xFE;
            for (int i = 0; i < 4; i++)
            {
                buffer[2 + 10 * i] = i;
                buffer[3 + 10 * i] = g_keyboard_advanced_keys[i].key.state;
                memcpy(buffer + 4 + 10 * i, &g_keyboard_advanced_keys[i].raw, sizeof(float));
                memcpy(buffer + 4 + 10 * i + 4, &g_keyboard_advanced_keys[i].value, sizeof(float));

            }
            hid_send(buffer + 1, 63);
            break;
        case KEYBOARD_UPLOAD_CONFIG:
            if (!load_cargo())
            {
              g_keyboard_state = KEYBOARD_IDLE;
            }
            break;
        default:
            break;
        }
    }
}

/**
 * @brief  中断函数
 **/
void EXTI9_5_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast"))); // 中断函数前加这上这句,告诉编译器这个是中断函数
void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line6) != RESET) // 产生中断
    {
        g_keyboard_knob_flag = 8;
        if (GPIO_ReadInputDataBit(EC11_B_GPIO_Port, EC11_B_Pin))
        {
            keyboard_key_update(&KEY_KNOB_CLOCKWISE, false);
            keyboard_key_update(&KEY_KNOB_ANTICLOCKWISE, true);
        }
        else
        {
            keyboard_key_update(&KEY_KNOB_CLOCKWISE, true);
            keyboard_key_update(&KEY_KNOB_ANTICLOCKWISE, false);
        }
        EXTI_ClearITPendingBit(EXTI_Line6); // 清除中断标志
    }
}

void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void SysTick_Handler(void)
{
    if (SysTick->SR == 1)
    {
        SysTick->SR = 0; // clear State flag
    }
}

void DMA1_Channel1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void DMA1_Channel1_IRQHandler(void)
{
    if (DMA_GetITStatus(DMA1_IT_TC1) != RESET) // 检查TIM1中断是否发生。
    {
        DMA_ClearITPendingBit(DMA1_IT_TC1); // 清除TIM1的中断挂起位。
        // temp_count++;
    }
}