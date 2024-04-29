#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "main.h"

void message_set_zero(void);
void message_standardization(float data);
void message_standardization32(float data);
void message_send_weight(void);
void message_send_AD(void);
void message_set_address(uint8_t addr);
void message_send_version(void);
void message_send_two_channel_weight(void);
void message_two_channel_AD_send(void);

#endif
