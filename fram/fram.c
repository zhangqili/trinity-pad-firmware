
/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-02-20     XiaojieFan   add port file
 */

#include "main.h"
#include "sfud.h"
#include "fram.h"

#define PKG_USING_MB85RS16

#ifdef PKG_USING_MB85RS16

#define DBG_ENABLE
#define DBG_SECTION_NAME  "mb85rs16"
#define DBG_LEVEL  			DBG_LOG //DBG_INFO



extern int fram_init();

static int spi_write_read(const uint8_t *write_buf, size_t write_size, uint8_t *read_buf,
                               size_t read_size)
{
    int result = SFUD_SUCCESS;
    uint8_t send_data, read_data;

    if (write_size) {
        SFUD_ASSERT(write_buf);
    }
    if (read_size) {
        SFUD_ASSERT(read_buf);
    }

	//GPIO_SetBits(FRAM_HOLD_GPIO_Port, FRAM_HOLD_Pin);
    //GPIO_ResetBits(SPI3_NSS_GPIO_Port, SPI3_NSS_Pin);
    /* ?????? */
    for (size_t i = 0, retry_times; i < write_size + read_size; i++) {
        /* ?????????? SPI ??????????? dummy(0xFF) ? SPI ?? */
        if (i < write_size) {
            send_data = *write_buf++;
        } else {
            send_data = SFUD_DUMMY_DATA;
        }
        /* ???? */
        retry_times = 1000;
        while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET) {
            SFUD_RETRY_PROCESS(NULL, retry_times, result);
        }
        if (result != SFUD_SUCCESS) {
            goto exit;
        }
        SPI_I2S_SendData(SPI3, send_data);
        /* ???? */
        retry_times = 1000;
        while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET) {
            SFUD_RETRY_PROCESS(NULL, retry_times, result);
        }
        if (result != SFUD_SUCCESS) {
            goto exit;
        }
        read_data = SPI_I2S_ReceiveData(SPI3);
        /* ??????????????? SPI ??????????? */
        if (i >= write_size) {
            *read_buf++ = read_data;
        }
    }
exit:
    //GPIO_SetBits(SPI3_NSS_GPIO_Port, SPI3_NSS_Pin);
    return result;
}

void fram_spi_send(uint8_t *send_buf, uint16_t send_bytes)
{
    GPIO_SetBits(FRAM_HOLD_GPIO_Port, FRAM_HOLD_Pin);
    GPIO_ResetBits(SPI3_NSS_GPIO_Port, SPI3_NSS_Pin);
	spi_write_read(send_buf,send_bytes,NULL,0);
    GPIO_SetBits(SPI3_NSS_GPIO_Port, SPI3_NSS_Pin);
}

void fram_spi_send_then_send(uint8_t *send_buf, uint16_t send_bytes,uint8_t *send_buf1, uint16_t send_bytes1)
{
    GPIO_SetBits(FRAM_HOLD_GPIO_Port, FRAM_HOLD_Pin);
    GPIO_ResetBits(SPI3_NSS_GPIO_Port, SPI3_NSS_Pin);
	spi_write_read(send_buf,send_bytes,NULL,0);
	spi_write_read(send_buf1,send_bytes1,NULL,0);
    GPIO_SetBits(SPI3_NSS_GPIO_Port, SPI3_NSS_Pin);
}
void fram_spi_send_then_recv(uint8_t *send_buf, uint16_t send_bytes,uint8_t *recv_buf, uint16_t recv_bytes)
{

    GPIO_SetBits(FRAM_HOLD_GPIO_Port, FRAM_HOLD_Pin);
    GPIO_ResetBits(SPI3_NSS_GPIO_Port, SPI3_NSS_Pin);
	spi_write_read(send_buf,send_bytes,NULL,0);
	spi_write_read(NULL,0,recv_buf,recv_bytes);
    GPIO_SetBits(SPI3_NSS_GPIO_Port, SPI3_NSS_Pin);
}
//CS- PD0
int fram_spi_device_init()
{
	/*
    __HAL_RCC_GPIOD_CLK_ENABLE();
		rt_err_t res = RT_EOK;
    res = rt_hw_spi_device_attach("spi3", fram_SPI_DEVICE_NAME, GPIOD, GPIO_PIN_0);
	  if (res != RT_EOK)
		{
			LOG_E("mb85rs16 Failed to attach device %s\n", fram_SPI_DEVICE_NAME);
			return res;
		}
		*/
	return fram_init();
}

int fram_init()
{
	  //rt_err_t res = RT_EOK;
	/*
		spi_dev_mb85rs16 = (struct rt_spi_device *)rt_device_find(fram_SPI_DEVICE_NAME);
    if (!spi_dev_mb85rs16)
    {
        LOG_E("spi find run failed! cant't find %s device!\n", fram_SPI_DEVICE_NAME);
        return RT_NULL;
    }
		//Set device SPI Mode
		struct rt_spi_configuration cfg = {0};
		cfg.data_width = 8;
		cfg.mode = RT_SPI_MASTER |RT_SPI_MODE_0 | RT_SPI_MSB ;//| RT_SPI_NO_CS;// 
		cfg.max_hz = fram_SPICLOCK;
		rt_spi_configure(spi_dev_mb85rs16, &cfg);
		*/
		//return res;
	return 0;
}

void fram_write_enable(uint8_t select)
{
		fram_spi_send(&select,1);
}
void fram_write_byte(uint16_t write_addr, uint8_t write_data)
{
		uint8_t send_buff[3];
		fram_write_enable(REG_WRITE_ENABLE);
		send_buff[0] = REG_WRITE_COMMAND;
		send_buff[1] = (write_addr>>8) &0xff;
		send_buff[2] = write_addr&0xff;
		fram_spi_send_then_send(send_buff, 3,&write_data, 1);
		fram_write_enable(REG_WRITE_DISABLE);	
}

void fram_write_bytes(uint16_t write_addr, void *write_buff, uint16_t write_bytes)
{
		uint8_t send_buff[3];
		fram_write_enable(REG_WRITE_ENABLE);
		send_buff[0] = REG_WRITE_COMMAND;
		send_buff[1] = (write_addr>>8) &0xff;
		send_buff[2] = write_addr&0xff;
		fram_spi_send_then_send(send_buff, 3,(uint8_t*)write_buff, write_bytes);
		fram_write_enable(REG_WRITE_DISABLE);
	
}

void fram_read_bytes(uint16_t read_addr, void *read_buff, uint16_t read_bytes)
{
	uint8_t send_buff[3] = {0};
	send_buff[0] = REG_READ_COMMAND;
	send_buff[1] = (read_addr>>8)&0xff;
	send_buff[2] = read_addr&0xff;
	fram_spi_send_then_recv(send_buff,3, (uint8_t*)read_buff, read_bytes);
}

uint32_t fram_read_device_id()
{
	uint8_t send_buff[3] = {0};
	uint32_t read_buff=0;
	send_buff[0] =  REG_READ_DEVICE_ID;
	fram_spi_send_then_recv(send_buff, 1, (uint8_t*)&read_buff, 4);
	return read_buff;
}




void fram_read_device_id_write_status(uint8_t write_data)
{
	uint8_t send_buff[2];
	
	send_buff[0] = REG_WRITE_STATUS;
	send_buff[1] = write_data;
	fram_spi_send(send_buff,2);
}

uint8_t fram_read_status(void)
{
	uint8_t read_status = 0,send_buff[1];
	
	send_buff[0] = REG_READ_STATUS;
	fram_spi_send_then_recv(send_buff,1,&read_status,1);
	
	return read_status;
}


void fram_write_status(uint8_t write_data)
{
	uint8_t send_buff[2];
	
	send_buff[0] = REG_WRITE_STATUS;
	send_buff[1] = write_data;
	fram_spi_send(send_buff,2);
}













#endif