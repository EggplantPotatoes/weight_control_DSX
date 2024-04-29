
#include "modbus.h"
#include "user_uart.h"
#include "message.h"
#include "flash.h"
#include "soft_SPI.h"
#include "CS5530.h"

MODBUS modbus;

//// CRC16校验算法
//uint16_t CRC_CHECK(uint8_t *Buf, uint8_t CRC_CNT)
//{
//	uint16_t CRC_Temp;
//	uint16_t i, j;
//	CRC_Temp = 0xffff;
//
//	for (i = 0; i < CRC_CNT; i++)
//	{
//		CRC_Temp ^= Buf[i];
//		for (j = 0; j < 8; j++)
//		{
//			if (CRC_Temp & 0x01)
//				CRC_Temp = (CRC_Temp >> 1) ^ 0xa001;
//			else
//				CRC_Temp = CRC_Temp >> 1;
//		}
//	}
//	return (CRC_Temp);
//}

// 计算 Modbus RTU CRC 校验码
uint16_t CRC_CHECK(uint8_t *data, uint8_t length)
{
    uint16_t crc = 0xFFFF;

    for (size_t i = 0; i < length; ++i) {
        crc ^= data[i];

        for (int j = 0; j < 8; ++j) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }

    return crc;
}

void Modbud_fun_03(uint16_t reg_addr) // 3号功能码处理  ---查询功能
{
	switch (reg_addr)
	{
	case 0x0009: //
		message_send_weight();
		break;
	case 0x000b: //
		break;

	case 0x0011: //
		message_send_AD();
		break;
	case 0x0014: //

			break;
	case 0x0090: //
		message_send_version();
		break;
	}
}

void Modbud_fun_06(uint16_t reg_addr) // 6号功能码处理  修改功能
{
	switch (reg_addr)
	{
	case 0x0001: // 零点校准
		message_set_zero();
		break;
	case 0x0002: // 零点校准
		message_set_zero();
		break;
	case 0x0005: // 通道1 重量标定
		message_standardization(modbus.write_data);
		break;
	case 0x0007: // 通道2 重量标定
//		message_standardization(modbus.write_data);
		break;
	case 0x0012: // 地址修改
		message_set_address(modbus.write_data);
		break;
	}
}

void Modbud_fun_10(uint16_t reg_addr) // 16号功能码处理  修改多个寄存器功能
{
	float temp_f=0;
	switch (reg_addr)
		{
//		case 0x0001: // 零点校准
//			message_set_zero();
//			break;
		case 0x0005: // 重量标定
			temp_f=(float)modbus.write_data32;
			message_standardization32(temp_f);
			break;
		case 0x0007: // 重量标定
			temp_f=(float)modbus.write_data32;
			message_standardization32(temp_f);
					break;
//		case 0x0012: // 地址修改
//			message_set_address(modbus.write_data);
//			break;
		}
}

uint8_t modbus_receive_packet(uint8_t *rx_data)
{
	uint16_t crc = 0;
	uint16_t rc_crc = 0;
	//  uint16_t reg_addr ;

	crc = CRC_CHECK(rx_data, modbus.ReceiveCount - 2);									   // 计算校验码
	rc_crc = (rx_data[modbus.ReceiveCount - 1] << 8) + (rx_data[modbus.ReceiveCount - 2]); // 收到的校验码
	if (crc != rc_crc)
	{
		return 1;
	}
	if (rx_data[0] == modbus.Slave_addr)
	{
		modbus.reg_addr = (rx_data[2] << 8) + rx_data[3];
		switch (rx_data[1])
		{
		case 0x03: // 读取多个寄存器
			modbus.read_data_num = (rx_data[4] << 8) + rx_data[5];
			Modbud_fun_03(modbus.reg_addr);
			break;

		case 0x06: // 读取多个输入寄存器
			modbus.write_data = (rx_data[4] << 8) + rx_data[5];
			Modbud_fun_06(modbus.reg_addr);
			break;

		case 0x10: // 写入单个寄存器
			modbus.write_data32 = (rx_data[7] << 24) +(rx_data[8] << 16)+(rx_data[9] << 8) + rx_data[10];
			Modbud_fun_10(modbus.reg_addr);
			break;
		}
	}
	else if (rx_data[0] == 0) // 广播地址
	{
	}
	return 0;
}

// modbus串口发送一包数据
void modbus_send_packet(uint8_t *data, uint8_t len)
{
	uart1_send_data(data, len);
}


void uart1_callback(void)
{
	uint8_t res = 0;
	res = uart1_Receive_one_byte();
	modbus.ReceiveBuff[modbus.ReceiveCount] = res;
	modbus.ReceiveCount++;
	if(modbus.ReceiveCount >=RX_SIZE)
	{
		modbus.ReceiveCount = 0;
	}
	if (modbus.ReceiveCount == 1)
	{
		modbus.timerun = 1;
	}

}
void modbus_timeout(void)
{
	if (modbus.timerun == 1)
	{
		modbus.timecount++;
	}
	if (modbus.timecount >= 200) // 大于3.5个字符时间,接受到一包数据
	{
		modbus.timerun = 0;
		modbus.timecount = 0;
		modbus.ReceiveComplete = 1;
		modbus_receive_packet(modbus.ReceiveBuff); //解析数据
		modbus.ReceiveCount = 0;
	//			modbus.ReceiveComplete = 0;
	}

}

void modbus_init(void)
{
	uint16_t flash_read[4] = {0x00, 0x00, 0x00, 0x00};

	STMFLASH_Read(DEVICE_ADDR, flash_read, 1); // 读取modbus地址
	SPI_Delay_us(200);
	modbus.Slave_addr = flash_read[0];
	if (flash_read[0] == 0xffff) // 初次上电
	{
		modbus.Slave_addr = 1;
	}


}

void set_modbus_address(uint8_t addr)
{
	uint16_t flash_write[2] = {0x00, 0x00};

	flash_write[0] = (uint16_t)(addr);
	STMFLASH_Write(DEVICE_ADDR, flash_write, 1);


}
