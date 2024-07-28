#include "IIC.h"
#include "main.h"

 
/**
  * @brief  ?????
  * @param  xus ????,??:0~233015
  * @retval ?
  */
void Delay_us(uint32_t xus)
{
	SysTick->LOAD = 72 * xus;				//????????
	SysTick->VAL = 0x00;					//???????
	SysTick->CTRL = 0x00000005;				//??????HCLK,?????
	while(!(SysTick->CTRL & 0x00010000));	//?????0
	SysTick->CTRL = 0x00000004;				//?????
}
 
/**
  * @brief  ?????
  * @param  xms ????,??:0~4294967295
  * @retval ?
  */
void Delay_ms(uint32_t xms)
{
	while(xms--)
	{
		Delay_us(1000);
	}
}
 
/**
  * @brief  ????
  * @param  xs ????,??:0~4294967295
  * @retval ?
  */
void Delay_s(uint32_t xs)
{
	while(xs--)
	{
		Delay_ms(1000);
	}
} 
 
 
void MyI2C_W_SCL(uint8_t BitValue)
{
//	GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,BitValue);
	Delay_us(10);
}
 
void MyI2C_W_SDA(uint8_t BitValue)
{
//	GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,BitValue);
	Delay_us(10);
}
 
uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
//	BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
	BitValue = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
	Delay_us(10);
	return BitValue;
}
 
//void MyI2C_Init(void)
//{
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	
//	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
//}
 
void MyI2C_Start(void)
{
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}
 
void MyI2C_Stop(void)
{
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}
 
void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SDA(Byte & (0x80 >> i));
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
	}
}
 
uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t i, Byte = 0x00;
	MyI2C_W_SDA(1);
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SCL(1);
		if (MyI2C_R_SDA() == 1){Byte |= (0x80 >> i);}
		MyI2C_W_SCL(0);
	}
	return Byte;
}
 
void MyI2C_SendAck(uint8_t AckBit)
{
	MyI2C_W_SDA(AckBit);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
}
 
uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t AckBit;
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	return AckBit;
}
