/*
 * CS5530.h
 *
 *  Created on: Sep 11, 2023
 *      Author: smile
 */

#ifndef CS5530_H_
#define CS5530_H_

#include "main.h"
#include "stdint.h"
#include "soft_SPI.h"


#define CS5530_FIRST 1
#define CS5530_SECOND 2

#define WO_OFFSET_REG_CMD (0x00 | 0x01)
#define RO_OFFSET_REG_CMD (0x08 | 0x01)
#define WO_GAIN_REG_CMD (0x00 | 0x02)
#define RO_GAIN_REG_CMD (0x08 | 0x02)
#define WO_CONFIG_REG_CMD (0x00 | 0x03)
#define RO_CONFIG_REG_CMD (0x08 | 0x03)
#define SINGLE_CONVER_CMD (0x80 | 0x00)
#define CONTINU_CONVER_CMD (0x80 | 0x40)
#define SYS_OFFSET_CALI_CMD (0x85)
#define SYS_GAIN_CALI_CMD (0x86)
#define SYNC1_CMD (0xff)
#define SYNC0_CMD (0xfe)
#define NULL_CMD (0x00)

#define REG_CONFIG_RS 0x20000000 // 系统复位
#define REG_CONFIG_RV 0x10000000 // 有效复位
#define REG_DATA_OF 0x00000004	 // 溢出标志

// sensor user define

#define KG_FULL_SCALL 40.0																 // 重量满量程 KG
#define SENSITIVITY 2																	 //  灵敏度 mV/V
#define mV_FULL_SCALL SENSITIVITY * 5													 // 重量满量程输出的mV
#define VREF_P 5.0																		 // 参考电源正  V
#define VREF_N 0																		 // 参考电源负  V
#define AD_FULL_SCALL 78.125															 // AD满量程 mV  (VREF_P- VREF_N)/(64)
#define K_mV_KG mV_FULL_SCALL / KG_FULL_SCALL											 //  mV / KG
#define K_FULL_VALUE mV_FULL_SCALL / AD_FULL_SCALL										 // 满量程转换系数
#define DEFAULT_K_CONVERT (KG_FULL_SCALL / ((mV_FULL_SCALL / AD_FULL_SCALL) * 0xffffff)) // AD 重量转换系数

// #define DEFAULT_K_CONVERT (5000.0* SENSITIVITY)/(13107.2*KG_FULL_SCALL*1000)    //默认转换系数  kg

// #define DEFAULT_K_CONVERT    0.00002f  //默认转换系数

// 标定相关
#define STANDARDIZATION_KG 0.5	 // 重量
#define STANDARDIZATION_AD 25000 // 对应AD
#define STANDARDIZATION_K (STANDARDIZATION_KG / STANDARDIZATION_AD)

#define AD_SHAKE 100

#define DEFAULT_DIVISION 0.5f

#define Filter_A 0.12f

#define SENSOR_SHAKE 1000

#define OFFSET_TIME 25			// *10ms
#define STANDARDIZATION_TIME 50 // *10ms

typedef struct
{
	volatile int32_t CS5530_ad_original;
	volatile int32_t CS5530_ad_original_new;
	volatile int32_t CS5530_ad_original_filter_1;
	volatile int32_t CS5530_ad_original_filter_2;
	volatile int32_t AD_value;
//	volatile int32_t AD_original;
	volatile float weight_value;
	volatile float weight_value_new;
	volatile int32_t zero_offset;
	volatile int32_t peel;
	volatile float K_convert;
	volatile uint8_t set_zero_offset_flag;
	volatile float standardization_kg;
	volatile int32_t standardization_AD;
	volatile float standardization_K_convert;
	volatile uint8_t sample_flag;
	volatile uint16_t time_tick;
	volatile uint16_t report_time_tick;

} ADC_Def;


extern ADC_Def ADC;


void ADC_init(void);
void ADC_read_value(void);
void CS5530_set_zero_offset(void);
int32_t ADC_read_ad_data(void);
void CS5530_standardization( float standard_weight);

#endif /* CS5530_H_ */
