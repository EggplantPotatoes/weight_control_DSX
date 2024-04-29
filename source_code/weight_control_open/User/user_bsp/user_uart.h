/*
 * user_uart.h
 *
 *  Created on: Sep 22, 2023
 *      Author: smile
 */

#ifndef USER_UART_H_
#define USER_UART_H_

#include "main.h"

#define Uart_Rx_MaxSize  20

typedef struct Uart_Type
{
	uint8_t RxState;
	uint8_t where;
	uint16_t RxCnt;
	uint16_t TxCnt;
	uint16_t RxNum;

//    uint8_t  Source;   //表明哪个串口接收
//    uint8_t  Transpond;  //表明给谁

	uint8_t RxBuf[Uart_Rx_MaxSize];
	uint8_t TxBuf[Uart_Rx_MaxSize];
//    uint32_t timerstamp;
//    uint8_t Flag;
//       uint8_t device_address;
//       uint8_t destination_address;
} Uart_Typedef;

extern Uart_Typedef ble_uart;

void modbus_uart_init(void);
uint8_t uart1_Receive_one_byte(void);
void set_uart1_baud(uint16_t baud);
void uart1_send_data(uint8_t *data, uint16_t len);

uint8_t XOR_Check(uint8_t *bufpt, uint8_t num);
void ble_uart_init(void);
void ble_send_data(uint8_t *data, uint16_t len);
#endif /* USER_UART_H_ */
