#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdio.h"
#include "ch32v30x.h"

#define K1_Pin GPIO_Pin_1
#define K1_GPIO_Port GPIOB
#define K2_Pin GPIO_Pin_0
#define K2_GPIO_Port GPIOB
#define K3_Pin GPIO_Pin_5
#define K3_GPIO_Port GPIOC
#define K4_Pin GPIO_Pin_4
#define K4_GPIO_Port GPIOC

#define FN_K1_Pin GPIO_Pin_9
#define FN_K1_GPIO_Port GPIOC
#define FN_K2_Pin GPIO_Pin_8
#define FN_K2_GPIO_Port GPIOC
#define FN_K3_Pin GPIO_Pin_9
#define FN_K3_GPIO_Port GPIOD
#define FN_K4_Pin GPIO_Pin_8
#define FN_K4_GPIO_Port GPIOD
#define FN_K5_Pin GPIO_Pin_0
#define FN_K5_GPIO_Port GPIOE
#define FN_K6_Pin GPIO_Pin_4
#define FN_K6_GPIO_Port GPIOB

#define LED_Pin GPIO_Pin_12
#define LED_GPIO_Port GPIOA

#define DISP_RES_Pin GPIO_Pin_3
#define DISP_RES_GPIO_Port GPIOA
#define DISP_CS_Pin GPIO_Pin_4
#define DISP_CS_GPIO_Port GPIOA
#define DISP_DC_Pin GPIO_Pin_2
#define DISP_DC_GPIO_Port GPIOB

#define SPI2_NSS_Pin GPIO_Pin_12
#define SPI2_NSS_GPIO_Port GPIOB
#define SPI3_NSS_Pin GPIO_Pin_15
#define SPI3_NSS_GPIO_Port GPIOA

#define KNOB_Pin GPIO_Pin_3
#define KNOB_GPIO_Port GPIOB

#define ADC_CON_Pin GPIO_Pin_13
#define ADC_CON_GPIO_Port GPIOC

#define EC11_A_Pin GPIO_Pin_6
#define EC11_A_GPIO_Port GPIOC
#define EC11_B_Pin GPIO_Pin_7
#define EC11_B_GPIO_Port GPIOC

#define FRAM_HOLD_Pin GPIO_Pin_2
#define FRAM_HOLD_GPIO_Port GPIOD

void JumpToBootloader();
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
