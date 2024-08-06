#include "key.h"
#include "oled.h"
#include "JY61P.h"
// #include <cstdio>



/*下面的方式是通过直接操作库函数方式读取IO*/
#define KEY0 DL_GPIO_readPins(GPIO_KEYS_PORT, GPIO_KEYS_KEY0_PIN) // PB18
#define KEY1 DL_GPIO_readPins(GPIO_KEYS_PORT, GPIO_KEYS_KEY1_PIN) // PB2
#define KEY2 DL_GPIO_readPins(GPIO_KEYS_PORT, GPIO_KEYS_KEY2_PIN) // PB3
#define KEY3 DL_GPIO_readPins(GPIO_KEYS_PORT, GPIO_KEYS_KEY3_PIN) // PB4
#define KEY4 DL_GPIO_readPins(GPIO_KEYS_PORT, GPIO_KEYS_KEY4_PIN) // PB5

extern int Problem_Flag ;
// 按键处理函数
// 返回按键值
// mode:0,不支持连续按;1,支持连续按;
// 0，没有任何按键按下
// 1，KEY0按下
// 2，KEY1按下
// 3，KEY2按下
// 4，WKUP按下 WK_UP
// 注意此函数有响应优先级,KEY0>KEY1>KEY2>WK_UP!!
int KEY_Scan(int mode) {
  static int key_up = 0; // 按键按松开标志
  if (mode)
    key_up = 1; // 支持连按
  if (key_up && (KEY1 == 0 || KEY2 == 0 || KEY3 == 0 || KEY4 == 0 )) {
    delay_ms(5); // 去抖动
    key_up = 0;
    if (KEY1 == 0) {
        Problem_Flag = 1;
      return 1;
      }
     else if (KEY2 == 0) {
        Problem_Flag = 2;
      return 2;
    } else if (KEY3 == 0) {
        Problem_Flag = 3;
      return 3;
    } else if (KEY4 == 0) {
        Problem_Flag = 4;
      return 4;
    }

   else if (KEY1 == 1 && KEY2 == 1 && KEY3 == 1 && KEY4 == 1 ) 
    {key_up = 1;}

    }
  return 0; // 无按键按下
}
