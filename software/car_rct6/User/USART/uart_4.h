#include "main.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#ifndef __UART_4_H
#define __UART_4_H

// 串口4接收中断初始化
void UART4_Init(void);
// 串口4接收完成回调函数
void UARTE4_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
// 串口4错误回调
void UART4_ErrorCallback(UART_HandleTypeDef *huart);
// 实现printf函数
void Usart4DmaPrintf(const char *format, ...);
#endif // !__UART_4_H
