/*
 * @Author: DuRuofu duruofu@qq.com
 * @Date: 2023-08-02 12-02-17
 * @LastEditors: DuRuofu
 * @LastEditTime: 2024-04-13 18-13-01
 * @FilePath: \STM32(simple)\User\KEY\key.c
 * @Description: 键盘扫描
 * Copyright (c) 2023 by duruofu@foxmail.com All Rights Reserved. 
 */

#include "key.h"
#include "led.h"
#include "yuntai.h"
//#include "Buzzer.h"
#include "app.h"


//#define KEY1_Pin KEY1_Pin
//#define KEY2_Pin KEY2_Pin




#define DEBOUNCE_DELAY 250 // 设置消抖延时为200毫秒



//GUI菜单标志位
//extern uint8_t GUI_Menu;

//题目标志位
//extern uint8_t Problem_Flag;

//校准确认标志位
//extern uint8_t Calibration_Flag;

//急停标志位
//extern uint8_t Stop_Flag;

// 初始位置  //舵机中值
extern uint16_t pwm_A ;
extern uint16_t pwm_B ;


/**
 * @description: 按键初始化 (使用CubeMX自动生成的宏定义，就不用写这个函数了)
 * @return {*}
 */
void Key_Init(void)
{

}



//题目切换按钮
void Key_1_Callback(void)
{
//    pwm_A = 1307;
//    pwm_B = 1565;
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
}



// 急停按键
void Key_2_Callback(void)
{
//    pwm_A = 1460;
//    pwm_B = 1417;
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
}

void Key_3_Callback(void)
{
//    pwm_A = 1307;
//    pwm_B = 1565;
	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
}

void Key_4_Callback(void)
{
//    pwm_A = 1307;
//    pwm_B = 1565;
	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
}
//
/**
 * @description: 按键检测，外部中断回调函数
 * @param {uint16_t} GPIO_Pin
 * @return {*}
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

    /* Prevent unused argument(s) compilation warning */
    UNUSED(GPIO_Pin);
    /* NOTE: This function Should not be modified, when the callback is needed,
            the HAL_GPIO_EXTI_Callback could be implemented in the user file
    */
    // 按键按下
    if(GPIO_Pin == KEY1_Pin)
    {
        //printf("按键1");
        Debounce(GPIO_Pin, Key_1_Callback);
       
    }
    else if(GPIO_Pin == KEY2_Pin)
    {

        // 按键2按下的处理代码
        Debounce(GPIO_Pin, Key_2_Callback);
    }
//		    else if(GPIO_Pin == KEY3_Pin)
//    {

//        // 按键2按下的处理代码
//        Debounce(GPIO_Pin, Key_3_Callback);
//    }
//		    else if(GPIO_Pin == KEY4_Pin)
//    {

//        // 按键2按下的处理代码
//        Debounce(GPIO_Pin, Key_4_Callback);
//    }
}




// 通用的按键消抖函数
void Debounce(uint16_t GPIO_Pin, void (*callback)(void))
{
    static uint32_t lastTriggerTime = 0;
    uint32_t currentTime = HAL_GetTick(); // 获取当前时间戳

    if (currentTime - lastTriggerTime >= DEBOUNCE_DELAY)
    {
//        Buzzer_ShortBeep();
        callback(); // 调用传入的回调函数
        lastTriggerTime = currentTime; // 更新上一次触发的时间戳
    }
}

