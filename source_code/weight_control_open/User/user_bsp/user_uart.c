/*
 * user_uart.c
 *
 *  Created on: Sep 22, 2023
 *      Author: smile
 */

#include "user_uart.h"
#include "stdio.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

extern TIM_HandleTypeDef htim2;

uint8_t StaMessages[4] = {0x11, 0x22, 0x33, 0x44};
uint8_t RxBuffer[10];

Uart_Typedef ble_uart = {0};
uint8_t uart_RxBuffer[10];
/**
 * 函数功能: 重定向c库函数printf到DEBUG_USARTx
 * 输入参数: 无
 * 返 回 值: 无
 * 说    明：无
 */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}

/**
 * 函数功能: 重定向c库函数getchar,scanf到DEBUG_USARTx
 * 输入参数: 无
 * 返 回 值: 无
 * 说    明：无
 */
int fgetc(FILE *f)
{
  uint8_t ch = 0;
  HAL_UART_Receive(&huart1, &ch, 1, 0xffff);
  return ch;
}
//重写printf函数
int _write(int32_t file, uint8_t *ptr, int32_t len)
{
	HAL_UART_Transmit(&huart1, ptr, len, HAL_MAX_DELAY);
  return len;
}

//串口初始化
void modbus_uart_init(void)
{

//  HAL_UART_Transmit_IT(&huart1, (uint8_t *)StaMessages, sizeof(StaMessages));
  HAL_UART_Receive_IT(&huart1, (uint8_t *)RxBuffer, 1); // 调用中断接收函数，接收长度设为1，接收一个字节进一次中断
  HAL_TIM_Base_Start_IT(&htim2);
}

__weak void uart1_callback(void)
{


}

uint8_t uart1_Receive_one_byte(void)
{
	return RxBuffer[0];
}



void uart1_send_data(uint8_t *data, uint16_t len)
{
	HAL_UART_Transmit(&huart1, data, len, 100); // 发送数据
}

void set_uart1_baud(uint16_t baud)
{

	  huart1.Instance = USART1;
	  huart1.Init.BaudRate = baud;
	  huart1.Init.WordLength = UART_WORDLENGTH_8B;
	  huart1.Init.StopBits = UART_STOPBITS_1;
	  huart1.Init.Parity = UART_PARITY_NONE;
	  huart1.Init.Mode = UART_MODE_TX_RX;
	  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	  if (HAL_UART_Init(&huart1) != HAL_OK)
	  {
	    Error_Handler();
	  }
}



uint8_t XOR_Check(uint8_t *bufpt, uint8_t num)
{
    uint8_t i;
    uint8_t temp;
    temp = *bufpt++;
    for (i = 1; i < num; i++)
    {
        temp = temp ^ *bufpt++;
    }

    return temp;
}


void ble_send_data(uint8_t *data, uint16_t len)
{
	HAL_UART_Transmit(&huart3, data, len, 100); // 发送数据
}


/**
 * @brief 串口数据接收
 *
 */
void ble_Message_analysis(void)
{
    if (ble_uart.RxBuf[0] == 0xAA)
    {
        if (ble_uart.RxCnt == 4)
        {
        	ble_uart.RxNum = ble_uart.RxBuf[3];
        }
        if (((ble_uart.RxNum > Uart_Rx_MaxSize) || (ble_uart.RxNum < 7) || (ble_uart.RxCnt > ble_uart.RxNum)) && (ble_uart.RxCnt > 4))
        {
        	ble_uart.RxCnt = 0;
        	ble_uart.RxNum = 0;
        	ble_uart.RxState = 0;
//        	ble_uart.valid = 0;
        }
        else if (ble_uart.RxNum == ble_uart.RxCnt)
        {
            if (XOR_Check(ble_uart.RxBuf, ble_uart.RxNum) == 0)
            {
            	ble_uart.RxState = 1;
            	ble_uart.RxCnt = 0;
            	ble_uart.RxNum = 0;
                // ble_uart.valid=0;
            }
            else
            {
            	ble_uart.RxCnt = 0;
            	ble_uart.RxNum = 0;
            	ble_uart.RxState = 0;

            }
        }
    }

    else
    {
    	ble_uart.RxCnt = 0;
    	ble_uart.RxNum = 0;
    	ble_uart.RxState = 0;
//    	ble_uart.valid = 0;
    }
}




void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		  UNUSED(huart);
		  HAL_UART_Receive_IT(&huart1, (uint8_t *)RxBuffer, 1);       // 再开启接收中断（因为里面中断只会触发一次，因此需要再次开启）
		  uart1_callback();
	}
	if(huart->Instance == USART3)
	{
		HAL_UART_Receive_IT(&huart3, (uint8_t *)uart_RxBuffer, 1);       // 再开启接收中断（因为里面中断只会触发一次，因此需要再次开启）
		ble_uart.RxBuf[ble_uart.RxCnt++] = uart_RxBuffer[0];//
		ble_Message_analysis();
	}

}

