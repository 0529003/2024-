#include "main.h"
#include "eight.h"
#include "app.h"
extern float Target_Speed_A ; 
extern float Target_Speed_B;
uint8_t sensor[8];
uint8_t sensor_last[8];
extern float Target_Speed;
int weight_sum;
extern int32_t car_location_1 ;
extern int32_t car_location_2 ;
int choose_big = 0;
int choose_small = 0;
void read_eight(void)
{
	    // 记录当前的 sensor 数据
  for (int i = 0; i < 8; i++) {
        sensor_last[i] = sensor[i];
  }
	sensor[0] = HAL_GPIO_ReadPin(EIGHT_1_GPIO_Port, EIGHT_1_Pin);
	sensor[1] = HAL_GPIO_ReadPin(EIGHT_2_GPIO_Port, EIGHT_2_Pin);
	sensor[2] = HAL_GPIO_ReadPin(EIGHT_3_GPIO_Port, EIGHT_3_Pin);
	sensor[3] = HAL_GPIO_ReadPin(EIGHT_4_GPIO_Port, EIGHT_4_Pin);
	sensor[4] = HAL_GPIO_ReadPin(EIGHT_5_GPIO_Port, EIGHT_5_Pin);
	sensor[5] = HAL_GPIO_ReadPin(EIGHT_6_GPIO_Port, EIGHT_6_Pin);
	sensor[6] = HAL_GPIO_ReadPin(EIGHT_7_GPIO_Port, EIGHT_7_Pin);
	sensor[7] = HAL_GPIO_ReadPin(EIGHT_8_GPIO_Port, EIGHT_8_Pin);
	// 检查当前 sensor 数据是否全为1
  uint8_t all_one = 1;
   for (int i = 0; i < 8; i++) {
       if (sensor[i] != 1) 
				{
          all_one = 0;
          break;
        }
			 if(sensor[0]==1&&sensor[1]==0&&sensor[2]==0&&sensor[3]==1&&sensor[4]==0&&sensor[5]==1&&sensor[6]==1&&sensor[7]==1)
			 {
				choose_big = 1;
			 }
			 else if(sensor[0]==1&&sensor[1]==1&&sensor[2]==0&&sensor[3]==0&&sensor[4]==0&&sensor[5]==1&&sensor[6]==1&&sensor[7]==1)
			 {
				choose_small = 2;
			 }
			 else
				{
					choose_big = 0;
					choose_small = 0;
					weight_sum = sensor[0]*(-7)+sensor[1]*(-5)+sensor[2]*(-3)+sensor[3]*(0)+sensor[4]*0+sensor[5]*3+sensor[6]*5+sensor[7]*7;}
				}
    // 如果当前数据全为1，将上次不是全为1的数据赋值给现在的 sensor
    if (all_one) {
        for (int i = 0; i < 8; i++) {
            sensor[i] = sensor_last[i];
        }
    }

}
void circle_choose(int choose)
{
	if(choose == 1) //外圈
	{
		weight_sum = -5;
	}
	if(choose == 0) //内圈
	{
		weight_sum = 9;
	}
	
}
