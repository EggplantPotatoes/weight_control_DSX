/*
 * @FilePath: \stm32f103_ads1232\user_bsp\flash.h
 * @brief: 
 * @author: liumingming
 * @Date: 2024-04-23 16:08:27
 * @Version: 1.0
 */
#ifndef _FLASH_H
#define _FLASH_H

#include "main.h"

#define STM32_FLASH_SIZE 128 // 芯片flash大小
#if STM32_FLASH_SIZE < 256
#define STM_SECTOR_SIZE 1024 // �ֽ�
#else
#define STM_SECTOR_SIZE 2048
#endif
#define STM32_FLASH_BASE 0x08000000      // STM32 FLASH起始地址
#define USER_FLASH_ADDR_START 0x0801FC00 // 用户使用的flash起始地址

#define WEIGHT_STORAGE_FLASH_ADDR   USER_FLASH_ADDR_START

#define USER_OFFSET_1 USER_FLASH_ADDR_START // 零点存储位置 4 uint16_t
#define USER_OFFSET_2 USER_OFFSET_1 + 4     // 零点存储位置 4 uint16_t
#define USER_OFFSET_3 USER_OFFSET_2 + 4     // 零点存储位置 4 uint16_t
#define USER_OFFSET_4 USER_OFFSET_3 + 4     // 零点存储位置 4 uint16_t

#define STANDARDIZATION_1 USER_OFFSET_4 + 4     // 标定系数存储位置 4 uint16_t
#define STANDARDIZATION_2 STANDARDIZATION_1 + 4 // 标定系数存储位置 4 uint16_t
#define STANDARDIZATION_3 STANDARDIZATION_2 + 4 // 标定系数存储位置 4 uint16_t
#define STANDARDIZATION_4 STANDARDIZATION_3 + 4 // 标定系数存储位置 4 uint16_t

#define DEVICE_ADDR STANDARDIZATION_4 + 4 // 设备地址 2 uint16_t
#define POINT_NUM_1 DEVICE_ADDR + 2       // 1路小数点位数 2
#define POINT_NUM_2 POINT_NUM_1 + 2       // 2路小数点位数 2
#define POINT_NUM_3 POINT_NUM_2 + 2       // 3路小数点位数 2
#define POINT_NUM_4 POINT_NUM_3 + 2       // 4路小数点位数 2
#define WEIGH_NUM POINT_NUM_4 + 2         // 称重路数 2
#define BAUD_RATE WEIGH_NUM + 2           // 波特率 4
#define UINITY_1 BAUD_RATE + 4            // 1路单位 2
#define UINITY_2 UINITY_1 + 2             // 2路单位 2
#define UINITY_3 UINITY_2 + 2             // 3路单位 2
#define UINITY_4 UINITY_3 + 2             // 4路单位 2
#define DIVISION_1 UINITY_4 + 2           // 1路分度值
#define DIVISION_2 DIVISION_1 + 2         // 2路分度值
#define DIVISION_3 DIVISION_2 + 2         // 3路分度值
#define DIVISION_4 DIVISION_3 + 2         // 4路分度值

void STMFLASH_Write_NoCheck(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite);
void STMFLASH_Write(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite);
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);
void STMFLASH_Read(uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead);
int FLASH_ErasePage(uint32_t pageAddress, uint32_t nbPages);

#endif
