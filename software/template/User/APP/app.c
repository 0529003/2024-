
#include "app.h"
// 题目选择标志位
extern uint8_t Problem_Flag = 0;
// [K210]实时位置
int x = 0;
int y = 0;
//--------------------------舵机参考系---------------------
// 中心位置处参数
extern uint16_t Centre_A = 1460;
extern uint16_t Centre_B = 1417;
// 舵机A（最值）   垂直
extern uint16_t Servo_MAX_A = 2000;
extern uint16_t Servo_MIN_A = 1000;
// 舵机B （最值）  水平
extern uint16_t Servo_MIN_B = 1000;
extern uint16_t Servo_MAX_B = 2000;

// 实时位置
extern uint16_t pwm_A;
extern uint16_t pwm_B;

// OLED屏幕缓冲区
uint8_t str_buff1[64] = {0};
uint8_t str_buff2[64] = {0};
uint8_t str_buff3[64] = {0};
uint8_t str_buff4[64] = {0};
uint8_t str_buff5[64] = {0};
uint8_t str_buff6[64] = {0};

void App_Init(void)
{
 
    UART_IT_Init(); // 总串口接收初始化
    HAL_TIM_Base_Start_IT(&htim2); // 启动定时器2
		OLED_Init();		// OLED初始化
		OLED_Clear();
		OLED_ShowString(4, 0, "Im YangBIin", 16);
	  PWM_Init();
//		ENCODER_INIT();
//		MOTOR_INIT();
    Yuntai_Control();              // 云台初始化
		DEBUG_printf("APP", "系统初始化完成~");
//		Buzzer_ShortBeep(); //测试蜂鸣器
	}


int App_Task(void)
{
	sprintf((char *)str_buff3, "problem_flag:%02d", Problem_Flag);
	// 刷新屏幕
	sprintf((char *)str_buff4, "x:%04d y:%04d", x, y);
	OLED_ShowString(0, 4, str_buff3, 8);
	OLED_ShowString(0, 5, str_buff4, 8);
//	motorset(-550,150);
	  // 参数限幅
    if (pwm_A > 1613)
        pwm_A = Servo_MAX_A;
    if (pwm_A < Servo_MIN_A)
        pwm_A = Servo_MIN_A;
    if (pwm_B > Servo_MAX_B)
        pwm_B = Servo_MAX_B;
    if (pwm_B < Servo_MIN_B)
        pwm_B = Servo_MIN_B;
	

}

// 定时器中断回调函数(1ms一次)
uint8_t LED_Heartbeat = 0; // LED心跳
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim2) // 判断中断是否来自于定时器2
    {
        // 心跳(10ms一次)
        LED_Heartbeat++;
        if (LED_Heartbeat == 10)
        {
            LED_Toggle();
					// 编码器计数
//						encoder_speed();
        }
    }

}
