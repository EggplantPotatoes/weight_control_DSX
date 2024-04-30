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

//int32_t adc1_average_filter(void) // 滤波
//{
//	int32_t sum = 0;
//	int16_t i;
//	int32_t temp_data[SAMPLE_NUM];
//	int32_t average_data = 0;
//
//	for (i = 0; i < SAMPLE_NUM; i++)
//	{
//		temp_data[i] = ADC1_read_ad_data();
//		if(temp_data[i]>((16777216>>5)-SENSOR_SHAKE))  //  如果传感器没有重物，可能会有负值出现，滤掉负值
//		{
//		 temp_data[i] = temp_data[i] - (16777216>>5) ;
//		}
//	  SPI_Delay_us(5);
//	}
//	bubblesort(temp_data,SAMPLE_NUM);
//	for(i = DEL_NUM; i < SAMPLE_NUM-DEL_NUM; i++) // 去掉最大值，去掉最小值 ，取平均
//	{
//		sum = sum + temp_data[i];
//	}
//	average_data = sum / (SAMPLE_NUM-DEL_NUM*2);
////	sum = 0;
//	return (average_data);
//}

int32_t averageFilter(void) // 滤波
{
	int32_t sum = 0;
	static int32_t temp_data[SAMPLE_NUM];
	static int32_t average_data_old = 0;
	static int32_t average_data = 0;
	static int32_t delta_data = 0;
	static int32_t out_data = 0;
	static int32_t out_data_old = 0;
	static int32_t out_data_new = 0;
	uint8_t i;
	for (i = 0; i < SAMPLE_NUM; i++)
	{
		temp_data[i] = ADC_read_ad_data();
		//				if(temp_data[i]>((16777216>>1)-SENSOR_SHAKE))  //  如果传感器没有重物，可能会有负值出现，滤掉负值
		//				{
		//				 temp_data[i] = temp_data[i] - (16777216>>1) ;
		//				}
		//			  SPI_Delay_us(1);
	}
	//		bubblesort(temp_data,SAMPLE_NUM);
	for (i = 0; i < SAMPLE_NUM; i++) // 去掉最大值，去掉最小值 ，取平均
	{
		sum = sum + temp_data[i];
	}
	average_data = sum / (SAMPLE_NUM);
	delta_data = abs(average_data - average_data_old);
	if (delta_data > 5) // 去掉跳动较大的数据，与上次做平均
	{
		//			out_data = (average_data+average_data_old)/2;
		out_data = average_data_old;
	}
	else
	{
		out_data = average_data;
	}
	average_data_old = average_data;

	out_data_new = out_data;
	out_data = out_data_new * Filter_A + out_data_old * (1 - Filter_A); // 一阶互补滤波
	out_data_old = out_data_new;
	//    return (average_data);
	return (out_data);
}

int32_t kalman_filter(int32_t ADC_Value)
{
	int32_t LastData;
	int32_t NowData;
	int32_t kalman_adc;
	static int32_t ADC_OLD_Value;
	static int32_t kalman_adc_old = 500.0f;
	static float P1;
	static float Q = 0.005;
	static float R = 1.5;
	static float Kg = 0;
	static float P = 1;

	/*R值固定，Q值越大，代表越信任测量值，Q值无穷大，代表只用测量值。
	  Q值越小，代表越信任模型预测值，Q值为0，代表只用模型预测值。

		Q:过程噪声，Q增大，动态响应变快，收敛稳定性变坏
		R:测量噪声，R增大，动态响应变慢，收敛稳定性变好
	*/

	NowData = ADC_Value;
	//  比较一段时间内的ADC采样值与优化值差值，若一直处于某个范围，采用一阶滞后滤波算法，反之采用卡尔曼滤波算法。
	if (fabs(kalman_adc_old - ADC_Value) >= 5.0f)
	{
		//		Q = 10;
		//		R = 0.5;
		LastData = ADC_Value * 0.2 + kalman_adc_old * 0.8;
		kalman_adc_old = LastData;
		return (int32_t)(LastData);
	}
	//	if (fabs(kalman_adc_old-ADC_Value)>=20.0f)
	//	{
	//		kalman_adc_old = ADC_Value ;
	//	}
	//	else
	//	{
	////		Q = 0.0001;
	////		R = 2.5;
	//		LastData = kalman_adc_old;
	//	}
	LastData = kalman_adc_old;
	P = P1 + Q;
	Kg = P / (P + R);
	kalman_adc = LastData + Kg * (NowData - kalman_adc_old);
	P1 = (1 - Kg) * P;
	P = P1;
	ADC_OLD_Value = ADC_Value;
	kalman_adc_old = kalman_adc;
	return (uint32_t)(kalman_adc);
}


//kalman_data_t filter_x; // 滤波器数据结构体
//// 初始化卡尔曼滤波器
//void kalman_init(kalman_data_t *filter, float initial_value)
//{
//    filter->x_k = initial_value;
//    filter->p_k = 1;
//    filter->k_k = 0;
//    filter->z_k = initial_value;
//}
//
//// 更新卡尔曼滤波器状态
//float kalman_update(kalman_data_t *filter, uint32_t measurement)
//{
//    int i;
//    float x_k1k1, p_k1k1;
//    float innov, s_k, k_k;
//    float x_kk1 = filter->x_k;
//    float p_kk1 = filter->p_k;
//    float z_k = filter->z_k;
//    float q = Q_ESTIMATION;
//    float r = R_MEASUREMENT;
//
//    // 迭代更新状态向量和状态协方差
//    for (i = 0; i < MAX_ITERATIONS; i++) {
//        // 预测状态向量和卡尔曼增益
//        x_k1k1 = x_kk1;
//        p_k1k1 = p_kk1 + q;
//        k_k = p_k1k1 / (p_k1k1 + r);
//
//        // 更新状态向量和状态协方差
//        innov = z_k - x_k1k1;
//        x_kk1 = x_k1k1 + k_k * innov;
//        p_kk1 = (1 - k_k) * p_k1k1;
//
//        // 判断收敛达到精度
//        if (innov * innov < 1e-7) {
//            break;
//        }
//    }
//
//    // 更新滤波器状态
//    filter->x_k = x_kk1;
//    filter->p_k = p_kk1;
//    filter->k_k = k_k;
//    filter->z_k = measurement;
//
//    return x_kk1;
//}

//
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

 
 
