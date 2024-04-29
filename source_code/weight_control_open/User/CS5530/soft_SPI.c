/*
 * soft_SPI.c
 *
 *  Created on: Sep 11, 2023
 *      Author: smile
 */

#include "soft_SPI.h"

void SPI_Delay_us(uint32_t nus)
{
    uint32_t Delay = nus * 20;
    do
    {
        __NOP();
    }
    while (Delay --);
}

void SPI_Delay_ms(uint16_t ms)
{
	uint16_t i;
	for(i=0;i<ms;i++)
	{
		SPI_Delay_us(1000);
	}
}

//写一个字节数据
void ADC_SPI_Write_Byte(uint8_t dat)
{
    uint8_t i;


    for(i=0;i<8;i++)
    {
        if(dat&0x80)
        {
            ADC_SDI_HIGH();
        }
        else
        {
            ADC_SDI_LOW();
        }
        ADC_SCK_HIGH();
        SPI_Delay_us(1);
        ADC_SCK_LOW();
        SPI_Delay_us(1);
		dat <<= 1;
    }
		 ADC_SDI_LOW();
		 SPI_Delay_us(1);
}

//写数据
void ADC_SPI_Write_Data(uint32_t dat)
{
    int i;
    uint8_t tmp;

    for(i=3;i>=0;i--)
    {
        tmp = (uint8_t) (dat>>(8*i));
        ADC_SPI_Write_Byte(tmp);
    }
}

//写寄存器
void ADC_SPI_Write_Reg(uint8_t reg,uint32_t dat)
{
	ADC_SPI_Write_Byte(reg);
	ADC_SPI_Write_Data(dat);
}

//读一个字节数据
uint8_t ADC_SPI_Read_Byte(void)
{
    int i;
    uint8_t dat=0;

		ADC_SDI_LOW();
//	  SPI_Delay_us(3);
    for(i=0;i<8;i++)
    {
			ADC_SCK_HIGH();
			SPI_Delay_us(1);
			dat <<= 1;
			if(ADC_SDO())
			{
				dat |= 0x01;
			}
			else
			{
				dat &= 0xFE;
			}

			ADC_SCK_LOW();
			SPI_Delay_us(1);
    }

    return dat;
}

//读数据
uint32_t ADC_SPI_Read_Data(void)
{
    uint8_t i;
    uint32_t dat=0;

    for(i=0;i<4;i++)
    {
        dat <<= 8;
        dat |= ADC_SPI_Read_Byte();
    }

    return dat;
}

//读寄存器
uint32_t ADC_SPI_Read_Reg(uint8_t reg)
{
    uint32_t dat;

    ADC_SPI_Write_Byte(reg);
	  ADC_SDI_LOW();
    dat = ADC_SPI_Read_Data();

    return dat;
}



