#ifndef _MODBUS_H_
#define _MODBUS_H_

#include "main.h"

#define RX_SIZE   20
typedef struct
{
	uint8_t Slave_addr;		 // 从机地址
	uint8_t ReceiveBuff[RX_SIZE]; // 接收缓存数组
	uint8_t ReceiveCount;	 // 计算接收到的数据有多少字节
	uint8_t timecount;		 // 有多久没有接收到字节，数据断续的时间
	uint8_t timerun;		 // 断续时间是否开始累加
	uint8_t ReceiveComplete; // 一帧数据接收完成标志

	uint16_t reg_addr;		// 寄存器地址
	uint16_t write_data;	// 写入的数据
	uint32_t write_data32;	// 写入的数据
	uint16_t read_data_num; // 写入的数据

} MODBUS;

extern MODBUS modbus;

uint8_t modbus_receive_packet(uint8_t *rx_data);
uint16_t CRC_CHECK(uint8_t *Buf, uint8_t CRC_CNT);
void modbus_send_packet(uint8_t *data, uint8_t len);
void modbus_timeout(void);
void modbus_init(void);
void set_modbus_address(uint8_t addr);
#endif
