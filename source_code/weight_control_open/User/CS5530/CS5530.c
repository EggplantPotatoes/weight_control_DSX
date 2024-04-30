/*
 * CS5530.c
 *
 *  Created on: Sep 11, 2023
 *      Author: smile
 */
#include "CS5530.h"
#include "stdio.h"
#include "data_filter.h"
#include "flash.h"
#include "stdio.h"

extern TIM_HandleTypeDef htim3;

ADC_Def ADC;


MAX_MIN_FILTER mm_filter_1;
//MAX_MIN_FILTER mm_filter_1_2;



void read_flash_data_for_CS5530(void)
{
	uint16_t flash_read[4] = {0x00, 0x00, 0x00, 0x00};
	uint32_t flash_read_temp = 0;


	STMFLASH_Read(USER_OFFSET_1, flash_read, 2); // 读取0点
	SPI_Delay_us(200);
	ADC.zero_offset = (flash_read[0] << 16) + flash_read[1];
	if ((flash_read[0] == 0xffff) && (flash_read[1] == 0xffff)) // 初次上电
	{
		ADC.zero_offset = 0;
	}
	STMFLASH_Read(STANDARDIZATION_1, flash_read, 2); // 标定的K值
	SPI_Delay_us(200);
	flash_read_temp = (flash_read[0] << 16) + flash_read[1];
	ADC.K_convert = (float)flash_read_temp / 100000000;
	if ((flash_read[0] == 0xffff) && (flash_read[1] == 0xffff))
	{
		ADC.K_convert = DEFAULT_K_CONVERT;
	}

}

void ADC1_init(void)
{
	uint8_t i;
	uint32_t tmp = 0x00AA;

	ADC_CS_LOW();
	SPI_Delay_ms(5);

	ADC_SDI_LOW();
	ADC_SCK_LOW();

	//	/* 复位串口进入命令模式 */
	for (i = 0; i < 16; i++)
	{
		ADC_SPI_Write_Byte(SYNC1_CMD); // 初始化串口
	}
	ADC_SPI_Write_Byte(SYNC0_CMD);
	SPI_Delay_ms(1);

	/* 系统复位 */
	ADC_SPI_Write_Reg(WO_CONFIG_REG_CMD, REG_CONFIG_RS);
	SPI_Delay_ms(1);

	/* 清除系统复位标志 */
	ADC_SPI_Write_Reg(WO_CONFIG_REG_CMD, 0x00000000);
	SPI_Delay_ms(1);

	/* 读取系统复位完成标志 */
	do
	{
		tmp = ADC_SPI_Read_Reg(RO_CONFIG_REG_CMD);
		SPI_Delay_ms(50);
	} while (tmp & REG_CONFIG_RV);

	ADC_SPI_Write_Reg(WO_GAIN_REG_CMD, 0x02000000); // 设置放大倍数128
//	SPI_Write_Reg(WO_GAIN_REG_CMD, 0x04000000); // 设置放大倍数256
	SPI_Delay_ms(1);
	//	tmp = SPI_Read_Reg(RO_GAIN_REG_CMD); //默认放大倍数是64
	//	SPI_Delay_ms(10);
	////

	tmp = ADC_SPI_Read_Reg(RO_CONFIG_REG_CMD);
	SPI_Delay_ms(1);
//	ADC_SPI_Write_Reg(WO_CONFIG_REG_CMD, tmp | 0x00082000); // FRS =1  WR3~WR0  0100    6.25Sps
	ADC_SPI_Write_Reg(WO_CONFIG_REG_CMD, tmp | 0x00081000); // FRS =1  WR3~WR0  0010    25Sps
//	ADC_SPI_Write_Reg(WO_CONFIG_REG_CMD, tmp | 0x00080800); // FRS =1  WR3~WR0  0010    50Sps
//	ADC_SPI_Write_Reg(WO_CONFIG_REG_CMD, tmp | 0x00080000); // FRS =1  WR3~WR0  0000    100Sps

	SPI_Delay_ms(1);

	tmp = ADC_SPI_Read_Reg(RO_CONFIG_REG_CMD); //
	SPI_Delay_ms(1);

	// CS5530默认 VRS = 0  单极性
	ADC_SPI_Write_Byte(CONTINU_CONVER_CMD); // 连续转换模式
	SPI_Delay_ms(1);
}

void ADC_init(void)
{
	ADC1_init();
	read_flash_data_for_CS5530();
	HAL_TIM_Base_Start_IT(&htim3); //开启采样定时器，50Hz
}

int32_t ADC_read_ad_data(void)
{
	int32_t data_temp;
	int32_t ad_data;
	data_temp = ADC_SPI_Read_Reg(NULL_CMD);
	data_temp &= 0xFFFFFF00;
	ad_data = (data_temp >> 8);
//	if(ad_data>12582912)  //  如果传感器没有重物，可能会有负值出现，滤掉负值
//	{
//		ad_data = 16777216- ad_data;
//	}
	return (ad_data >> 5); // 去掉噪声位
}


void ADC_read_value(void)
{
		ADC.CS5530_ad_original = ADC_read_ad_data();
//		printf("ADC1_data = %d \r\n",ADC.CS5530_ad_original);
		ADC.CS5530_ad_original_filter_1 = window_filter(ADC.CS5530_ad_original, ad1_window_buf, WIN_NUM);
		ADC.CS5530_ad_original_filter_2 = max_min_filter(ADC.CS5530_ad_original_filter_1, &mm_filter_1);
		ADC.CS5530_ad_original_new  = ADC.CS5530_ad_original_filter_2;
		ADC.AD_value = ADC.CS5530_ad_original_new - ADC.zero_offset - ADC.peel;
		ADC.weight_value_new = ADC.AD_value * ADC.K_convert;
		ADC.weight_value = ADC.weight_value_new;
//		printf("weight = %f \r\n",ADC.weight_value);

}

void CS5530_set_zero_offset(void)
{
	uint16_t flash_write[2] = {0x00, 0x00};

		ADC.zero_offset = ADC.CS5530_ad_original_new;
		flash_write[0] = ADC.zero_offset >> 16;
		flash_write[1] = ADC.zero_offset;
		STMFLASH_Write(USER_OFFSET_1, flash_write, 2);


}

void CS5530_standardization(float standard_weight)
{
	uint16_t flash_write[2] = {0x00, 0x00};
	float k_temp;
		ADC.standardization_AD = ADC.CS5530_ad_original_new - ADC.zero_offset;
		ADC.standardization_kg = standard_weight/1000.0f;
		ADC.standardization_K_convert = ADC.standardization_kg / ADC.standardization_AD;
		ADC.K_convert = ADC.standardization_K_convert;
		k_temp = ADC.K_convert * 100000000;
		flash_write[0] = (uint32_t)(k_temp) >> 16;
		flash_write[1] = (uint32_t)(k_temp);
		STMFLASH_Write(STANDARDIZATION_1, flash_write, 2);

}

