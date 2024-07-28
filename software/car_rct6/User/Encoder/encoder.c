#include "encoder.h"
#include <stdio.h>
#include "uart_1.h"
//蓝轱辘轮子 一圈2040编码器计数，记直径为6.8CM 位置环设CM为实际目标，（目标/6.8*3.14）*2040 为传入的位置环目标 定时器选择四倍频
extern int32_t car_speed_1 ; // 电机1速度
extern int32_t car_speed_2 ; // 电机2速度
static int32_t car_speed_sum_A = 0; 
static int32_t car_speed_sum_B = 0;//总转速
extern int32_t Target_Speed;
extern int32_t Target_Location;
int32_t car_location_1 = 0;
int32_t car_location_2 = 0 ;
extern float Target_Speed_A ; 
extern float Target_Speed_B; 
extern int stop_flag ;
extern int weight_sum ;
void ENCODER_INIT(void)
{
	HAL_TIM_Encoder_Start(&htim1,TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim8,TIM_CHANNEL_ALL);
    __HAL_TIM_SET_COUNTER(&htim1, 0);
    __HAL_TIM_SET_COUNTER(&htim8, 0);
}


//编码器计数初始化
void Encoder_Count_Init(void)
{
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL);
}

// 编码器数据读取
int16_t Encoder_Value(uint8_t n)
{
    if (n == 1)
    {
        return (int16_t)(__HAL_TIM_GET_COUNTER(&htim1));
    }
    else if (n == 2)
    {
        return (int16_t)(__HAL_TIM_GET_COUNTER(&htim8));
    }
    else
    {
        return 0;
    }
}

//编码器清零
void Encoder_Count_Clear(uint8_t n)//看看编码器连的啥，给忘了
{
    if (n == 1)
    {
        __HAL_TIM_SET_COUNTER(&htim1, 0);
    }
    else if (n == 2)
    {
        __HAL_TIM_SET_COUNTER(&htim8, 0);
    }
}

void encoder_speed(void)//car_speed_1
{
	
//		// 清零编码器计数值
//		Encoder_Count_Clear(1);
//		Encoder_Count_Clear(2);
//	
	
	
// 读取编码器速度
    car_speed_1 = 5*Encoder_Value(1);
    car_speed_2 = -5*Encoder_Value(2);
//		if(stop_flag >=1&&stop_flag<=3)
//		{car_speed_sum_A += car_speed_1;
//		car_speed_sum_B += car_speed_2;
//	
//		car_location_1 = car_speed_sum_A;
//		car_location_2 = car_speed_sum_B;
//	
//		} //右轮转的总圈数
//		else
//		{
//			car_location_1 = 0;
//			car_location_2 = 0;
//		}
//		if(car_location_1>500&&car_location_1<5500)
//		{DEBUG_printf("APP","%d,%d,%d",car_location_1,car_location_2,weight_sum );}
//		
		car_speed_sum_A += car_speed_1;
		car_speed_sum_B += car_speed_2;
	
		car_location_1 = car_speed_sum_A;
		car_location_2 = car_speed_sum_B;
//    // 清零编码器计数值
		Encoder_Count_Clear(1);
		Encoder_Count_Clear(2);
//		DEBUG_printf("ENCODER","%d,%d,%d",car_speed_1,car_speed_2,32);

}





