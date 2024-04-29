#include "message.h"
#include "modbus.h"
#include "CS5530.h"

// 3.1.0
#define USER_FIRM_VERSION_HH 1
#define USER_FIRM_VERSION_H 0
#define USER_FIRM_VERSION_L 0
const uint32_t firm_version = (USER_FIRM_VERSION_HH << 16 | USER_FIRM_VERSION_H << 8 | USER_FIRM_VERSION_L);

uint8_t modbus_tx_buf[30];
uint16_t tx_crc = 0;
void message_set_zero(void)
{


	CS5530_set_zero_offset();

	modbus_tx_buf[0] = modbus.Slave_addr;
	modbus_tx_buf[1] = 0x06;
	modbus_tx_buf[2] = 0;
	modbus_tx_buf[3] = 1;
	modbus_tx_buf[4] = 0;
	modbus_tx_buf[5] = 0;
	tx_crc = CRC_CHECK(modbus_tx_buf, 6);
	modbus_tx_buf[6] = tx_crc ;
	modbus_tx_buf[7] = tx_crc >> 8;
	modbus_send_packet(modbus_tx_buf, 8);
}

void message_standardization(float data)
{
	uint16_t reg_data=0;
	uint8_t ack_reg=0;

	CS5530_standardization(data);


	modbus_tx_buf[0] = modbus.Slave_addr;
	modbus_tx_buf[1] = 0x06;
	modbus_tx_buf[2] = 0;
	modbus_tx_buf[3] = 5;
	modbus_tx_buf[4] = reg_data>>8;
	modbus_tx_buf[5] = reg_data;
	tx_crc = CRC_CHECK(modbus_tx_buf, 6);
	modbus_tx_buf[6] = tx_crc ;
	modbus_tx_buf[7] = tx_crc >> 8;
	modbus_send_packet(modbus_tx_buf, 8);
}
void message_standardization32(float data)
{
	CS5530_standardization(data);

	modbus_tx_buf[0] = modbus.Slave_addr;
	modbus_tx_buf[1] = 0x10;
	modbus_tx_buf[2] = 0;
	modbus_tx_buf[3] = 5;
	modbus_tx_buf[4] = 0;
	modbus_tx_buf[5] = 2;
	tx_crc = CRC_CHECK(modbus_tx_buf, 6);
	modbus_tx_buf[6] = tx_crc ;
	modbus_tx_buf[7] = tx_crc >> 8;
	modbus_send_packet(modbus_tx_buf, 8);
}

void message_send_weight(void)
{
	int32_t message_weight;
	float wei_f=0;


	wei_f=ADC.weight_value;
	message_weight = (int32_t)(wei_f*1000.0f);


	modbus_tx_buf[0] = modbus.Slave_addr;
	modbus_tx_buf[1] = 0x03;
	modbus_tx_buf[2] = 4;
	modbus_tx_buf[3] = message_weight >> 24;
	modbus_tx_buf[4] = message_weight >> 16;
	modbus_tx_buf[5] = message_weight >> 8;
	modbus_tx_buf[6] = message_weight;
	tx_crc = CRC_CHECK(modbus_tx_buf, 7);
	modbus_tx_buf[7] = tx_crc ;
	modbus_tx_buf[8] = tx_crc >> 8;
	modbus_send_packet(modbus_tx_buf, 9);
}


void message_send_AD(void)
{
	int32_t message_AD;

		message_AD = ADC.AD_value;;

		modbus_tx_buf[0] = modbus.Slave_addr;
		modbus_tx_buf[1] = 0x03;
		modbus_tx_buf[2] = 4;
		modbus_tx_buf[3] = message_AD >> 24;
		modbus_tx_buf[4] = message_AD >> 16;
		modbus_tx_buf[5] = message_AD >> 8;
		modbus_tx_buf[6] = message_AD;
		tx_crc = CRC_CHECK(modbus_tx_buf, 7);
		modbus_tx_buf[7] = tx_crc ;
		modbus_tx_buf[8] = tx_crc >> 8;
		modbus_send_packet(modbus_tx_buf, 9);

}
void message_send_version(void)
{
	uint32_t temp32=0;
	temp32=firm_version;
		modbus_tx_buf[0] = modbus.Slave_addr;
		modbus_tx_buf[1] = 0x03;
		modbus_tx_buf[2] = 4;
		modbus_tx_buf[3] = temp32 >> 24;
		modbus_tx_buf[4] = temp32 >> 16;
		modbus_tx_buf[5] = temp32 >> 8;
		modbus_tx_buf[6] = temp32;
		tx_crc = CRC_CHECK(modbus_tx_buf, 7);
		modbus_tx_buf[7] = tx_crc ;
		modbus_tx_buf[8] = tx_crc >> 8;
		modbus_send_packet(modbus_tx_buf, 9);
}
void message_send_addr(void)
{
	uint32_t temp32=0;
	temp32=modbus.Slave_addr;
		modbus_tx_buf[0] = modbus.Slave_addr;
		modbus_tx_buf[1] = 0x03;
		modbus_tx_buf[2] = 4;
		modbus_tx_buf[3] = temp32 >> 24;
		modbus_tx_buf[4] = temp32 >> 16;
		modbus_tx_buf[5] = temp32 >> 8;
		modbus_tx_buf[6] = temp32;
		tx_crc = CRC_CHECK(modbus_tx_buf, 7);
		modbus_tx_buf[7] = tx_crc ;
		modbus_tx_buf[8] = tx_crc >> 8;
		modbus_send_packet(modbus_tx_buf, 9);
}

void message_set_address(uint8_t addr)
{
	set_modbus_address(addr);
	modbus_tx_buf[0] = modbus.Slave_addr;
	modbus_tx_buf[1] = 0x06;
	modbus_tx_buf[2] = 0;
	modbus_tx_buf[3] = 0x12;
	modbus_tx_buf[4] = addr;
	tx_crc = CRC_CHECK(modbus_tx_buf, 6);
	modbus_tx_buf[5] = tx_crc ;
	modbus_tx_buf[6] = tx_crc >> 8 ;
	modbus_send_packet(modbus_tx_buf, 7);
}
