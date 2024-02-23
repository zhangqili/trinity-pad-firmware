/*
 * display.c
 *
 *  Created on: 2023骞�5鏈�23鏃�
 *      Author: xq123
 */
#include "display.h"
#include "ch32v30x.h"

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,void *arg_ptr)
{
  switch (msg)
  {
    case U8X8_MSG_BYTE_SEND:
        //GPIO_WriteBit(DISP_CS_GPIO_Port, DISP_CS_Pin, Bit_RESET);

        for (uint16_t i = 0; i < arg_int; i++) {             // 写数据
            while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET)
                ;
            while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
                ;
            SPI_I2S_SendData(SPI1, ((uint8_t*)arg_ptr)[i]);

        }
        //GPIO_WriteBit(DISP_CS_GPIO_Port, DISP_CS_Pin, Bit_SET);
      break;
    case U8X8_MSG_BYTE_INIT:
      break;
    case U8X8_MSG_BYTE_SET_DC:
      GPIO_WriteBit(DISP_DC_GPIO_Port, DISP_DC_Pin, arg_int);
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
      break;
    default:
      return 0;
  }
  return 1;
}

uint8_t u8x8_riscv_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
    U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
    U8X8_UNUSED void *arg_ptr)
{
  switch (msg)
  {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
      break;
    case U8X8_MSG_DELAY_MILLI:
      Delay_Ms(arg_int);
      break;
    case U8X8_MSG_GPIO_CS:
      break;
    case U8X8_MSG_GPIO_DC:
     GPIO_WriteBit(DISP_DC_GPIO_Port, DISP_DC_Pin, arg_int);
      break;
    case U8X8_MSG_GPIO_RESET:
      break;
  }
  return 1;
}
