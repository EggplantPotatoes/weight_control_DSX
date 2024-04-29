/*
 * soft_SPI.h
 *
 *  Created on: Sep 11, 2023
 *      Author: smile
 */

#ifndef SOFT_SPI_H_
#define SOFT_SPI_H_

#include "stdint.h"
#include "main.h"



#define  ADC_CS_LOW()    HAL_GPIO_WritePin(ADC_CS_GPIO_Port, ADC_CS_Pin,GPIO_PIN_RESET)
#define  ADC_CS_HIGH()   HAL_GPIO_WritePin(ADC_CS_GPIO_Port, ADC_CS_Pin,GPIO_PIN_SET)
#define  ADC_SCK_LOW()    HAL_GPIO_WritePin(ADC_SCK_GPIO_Port, ADC_SCK_Pin,GPIO_PIN_RESET)
#define  ADC_SCK_HIGH()   HAL_GPIO_WritePin(ADC_SCK_GPIO_Port, ADC_SCK_Pin,GPIO_PIN_SET)
#define  ADC_SDI_LOW()    HAL_GPIO_WritePin(ADC_SDI_GPIO_Port, ADC_SDI_Pin,GPIO_PIN_RESET)
#define  ADC_SDI_HIGH()   HAL_GPIO_WritePin(ADC_SDI_GPIO_Port, ADC_SDI_Pin,GPIO_PIN_SET)
#define  ADC_SDO()        HAL_GPIO_ReadPin(ADC_SDO_GPIO_Port, ADC_SDO_Pin)


void SPI_Delay_ms(uint16_t ms);
void SPI_Delay_us(uint32_t ms);
//void SPI_GPIO_Init(void);
void ADC_SPI_Write_Byte(uint8_t dat) ;
void ADC_SPI_Write_Data(uint32_t dat);
void ADC_SPI_Write_Reg(uint8_t reg,uint32_t dat);
uint8_t ADC_SPI_Read_Byte(void);
uint32_t ADC_SPI_Read_Data(void);
uint32_t ADC_SPI_Read_Reg(uint8_t reg);


#endif /* SRC_SOFT_SPI_H_ */
