
#include "app.h"
// 题目选择标志位
uint8_t Problem_Flag = 0;
// [K210]实时位置
int x = 0;
int y = 0;
//距离
int distance = 0;
extern int32_t car_speed_1 ; // 电机1速度
extern int32_t car_speed_2 ; // 电机2速度
extern float Target_Speed_A ; 
extern float Target_Speed_B; 
extern float Target_Speed;
extern int32_t car_location_1 ;
extern int32_t car_location_2 ;
// OLED屏幕缓冲区
uint8_t str_buff1[64] = {0};
uint8_t str_buff2[64] = {0};
uint8_t str_buff3[64] = {0};
uint8_t str_buff4[64] = {0};
uint8_t str_buff5[64] = {0};
uint8_t str_buff6[64] = {0};
extern uint8_t sensor[8];
extern char HC_05[64];
extern char TOF250[64];
int flag;
int stop_flag = 0;
extern int weight_sum ;
extern int choose_big ;
extern int choose_small ;
uint8_t MPU_flag=0;
// MPU6050 数据
float pitch,roll,yaw; 		    //欧拉角
short aacx,aacy,aacz;			//加速度传感器原始数据
short gyrox,gyroy,gyroz;		//陀螺仪原始数据
void App_Init(void)
{
 
    UART_IT_Init(); // 总串口接收初始化
    HAL_TIM_Base_Start_IT(&htim2); // 启动定时器2
		OLED_Init();		// OLED初始化
		OLED_Clear();
		OLED_ShowString(1, 0, "START", 8);
	  PWM_Init();
		ENCODER_INIT();
		MOTOR_INIT();
		MPU_Init();			//MPU6050初始化
    mpu_dmp_init();		//dmp初始化
		DEBUG_printf("APP", "系统初始化完成~");
	}

int App_Task(void)
{
	sprintf((char *)str_buff1, "problem:%02d", flag);
	// 将传感器数据格式化为字符串
  sprintf((char *)str_buff2, "%d %d %d %d %d %d %d %d",
  sensor[0], sensor[1], sensor[2], sensor[3],
	sensor[4],	sensor[5], sensor[6], sensor[7]);
	sprintf((char *)str_buff3, "distance:%04d",distance);
	sprintf((char *)str_buff4, "L:%04d R:%04d", car_speed_1, car_speed_2);
	sprintf((char *)str_buff5, "L:%05d R:%05d", car_location_1, car_location_2);
	sprintf((char *)str_buff6, "yaw:%.01f ", yaw);
	// 刷新屏幕
	OLED_ShowString(0, 2, str_buff1, 8);
	OLED_ShowString(0, 3, str_buff2, 8);
	OLED_ShowString(0, 4, str_buff3, 8);
	OLED_ShowString(0, 5, str_buff4, 8);
	OLED_ShowString(0, 6, str_buff5, 8);
	OLED_ShowString(0, 7, str_buff6, 8);
//	motorset(PID_Speed_A(Target_Speed_A,car_speed_1),PID_Speed_B(Target_Speed_B,car_speed_2));  //左右，
	if(1)
	{
		while(mpu_dmp_get_data(&pitch, &roll, &yaw));	//必须要用while等待，才能读取成功
		MPU_flag = 1;
	}
	MPU_Get_Accelerometer(&aacx,&aacy, &aacz);		//得到加速度传感器数据
	MPU_Get_Gyroscope(&gyrox, &gyroy, &gyroz);		//得到陀螺仪数据
//	DEBUG_printf("mpu","X:%.1f°  Y:%.1f°  Z:%.1f°  \r\n",roll,pitch,yaw);
	DEBUG_printf("mpu","%f,%f",yaw,90.0);
	static uint8_t last_Flag = 0;
	Problem1();
	// 处理问题
	switch (Problem_Flag)
	{
		case 0:
			last_Flag = Problem_Flag;
		case 1:
			if (last_Flag != Problem_Flag)
			{
				Problem1();
				last_Flag = Problem_Flag;
			}
			break;
		case 2:
			if (last_Flag != Problem_Flag)
			{
				Problem2();
				last_Flag = Problem_Flag;
			}
			break;
		case 3:
			if (last_Flag != Problem_Flag)
			{
				Problem3();
				last_Flag = Problem_Flag;
			}
			break;
		case 4:
			if (last_Flag != Problem_Flag)
			{
				Problem4();
				last_Flag = Problem_Flag;
			}
			break;
		default:
			break;
		}
		

}

void Problem1(void)
{
		if(sensor[0]==1&&sensor[1]==1&&sensor[2]==0&&sensor[3]==0&&sensor[4]==0&&sensor[5]==0&&sensor[6]==1&&sensor[7]==1)
	{
		HAL_Delay(30);
		stop_flag ++;
//		Buzzer_ShortBeep(); 
		if(stop_flag ==1)
		{
			circle_choose(choose_big);
//			Target_Speed = 0;
		}
	}
		if(stop_flag ==2)
		{
			circle_choose(choose_small);
//			Target_Speed = 0;
			stop_flag = 0;
		}
	
	

}
void Problem2(void)
{
}
void Problem3(void)
{
}
void Problem4(void)
{
}
// 定时器中断回调函数(1ms一次)
uint8_t LED_Heartbeat = 0; // LED心跳
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim2) // 判断中断是否来自于定时器2
    {

        // 心跳(10ms一次)
        LED_Heartbeat++;
        if (LED_Heartbeat == 5)
        {
          LED_Toggle();

        }
    }
		static uint16_t count = 0, time = 0;
		count++;
		if (count % 10 == 0)
	{
		// 编码器计数
		encoder_speed();
		
	}

//		Car_PID_Ctrl_Speed();
		Car_PID_GRAY(Target_Speed); //频率要快，太慢会摇摆
//	if(MPU_flag == 1)
//	{Car_PID_Angle();}
		//获取八路灰度数据
		read_eight();
		//解析蓝牙信号
//		sscanf(HC_05, "%03d", &flag);
	
		//解析距离信号
		sscanf(TOF250, "%d", &distance);
}
