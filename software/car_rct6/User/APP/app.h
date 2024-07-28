#ifndef __APP_H
#define __APP_H

#include "main.h"
#include "tim.h"
#include "led.h"
#include "key.h"
#include "oled.h"
#include "uart_it_config.h"
#include "uart_1.h"
#include "uart_2.h"
#include "pwm.h"
#include "yuntai.h"
#include "motor.h"
#include "encoder.h"
#include "Buzzer.h"
#include "eight.h"
#include "pid.h"
#include "IIC.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

void App_Init(void);
int App_Task(void);
void Problem1(void);
void Problem2(void);
void Problem3(void);
void Problem4(void);

#endif // !__APP_H
