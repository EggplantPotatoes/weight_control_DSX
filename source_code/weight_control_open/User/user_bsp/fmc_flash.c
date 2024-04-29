

#include "fmc_flash.h"
#include "string.h"
#include "stdio.h"

typedef union
{
	uint32_t four[FMC_FLASH_PAGE_SIZE >> 2];
	uint16_t two[FMC_FLASH_PAGE_SIZE >> 1];
	uint8_t one[FMC_FLASH_PAGE_SIZE];
} union_t;

union_t f_data;

void fmc_read_bytes(uint32_t addr, uint8_t *bytes, uint16_t length)
{
	uint16_t i = 0;
	uint32_t offset_addr = addr - (addr & 3); // addr-(addr%4)

	if (length > FMC_FLASH_PAGE_SIZE || length > (FMC_FLASH_PAGE_SIZE - (addr % FMC_FLASH_PAGE_SIZE)))
	{
		return;
	}

	while (offset_addr + (i << 2) < addr + length)
	{
		f_data.four[i] = *(__IO uint32_t *)(offset_addr + (i << 2));
		i++;
	}
	for (i = 0; i < (length >> 2) + 1; i++)
	{
		f_data.four[i] = *(__IO uint32_t *)(offset_addr + (i << 2));
	}

	memcpy(bytes, &f_data.one[addr & 3], length);
}
int fmc_earse_page(uint32_t addr)
{
	FLASH_EraseInitTypeDef EraseInitStruct; // 擦除
	uint32_t PAGEError;
	uint16_t offset_index = addr & (FMC_FLASH_PAGE_SIZE - 1); // addr%FMC_FLASH_PAGE_SIZE;
	uint32_t page_addr = addr - offset_index;
	// 擦除
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = page_addr;
	EraseInitStruct.NbPages = 1;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK) // 页面擦除操作
	{
		return HAL_FLASH_GetError();
	}
	return 0;
}
int fmc_write_bytes(uint32_t addr, uint8_t *bytes, uint16_t length)
{
	FLASH_EraseInitTypeDef EraseInitStruct; // 擦除
	uint32_t PAGEError;

	uint16_t i, offset_index = addr & (FMC_FLASH_PAGE_SIZE - 1); // addr%FMC_FLASH_PAGE_SIZE;
	uint32_t page_addr = addr - offset_index;

	if (length > FMC_FLASH_PAGE_SIZE || length > (FMC_FLASH_PAGE_SIZE - offset_index))
	{
		return 1;
	}
	// 读取
	for (i = 0; i < (FMC_FLASH_PAGE_SIZE >> 2); i++)
	{
		f_data.four[i] = *(__IO uint32_t *)(page_addr + (i << 2));
	}
	// 擦除
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = page_addr;
	EraseInitStruct.NbPages = 1;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK) // 页面擦除操作
	{
		return HAL_FLASH_GetError();
	}

	for (i = 0; i < length; i++)
	{
		f_data.one[offset_index + i] = bytes[i];
	}

	for (i = 0; i < (FMC_FLASH_PAGE_SIZE >> 2); i++)
	{
		//		FLASH_ProgramWord(page_addr+(i<<2),f_data.four[i]);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, page_addr + (i << 2), f_data.four[i]);
	}
	return 0;
}

// uint8_t tssend_data[20] = {0x06,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12};
// void test_flash(void)
//{
//
//	fmc_write_bytes(FLASH_START_ADDR,tssend_data,10);
//     memset(tssend_data,0,sizeof(tssend_data));
//     fmc_read_bytes(FLASH_START_ADDR,tssend_data,10);
//
//
// }
