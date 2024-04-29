

#ifndef USER_I2C_FMC_FLASH_H_
#define USER_I2C_FMC_FLASH_H_



//#include "stm32f1xx_hal.h"
#include "main.h"

#include "flash.h"

#define FMC_FLASH_PAGE_SIZE   1024
#define FLASH_START_ADDR  0x0800FC00  //最后一页

void fmc_read_bytes(uint32_t addr,uint8_t *bytes,uint16_t length);
int fmc_write_bytes(uint32_t addr,uint8_t *bytes,uint16_t length);


int fmc_earse_page(uint32_t addr);



#endif /* USER_I2C_FMC_FLASH_H_ */
