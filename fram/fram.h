#ifndef __MB85RS16_H__
#define __MB85RS16_H__
#include "stdint.h"
#define MB85RS16_SPICLOCK    2000000
/* mb85rs16 register */
#define    REG_READ_COMMAND    0x03
#define	   REG_WRITE_COMMAND   0x02
#define	   REG_WRITE_ENABLE    0x06
#define	   REG_WRITE_DISABLE   0x04
#define	   REG_READ_STATUS     0x05
#define	   REG_WRITE_STATUS    0x01
#define    REG_READ_DEVICE_ID  0x9F

uint32_t fram_read_device_id();
void fram_read_bytes(uint16_t read_addr, void *read_buff, uint16_t read_bytes);
void fram_write_bytes(uint16_t write_addr, void *write_buff, uint16_t write_bytes);
void fram_write_enable(uint8_t select);
uint8_t fram_read_status(void);
void fram_write_status(uint8_t write_data);
int fram_init();







#endif
