/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Delay.h"
#include "JY61P.h"
#include "bmp.h"
#include "control_two.h"
#include "key.h"
#include "math.h"
#include "motor.h"
#include "oled.h"
#include "pid.h"
#include "stdio.h"
#include "string.h"
#include "ti/driverlib/m0p/dl_core.h"
#include "ti_msp_dl_config.h"

// #include <cstdlib>
// #include <cstdio>

// 串口接收状态标识
#define WAIT_HEADER1 0
#define WAIT_HEADER2 1
#define RECEIVE_DATA 2
#define MAX_DATA_LENGTH 254 // 最大数据包长度

char OledString[30];
int y = 0;
int Problem_Flag = 0;
int timeout = 0;

float pitch = 0, roll = 0, yaw = 0; // 欧拉角

// 编码器后两个轮子
extern int car_speed_1;  // 电机1速度
extern int car_speed_2;  // 电机2速度
extern int Target_Speed; // 目标速度
extern uint8_t RxState;
extern uint8_t receivedData[9];
extern uint8_t dataIndex;
extern uint8_t RollL, RollH, PitchL, PitchH, YawL, YawH, VL, VH, SUM;
float target_angle = 0;
extern int Target_Location;
extern int car_location_1; // 转一圈大概550编码器计数 一圈14.1cm左右
                           // 110cm/14.1=7.8圈  7.8*550=4290
extern int car_location_2;
extern float Target_Speed_A ;
extern float Target_Speed_B ;

// float Kp_Gray ;      // 循迹环比例系数  2.5
// float Ki_Gray ;        // 循迹环积分系数
// float Kd_Gray ;      // 循迹环微分系数    0.1

extern float Kp_Angle; // 转向环比例系数                0.12      0.25 0.165
                       // 0.119   0.117
extern float Ki_Angle; // 转向环积分系数   0.0111   0.001    0.0005 0.0008
                       // 0.0008  0.0007
extern float Kd_Angle; // 转向环微分系数                       0.0003 0.0003
                       // 0.0002      0.0002

volatile uint32_t gpioA;

int problem = 0;
int left_speed = 0;
int right_speed = 0;
bool xunji_0, xunji_1, xunji_2, xunji_3, xunji_4, xunji_5, xunji_6;

int left, right;
int angle_flag = 0;
int weight = 0;
int start_flag = 0;
int turn_flag = 0;
int stright_flag = 0;
// int turn_count = 0;
int progress = 0;
int buzzer_flag = 0;
int flag_count = 0;
int buzzer_time_count = 0;
int stright_flag_single = 0;
int justencde = 0;
int huidu = 0;
int black = 0;
int wight = 0;
int speed_flag = 0;
int wait_flag = 0;
// 程序左右轮与实际前进左右轮相同
// car_speed_1为右轮，car_speed_2为左轮
void Problem1(void);
void Problem2(void);
void Problem3(void);
void Problem4(void);
void buzzer(void);
int main(void) {
  SYSCFG_DL_init();
  NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
  DL_TimerA_startCounter(TIMER_0_INST);
  DL_TimerA_startCounter(MOTOR_PWM_INST);
  NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
  NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
  NVIC_ClearPendingIRQ(UART_JY61P_INST_INT_IRQN);
  NVIC_EnableIRQ(UART_JY61P_INST_INT_IRQN);
  DL_TimerG_startCounter(MOTOR_PWM_INST); // 初始化电机
  NVIC_EnableIRQ(GPIO_Encoder_INT_IRQN);  // 开启编码器的中断

  OLED_Init();

  //   Motor_Set(2000, 1800);  //第一个参数是头前左轮

  while (1) {
    // if (justencde == 0) {
    //   target_angle = -40;
    //   Target_Speed = 0;
    //   angle_flag = 1;
    //   // stright_flag_single = 1;
    //   if ((yaw - target_angle) <= 4 & (yaw - target_angle) >= -4) {
    //     Target_Speed = 10;
    //       justencde = 1;
    //   }
    // }

    sprintf(OledString, "problem=%d ", Problem_Flag);
    OLED_ShowString(0, 0, (u8 *)OledString, 8);

    sprintf(OledString, "problem2=%d ", progress);
    OLED_ShowString(0, 1, (u8 *)OledString, 8);

    sprintf(OledString, "left_speed=%03d ", left);
    OLED_ShowString(0, 2, (u8 *)OledString, 8);

    sprintf(OledString, "right_speed=%03d ", right);
    OLED_ShowString(0, 3, (u8 *)OledString, 8);

    sprintf(OledString, "yaw=%f ", yaw);
    OLED_ShowString(0, 4, (u8 *)OledString, 8);

    sprintf(OledString, "time=%d ", flag_count);
    OLED_ShowString(0, 5, (u8 *)OledString, 8);

    sprintf(OledString, "%d %d %d %d %d %d %d", xunji_0, xunji_1, xunji_2,
            xunji_3, xunji_4, xunji_5, xunji_6);
    OLED_ShowString(0, 6, (u8 *)OledString, 8);

    KEY_Scan(1);
    // 处理问题
    switch (Problem_Flag) {
    case 1:
      Problem1();
      break;
    case 2:
      Problem2();
      break;
    case 3:
      Problem3();
      break;
    case 4:
      Problem4();
      break;
    default:
      break;
    }
    printf("%d,%d,%d\n", left, right, Target_Speed);
  }
}

void Problem1(void) {
  if (xunji_0 == 0 & xunji_1 == 0 & xunji_2 == 0 & xunji_3 == 0 & xunji_4 == 0 &
      xunji_5 == 0 & xunji_6 == 0) {

    Target_Speed = 15;
    angle_flag = 1;
    target_angle = 0;
    speed_flag = 1;
    turn_flag = 0;
  } else {
    Target_Speed = 0;
    buzzer_flag = 1;
    speed_flag = 0;
    turn_flag = 0;
    while (1) {
      Motor_Set(0, 0);
    }
    Problem_Flag = 0;
  }
}

void Problem2(void) {
  // 直行到弯道处
  Target_Speed_A =13;
  Target_Speed_B = 10;
  switch (progress) {
  case 0:
    speed_flag = 1;
    Target_Speed = 15;
    angle_flag = 1;
    target_angle = 0;
    if (black == 1) {
      speed_flag = 0;
      buzzer_flag = 1;
      angle_flag = 0;
      progress = 1;
    }
    break;
  case 1:
    speed_flag = 0;
    turn_flag = 1;
    if (wight == 1) {
      turn_flag = 0;
      buzzer_flag = 1;
      progress = 2;
    }
    break;
  case 2:
    speed_flag = 1;
    Target_Speed = 15;
    target_angle = 179;
    angle_flag = 1;

    // if(justencde == 0)
    // {
    //     target_angle = 180;
    //     angle_flag = 1;
    //     Target_Speed = 0;
    //     if((yaw - target_angle) <= 3 & (yaw - target_angle) >= -3)
    //     {
    //         angle_flag = 0;
    //         Target_Speed = 20;
    //         justencde = 1;
    //     }
    // }
    if (black == 1) {
      speed_flag = 0;
      buzzer_flag = 1;
      angle_flag = 0;
      progress = 3;
    }
    break;
  case 3:
    turn_flag = 1;
    speed_flag = 0;
    if (wight == 1) {
      Target_Speed = 0;
      turn_flag = 0;
      buzzer_flag = 1;
      while (1) {
        Motor_Set(0, 0);
      }
    }
    break;
  default:
    break;
  }
}

void Problem3(void) {

  if(progress == 1)
  {
  Target_Speed_A =18;
  Target_Speed_B = 22;
  }
  else if(progress == 3){
  Target_Speed_A =22;
  Target_Speed_B = 18;
  }
  switch (progress) {
  case 0:
  if(flag_count <= 4000)  //5600
   {
    speed_flag = 1;
    Target_Speed = 20;   //15
   }
   else{
    
    if(justencde == 0)
    {
        Target_Speed = 0;
        target_angle = -4;
        angle_flag = 1;
        if((yaw - target_angle) <= 3 & (yaw - target_angle) >= -3)
        {
            angle_flag = 0;
            speed_flag = 1;
            Target_Speed = 20;
            justencde = 1;
        }
        if(black == 1)
        {
            Target_Speed = 0;
            speed_flag = 1;
            buzzer_flag = 1;
            progress = 1;
        }
    }
   }
   if(black == 1)
   {
    Target_Speed = 0;
    speed_flag = 1;
    buzzer_flag = 1;
    progress = 1;
   }
    break;
  case 1:
    speed_flag = 0;
    turn_flag = 1;
    if (wight == 1) {
      turn_flag = 0;
      buzzer_flag = 1;
      justencde = 0;
      speed_flag = 1;
      Target_Speed = 0;
      progress = 2;
    }
    break;
  case 2:
    if (justencde == 0) {
      speed_flag = 1;
      angle_flag = 1;
      target_angle = 230;
      Target_Speed = 0;
      if ((yaw - target_angle) <= 3 & (yaw - target_angle) >= -3) {
        angle_flag = 0;
        // Target_Speed = 18;
        // speed_flag = 1;
        justencde = 1;
      }
    }
    if (justencde == 1) {
      if (flag_count <= 4200) {
        Target_Speed = 20;
        speed_flag = 1;
      } else {
          target_angle = 179;
          angle_flag = 1;
          Target_Speed = 0;
          speed_flag = 1;
          if ((yaw - target_angle) <= 3 & (yaw - target_angle) >= -3) {
            Target_Speed = 18;
            speed_flag = 1;
            justencde = 2;
          }
         if (black == 1) {
            Target_Speed = 0;
            angle_flag =0;
            buzzer_flag = 1;
            speed_flag = 0;
            justencde = 0;
            progress = 3;
            }
      }
    }

    if (black == 1) {
      Target_Speed = 0;
      angle_flag =0;
      buzzer_flag = 1;
      speed_flag = 0;
      justencde = 0;
      progress = 3;
    }
    break;
  case 3:
    turn_flag = 1;
    speed_flag = 0;
    if (wight == 1) {
      Target_Speed = 0;
      turn_flag = 0;
      buzzer_flag = 1;
      while (1) {
        Motor_Set(0, 0);
      }
    }
    break;
  default:
    break;
  }
}

int first_time = 0;
void Problem4(void) {
  if(progress == 1)
  {
  Target_Speed_A =18;
  Target_Speed_B = 22;
  }
  else if(progress == 3){
  Target_Speed_A =22;
  Target_Speed_B = 18;
  }
  switch (progress) {
  case 0:
  if(flag_count <= 4000)  //5600
   {
    speed_flag = 1;
    Target_Speed = 20;   //15
   }
   else{
    
    if(justencde == 0)
    {
        Target_Speed = 0;
        target_angle = -4;
        angle_flag = 1;
        if((yaw - target_angle) <= 3 & (yaw - target_angle) >= -3)
        {
            angle_flag = 0;
            speed_flag = 1;
            Target_Speed = 20;
            justencde = 1;
        }
        if(black == 1)
        {
            Target_Speed = 0;
            speed_flag = 1;
            buzzer_flag = 1;
            progress = 1;
        }
    }
   }
   if(black == 1)
   {
    Target_Speed = 0;
    speed_flag = 1;
    buzzer_flag = 1;
    progress = 1;
   }
    break;
  case 1:
    speed_flag = 0;
    turn_flag = 1;
    if (wight == 1) {
      turn_flag = 0;
      buzzer_flag = 1;
      justencde = 0;
      speed_flag = 1;
      Target_Speed = 0;
      progress = 2;
    }
    break;
  case 2:
    if (justencde == 0) {
      speed_flag = 1;
      angle_flag = 1;
      target_angle = 230;
      Target_Speed = 0;
      if ((yaw - target_angle) <= 3 & (yaw - target_angle) >= -3) {
        angle_flag = 0;
        justencde = 1;
      }
    }
    if (justencde == 1) {
      if (flag_count <= 4200) {
        Target_Speed = 20;
        speed_flag = 1;
      } else {
          target_angle = 179;
          angle_flag = 1;
          Target_Speed = 0;
          if ((yaw - target_angle) <= 3 & (yaw - target_angle) >= -3) {
            Target_Speed = 18;
            justencde = 2;
          }
      }
    }

    if (black == 1) {
      Target_Speed = 0;
      angle_flag =0;
      buzzer_flag = 1;
      speed_flag = 0;
      justencde = 0;
      progress = 3;
    }
    break;
  case 3:
    turn_flag = 1;
    speed_flag = 0;
    if (wight == 1) {
      Target_Speed = 10;
      speed_flag = 1;
      turn_flag = 0;
      buzzer_flag = 1;
      Serial_JY61P_Zero_Yaw();
      delay_ms(500);
      first_time++;
      if(first_time >= 1&first_time<4)
      {
        
        justencde = 0;
        progress = 4;
      }
      else if (first_time == 4) {
        while (1) 
        {
            Motor_Set(0, 0);
        
        }
      }
    }
    break;
    case 4:
        if(justencde == 0)
        {
            speed_flag = 1;
            target_angle = -38;
            angle_flag = 1;
            Target_Speed = 0;
            if((yaw - target_angle) <= 3 & (yaw - target_angle) >= -3)
        {
            angle_flag = 0;
            progress = 0;
            
        }
        }
    break;
case 5:

     if (flag_count <= 4000)
     {
        speed_flag = 1;
       Target_Speed = 20;
     }
     else {
        speed_flag = 1;
        Target_Speed = 0;
        angle_flag = 1;
        target_angle = 0;
        if((yaw - target_angle) <= 3 & (yaw - target_angle) >= -3)
        {
            angle_flag = 0;
            progress = 1;
            
        }
     }
     break;
  default:
    break;
  }
}

void TIMER_0_INST_IRQHandler(void) // 1ms涓柇涓�娆�
{
  timeout++;
  if (timeout == 10000) {
    timeout = 0;
  }
  xunji();
  if (xunji_0 == 0 & xunji_1 == 0 & xunji_2 == 0 & xunji_3 == 0 & xunji_4 == 0 &
      xunji_5 == 0 & xunji_6 == 0) {
    huidu++;
    if (huidu >= 10) {
      wight = 1;
      black = 0;
    }
  } else {
    huidu = 0;
    wight = 0;
    black = 1;
  }
  if (timeout % 20 == 0) // 2
  {
    left = car_speed_1;
    right = car_speed_2;

    car_location_1 += car_speed_1;
    car_location_2 += car_speed_2;

    car_speed_1 = 0;
    car_speed_2 = 0;
  }
  if(wait_flag>0&wait_flag<100)
  {
    Car_PID_Ctrl_Speed_single();
  }
  if (speed_flag == 1) {
      Car_PID_Ctrl_Speed();
    }
  if (timeout % 5 == 0) {
    if (turn_flag == 1) {
      Car_PID_GRAY(Target_Speed);
    }

    if (angle_flag == 1) {
      Car_PD_Angle(target_angle, yaw);
    }
    
    }
  if (((progress == 0) | ((progress == 2) & (justencde == 1))) &
      (Problem_Flag == 3|Problem_Flag == 4)) {
    flag_count++; // 4560
    if (flag_count > 60000) {
      flag_count = 0;
    }
  } else {
    flag_count = 0;
  }
  if(progress == 2&Problem_Flag == 3)
  {
    wait_flag++;
  }
  else{wait_flag = 0;}
  if (buzzer_flag == 1) {
    buzzer_time_count++;
    DL_GPIO_setPins(GPIOB, GPIO_KEYS_sound_PIN);
    if (buzzer_time_count % 100 == 0) {
      DL_GPIO_clearPins(GPIOB, GPIO_KEYS_sound_PIN);
      buzzer_flag = 0;
    }
  }
}

void GROUP1_IRQHandler(void) // 编码器的中断函数
{
  gpioA = DL_GPIO_getEnabledInterruptStatus(
      GPIOA, GPIO_Encoder_PIN_E1_PIN | GPIO_Encoder_PIN_E2_PIN |
                 GPIO_Encoder_PIN_E3_PIN | GPIO_Encoder_PIN_E4_PIN);
  DL_GPIO_togglePins(GPIO_LED_PORT, GPIO_LED_LED_PIN);

  // if (gpioA & GPIO_Encoder_PIN_Back_Left_A_PIN) //左后轮     A相上升沿
  if (gpioA & GPIO_Encoder_PIN_E1_PIN) // 左后轮     A相上升沿
  {
    if (DL_GPIO_readPins(GPIO_Encoder_PORT,
                         GPIO_Encoder_PIN_E2_PIN)) // B相上升沿
      car_speed_2--;
    else
      car_speed_2++; // B相下降沿
    DL_GPIO_clearInterruptStatus(GPIO_Encoder_PORT, GPIO_Encoder_PIN_E1_PIN);
  }

  if (gpioA & GPIO_Encoder_PIN_E2_PIN) // 左后轮     A相上升沿
  {
    if (DL_GPIO_readPins(GPIO_Encoder_PORT,
                         GPIO_Encoder_PIN_E1_PIN)) // B相上升沿
      car_speed_2++;
    else
      car_speed_2--; // B相下降沿
    DL_GPIO_clearInterruptStatus(GPIO_Encoder_PORT, GPIO_Encoder_PIN_E2_PIN);
  }

  // if (gpioA & GPIO_Encoder_PIN_Back_Right_A_PIN)//右后轮     A相上升沿
  if (gpioA & GPIO_Encoder_PIN_E3_PIN) // 右后轮     A相上升沿
  {
    if (DL_GPIO_readPins(GPIO_Encoder_PORT,
                         GPIO_Encoder_PIN_E4_PIN)) // B相上升沿
      car_speed_1++;
    else // B相下降沿
      car_speed_1--;
    DL_GPIO_clearInterruptStatus(GPIO_Encoder_PORT, GPIO_Encoder_PIN_E3_PIN);
  }

  if (gpioA & GPIO_Encoder_PIN_E4_PIN) // 右后轮     A相上升沿
  {
    if (DL_GPIO_readPins(GPIO_Encoder_PORT,
                         GPIO_Encoder_PIN_E3_PIN)) // B相上升沿
      car_speed_1--;
    else // B相下降沿
      car_speed_1++;
    DL_GPIO_clearInterruptStatus(GPIO_Encoder_PORT, GPIO_Encoder_PIN_E4_PIN);
  }
}

void UART_0_INST_IRQHandler(void) {}
float yaw_first;

void UART_JY61P_INST_IRQHandler(void) {
  // 串口中断处理函数
  uint8_t uartdata =
      DL_UART_Main_receiveData(UART_JY61P_INST); // 接收一个uint8_t数据
  switch (RxState) {
  case WAIT_HEADER1:
    if (uartdata == 0x55) {
      RxState = WAIT_HEADER2;
    }
    break;
  case WAIT_HEADER2:
    if (uartdata == 0x53) {
      RxState = RECEIVE_DATA;
      dataIndex = 0;
    } else {
      RxState = WAIT_HEADER1; // 如果不是期望的第二个头，重置状态
    }
    break;
  case RECEIVE_DATA:
    receivedData[dataIndex++] = uartdata;
    if (dataIndex == 9) {
      // 数据接收完毕，分配给具体的变量
      RollL = receivedData[0];
      RollH = receivedData[1];
      PitchL = receivedData[2];
      PitchH = receivedData[3];
      YawL = receivedData[4];
      YawH = receivedData[5];
      VL = receivedData[6];
      VH = receivedData[7];
      SUM = receivedData[8];

      // 校验SUM是否正确
      uint8_t calculatedSum =
          0x55 + 0x53 + RollH + RollL + PitchH + PitchL + YawH + YawL + VH + VL;
      if (calculatedSum == SUM) {
        // 校验成功，可以进行后续处理
        if ((float)(((uint16_t)RollH << 8) | RollL) / 32768 * 180 > 180) {
          roll = (float)(((uint16_t)RollH << 8) | RollL) / 32768 * 180 - 360;
        } else {
          roll = (float)(((uint16_t)RollH << 8) | RollL) / 32768 * 180;
        }

        if ((float)(((uint16_t)PitchH << 8) | PitchL) / 32768 * 180 > 180) {
          pitch = (float)(((uint16_t)PitchH << 8) | PitchL) / 32768 * 180 - 360;
        } else {
          pitch = (float)(((uint16_t)PitchH << 8) | PitchL) / 32768 * 180;
        }

        if (progress == 1 | progress == 2) {
          yaw = (float)(((uint16_t)YawH << 8) | YawL) / 32768 * 180;
        } else {
          if ((float)(((uint16_t)YawH << 8) | YawL) / 32768 * 180 > 180) {
            yaw = (float)(((uint16_t)YawH << 8) | YawL) / 32768 * 180 - 360;
          } else {
            yaw = (float)(((uint16_t)YawH << 8) | YawL) / 32768 * 180;
          }
        }
      } else {
        // printf("erro\n");
        // 校验失败，处理错误
      }

      RxState = WAIT_HEADER1; // 重置状态以等待下一个数据包
    }
    break;
  }

  // DL_UART_Main_transmitData(UART_JY61P_INST, uartdata); //
  // 可选：回传接收到的数据
}

// 串口重定向
int fputc(int c, FILE *stream) {
  DL_UART_Main_transmitDataBlocking(UART_0_INST, c);
  return c;
}

int fputs(const char *restrict s, FILE *restrict stream) {
  uint16_t i, len;
  len = strlen(s);
  for (i = 0; i < len; i++) {
    DL_UART_Main_transmitDataBlocking(UART_0_INST, s[i]);
  }
  return len;
}

int puts(const char *_ptr) {
  int count = fputs(_ptr, stdout);
  count += fputs("\n", stdout);
  return count;
}

void buzzer(void) {
  DL_GPIO_setPins(GPIOB, GPIO_KEYS_sound_PIN);
  delay_ms(1000);
  DL_GPIO_clearPins(GPIOB, GPIO_KEYS_sound_PIN);
}