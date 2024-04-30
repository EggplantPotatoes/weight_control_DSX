#ifndef _DATA_FILTER_H_
#define _DATA_FILTER_H_
#include "main.h"
#include "CS5530.h"
#include "math.h"
#include "soft_spi.h"

#define SAMPLE_NUM 5
#define DEL_NUM 3
#define MAX_MIN_WINDOW 25
#define MAX_MIN_AVR_NUM 5

#define Pi 3.1415926f
#define HP_CUT_FRQ 5
#define SAMPLE_RATE 25
#define WIN_NUM 10

typedef struct
{
	int32_t max_new;
//	int32_t max_old;
	int32_t min_new;
//	int32_t min_old;
	int32_t cnt;
	int32_t data_input_last;
	int32_t max_value[MAX_MIN_AVR_NUM];
	int32_t min_value[MAX_MIN_AVR_NUM];
	int32_t max_num;
	int32_t min_num;
	int32_t max_sum;
	int32_t min_sum;
	int32_t max_avr;
	int32_t min_avr;
} MAX_MIN_FILTER;



//extern int32_t test_window_buf[WIN_NUM];
extern int32_t ad1_window_buf[WIN_NUM];
extern int32_t ad2_window_buf[WIN_NUM];
extern int32_t ad3_window_buf[WIN_NUM];
extern int32_t ad4_window_buf[WIN_NUM];

int32_t low_pass_filter(int32_t Vi);
int32_t window_filter(int32_t data, int32_t *buf, uint8_t len);
uint32_t data_filter(uint32_t ADC_Value);
int32_t max_min_filter(int32_t data_input, MAX_MIN_FILTER *mm_data);



#define MAX_ITERATIONS 20 // 最大迭代次数
#define R_MEASUREMENT 12.5f // 测量噪声协方差
#define Q_ESTIMATION 0.0001f // 状态估计噪声协方差
//#define ADC_DATA_COUNT 20 // ADC采样数据的数量



#endif
