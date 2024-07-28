/*
 * @Author: DuRuofu duruofu@qq.com
 * @Date: 2023-07-13 17-13-53
 * @LastEditors: DuRuofu
 * @LastEditTime: 2024-04-12 23-08-02
 * @FilePath: \CarFollowingSystem-00\Users\Components\usart\uart_2.c
 * @Description: 串口3逻辑
 * Copyright (c) 2023 by duruofu@foxmail.com All Rights Reserved.
 */
#include "uart_1.h"
#include "uart_4.h"
#include "led.h"

#define RXBUFFERSIZE_4 256 // 最大接收字节数

// 定义串口句柄,使用串口4
#define UART_HANDLE huart4
char TOF250[64];
// 定义数据缓冲区
uint8_t RxBuffer_4[RXBUFFERSIZE_4];
uint8_t Uart_RxBuffer_4;   // 接收中断缓冲
uint8_t Uart_Rx_Cnt_4 = 0; // 接收缓冲计数

// 发送缓冲区
uint8_t TxBuffer_4[RXBUFFERSIZE_4];

extern DMA_HandleTypeDef hdma_uart4_rx;
// 消息接收标志位
void UART4_Init(void)
{
  /*串口硬件配置代码(使用cudeMX则不需要此部分)
  Init the GPIO of USART4
  */
  // 使能 USART4 的接收中断
  __HAL_UART_ENABLE_IT(&UART_HANDLE, UART_IT_RXNE);
  // 开启 USART4 的连续接收中断，并指定接收缓冲区的地址和长度
  HAL_UARTEx_ReceiveToIdle_DMA(&UART_HANDLE, &Uart_RxBuffer_4, 1);
  // 关闭DMA的传输过半中断
  __HAL_DMA_DISABLE_IT(&hdma_uart4_rx, DMA_IT_HT);
}

// 串口4接收完成回调函数
void UARTE4_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
  UNUSED(Size);
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_UART_TxCpltCallback could be implemented in the user file
   */
  // 将数据发送回去
  HAL_UART_Transmit_DMA(&UART_HANDLE, RxBuffer_4, Size);

//  // 处理数据
  strcpy(TOF250, (const char *)RxBuffer_4);
  HAL_UARTEx_ReceiveToIdle_DMA(&UART_HANDLE, RxBuffer_4, RXBUFFERSIZE_4);
}

// 串口4错误回调函数(主要用来清除溢出中断)
void UART4_ErrorCallback(UART_HandleTypeDef *huart)
{
  if (HAL_UART_ERROR_ORE)
  {
    uint32_t temp = huart->Instance->SR;
    temp = huart->Instance->DR;
  }
}

// 实现printf函数
void Usart4DmaPrintf(const char *format, ...)
{
  uint16_t len;
  va_list args;
  va_start(args, format);
  len = vsnprintf((char *)TxBuffer_4, sizeof(TxBuffer_4) + 1, (char *)format, args);
  va_end(args);
  HAL_UART_Transmit_DMA(&UART_HANDLE, TxBuffer_4, len);
}
