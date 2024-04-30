#include "data_filter.h"
#include "stdlib.h"

int32_t ad1_window_buf[WIN_NUM];


/**
 * @brief  高通滤波器
 * @author
 * @param  in-输入数据 out-滤波输出数据
 * @return void
 */
void high_pass_filter(float in, float *out)
{
	float rc, coff;
	static float in_p, out_p;

	rc = 1.0 / 2.0 / Pi / HP_CUT_FRQ;
	coff = rc / (rc + 1 / SAMPLE_RATE);
	*out = (in - in_p + out_p) * coff;

	out_p = *out;
	in_p = in;
}

/**
 * @brief  implement 1 order RC low pass filter
 *         raw data filtered by a simple RC low pass filter@cufoff=5Hz
 * @param  Vi 		: 	Vi(k)
 * @param  Vi_p 	: 	Vi(k-1)
 * @param  Vo 		: 	Vo(k)
 * @param  Vo_p 	: 	Vo(k-1)
 * @note   This example shows a simple way to report end of conversion
 *         and get conversion result. You can add your own implementation.
 * @retval None
 */
int32_t low_pass_filter(int32_t Vi)
{
	float CutFrq, RC, Cof1, Cof2;
	static int32_t Vo_p;
	int32_t Vo;
	// low pass filter @cutoff frequency = 5 Hz
	CutFrq = 1;
	RC = (float)1.0 / 2.0 / Pi / CutFrq;
	Cof1 = 1 / (1 + RC * SAMPLE_RATE);
	Cof2 = RC * SAMPLE_RATE / (1 + RC * SAMPLE_RATE);
	Vo = Cof1 * (Vi) + Cof2 * (Vo_p);
	// update
	Vo_p = Vo;

	return Vo;
}

// 冒泡排序
void bubblesort(int32_t *arr, uint8_t len) // 小--》大
{
	int32_t temp;
	uint8_t i, j;
	for (i = 0; i < len - 1; i++) /* 外循环为排序趟数，len个数进行len-1趟 */
	{
		for (j = 0; j < len - 1 - i; j++)
		{ /* 内循环为每趟比较的次数，第i趟比较len-i次 */
			if (arr[j] > arr[j + 1])
			{ /* 相邻元素比较，若逆序则交换（升序为左大于右，降序反之） */
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

// 滑动窗口滤波
int32_t window_filter(int32_t data, int32_t *buf, uint8_t len)
{
	uint8_t i;
	int32_t sum = 0;

	for (i = 1; i < len; i++)
	{
		buf[i - 1] = buf[i];
	}
	buf[len - 1] = data;

	for (i = 0; i < len; i++)
	{
		sum += buf[i];
	}

	sum /= len;

	return sum;
}



int32_t max_min_filter(int32_t data_input, MAX_MIN_FILTER *mm_data) // 最大值最小值滤波
{
	int8_t i;
//	int32_t max_ret, min_ret;
	if (abs(data_input - mm_data->data_input_last) > 10) // 与上次值相差太大，则开始滤波
	{
		mm_data->cnt = 0;
		mm_data->min_sum = 0;
		mm_data->min_sum = 0;
		mm_data->max_num = 0;
		mm_data->min_num = 0;
	}
	if (mm_data->cnt == 0) // 第一次，初始化最大最小值
	{
		mm_data->max_new = data_input;
		mm_data->min_new = data_input;
	}
	if (data_input > mm_data->max_new) // 最大值更新
	{
		mm_data->max_new = data_input;
		mm_data->max_value[mm_data->max_num] = mm_data->max_new;
		mm_data->max_num++;

		if (mm_data->max_num >= MAX_MIN_AVR_NUM)
		{
			mm_data->max_num = 0;
			mm_data->max_sum = 0;
		}
	}
	if (data_input < mm_data->min_new) // 最小值更新
	{
		mm_data->min_new = data_input;
		mm_data->min_value[mm_data->min_num] = mm_data->min_new;
		mm_data->min_num++;
		mm_data->min_sum = mm_data->min_sum + mm_data->min_value[mm_data->min_num];
		mm_data->min_avr = mm_data->min_sum / mm_data->min_num;
		if (mm_data->min_num >= MAX_MIN_AVR_NUM)
		{
			mm_data->min_num = 0;
			mm_data->min_sum = 0;
		}
	}
	mm_data->cnt++;
	if (mm_data->cnt == MAX_MIN_WINDOW) // 滤波窗口大小
	{
		mm_data->cnt = 0;
	}
	mm_data->data_input_last = data_input;

	if (mm_data->max_num > 0)
	{
		mm_data->max_sum = 0;
		for (i = 0; i < mm_data->max_num; i++) // 最大值累加
		{
			mm_data->max_sum = mm_data->max_sum + mm_data->max_value[i];
		}
		mm_data->max_avr = mm_data->max_sum / mm_data->max_num;
	}
	else
	{
		mm_data->max_avr = mm_data->max_new;
	}

	if (mm_data->min_num > 0)
	{
		mm_data->min_sum = 0;
		for (i = 0; i < mm_data->min_num; i++) // 最大值累加
		{
			mm_data->min_sum = mm_data->min_sum + mm_data->min_value[i];
		}
		mm_data->min_avr = mm_data->min_sum / mm_data->min_num;
	}
	else
	{
		mm_data->min_avr = mm_data->max_new;
	}
	return (mm_data->max_avr + mm_data->min_avr) / 2; // 返回最大最小值的平均值
}

 
 
