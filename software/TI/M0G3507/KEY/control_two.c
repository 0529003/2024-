
#include "ti_msp_dl_config.h"
#include "control_two.h"

//先声明一下两个函数
void xunji();

//声明主程序中的蜂鸣器函数





extern bool xunji_0,xunji_1,xunji_2,xunji_3,xunji_4,xunji_5,xunji_6;
extern int weight;





//循迹函数
void xunji()
{          
                //八路循迹，识别到白线高电平
                //第一个传感器      最左边
                xunji_0=DL_GPIO_readPins(xunji_PIN_0_PORT,  xunji_PIN_0_PIN);
                //第二个
                xunji_1=DL_GPIO_readPins(xunji_PIN_1_PORT,  xunji_PIN_1_PIN);
                //第三个
                xunji_2=DL_GPIO_readPins(xunji_PIN_2_PORT,  xunji_PIN_2_PIN);
                //第四个
                xunji_3=DL_GPIO_readPins(xunji_PIN_3_PORT,  xunji_PIN_3_PIN);
                //第五个
                xunji_4=DL_GPIO_readPins(xunji_PIN_4_PORT,  xunji_PIN_4_PIN);
                //第六个
                xunji_5=DL_GPIO_readPins(xunji_PIN_5_PORT,  xunji_PIN_5_PIN);
                //第七个
                xunji_6=DL_GPIO_readPins(xunji_PIN_6_PORT,  xunji_PIN_6_PIN);

                weight=+5*xunji_0+3*xunji_1+1*xunji_2-0*xunji_3-1*xunji_4-3*xunji_5-5*xunji_6;

}


