#include "motor.h"
#include "oled.h"

// #include "ti_msp_dl_config.h"
#define xiaotiao 1.2
#define datiao 2

int turntime = 0;
int turnsign = 0;
float turnspeed = 0;
int walktime = 0;
int walksign = 0;
float walkspeed = 0;
float Speedleft;
float Speedright;

extern int hcl0;
extern int hcl1;
extern int hcl2;

void Motor_Set(int MotorL,
               int MotorR) // 电机正反转   //左右与小车前进时的车轮左右相反
{
  // 左轮
  if (MotorL > 0) {
    DL_GPIO_clearPins(GPIOB, GPIO_MOTOR_RIN1_PIN);
  } else if (MotorL == 0) {
    DL_GPIO_setPins(GPIOB, GPIO_MOTOR_RIN1_PIN);
  } else {
    DL_GPIO_setPins(GPIOB, GPIO_MOTOR_RIN1_PIN);
  }
  // 右轮
  if (MotorR > 0) {
    DL_GPIO_setPins(GPIOB, GPIO_MOTOR_LIN1_PIN);
  } else if (MotorR == 0) {
    DL_GPIO_setPins(GPIOB, GPIO_MOTOR_LIN1_PIN);
  } else {
    DL_GPIO_clearPins(GPIOB, GPIO_MOTOR_LIN1_PIN);
  }

  if (MotorL < 0) {
    if (MotorL < -3200)
      MotorL = -3200;
    DL_TimerA_setCaptureCompareValue(MOTOR_PWM_INST, -MotorL,
                                     DL_TIMER_CC_1_INDEX);
  } else {
    if (MotorL > 3200)
      MotorL = 3200;
    DL_TimerA_setCaptureCompareValue(MOTOR_PWM_INST, MotorL,
                                     DL_TIMER_CC_1_INDEX);
  }

  if (MotorR < 0) {
    if (MotorR < -3200)
      MotorR = -3200;
    DL_TimerA_setCaptureCompareValue(MOTOR_PWM_INST, -MotorR,
                                     DL_TIMER_CC_0_INDEX);
  } else {
    if (MotorR > 3200)
      MotorR = 3200;
    DL_TimerA_setCaptureCompareValue(MOTOR_PWM_INST, MotorR,
                                     DL_TIMER_CC_0_INDEX);
  }
}

void carturn(
    int jiaodu) // 閸樼喎婀存潪顒�鎮滈崙鑺ユ殶閿涘矂娓剁憰浣峰▏閻⑩暟urnsign濠碉拷濞诧拷
{
  if (turnsign == 1) {
    if (jiaodu > 0) {
      turnspeed = 5;
    }
    if (jiaodu < 0) {
      jiaodu = -jiaodu;
      turnspeed = -5;
    }
    turntime = jiaodu;
    turnsign = 0;
  }
  if (turntime != 0) {
    if (turntime > 0) {

      turntime--;
      Motor_Set(-turnspeed, turnspeed);
    }
    if (turntime == 0) {
      Motor_Set(0, 0);
    }
  }
}

void carwalk(
    int juli) // 閸ュ搫鐣鹃崥鎴濆閹存牕鎮滈崥搴ゎ攽妞规湹绔村▓浣冪獩缁傝鍤遍弫甯礉闂囷拷鐟曚椒濞囬悽鈺砤lksign濠碉拷濞诧拷
{
  if (walksign == 1) {
    if (juli > 0) {
      walkspeed = 5;
    }
    if (juli < 0) {
      juli = -juli;
      walkspeed = -5;
    }
    walktime = juli;
    walksign = 0;
  }
  if (walktime != 0) {
    if (walktime > 0) {

      walktime--;
      Motor_Set(walkspeed, walkspeed);
    }
    if (walktime == 0) {
      Motor_Set(0, 0);
    }
  }
}

// void carrun ( float Speedleft, float Speedright)
//{

//	if ( hd[0]+hd[1]+hd[2]+hd[3]+hd[4] == 1)
//	{
//		if (hd[0] ==1 ){Speedleft -= datiao ; Speedright += datiao ;}
//		if (hd[1] ==1 ){Speedleft -= xiaotiao ; Speedright += xiaotiao;}
//		if (hd[3] ==1 ){Speedleft += xiaotiao; Speedright -= xiaotiao;}
//		if (hd[4] ==1 ){Speedleft += datiao; Speedright -= datiao;}
//	}

//	if ( (hd[0]+hd[1]+hd[2]+hd[3]+hd[4] >= 3)&&(hcl0>=500))
//	{
//		 modenow =5;

//	}
//	if (
//((hd[0]==1&&hd[4]==1)||(hd[0]==1&&hd[3]==1)||(hd[1]==1&&hd[4]==1)||(hd[1]==1&&hd[3]==1))&&(hcl1>=350))
//	{
//		 modenow =5;
//	}
//	if ( ((hd[0]==1&&hd[4]==0)||(hd[0]==1&&hd[3]==0))&&
// hcl1>=300&&lastTime==0)
//	{
//		 modenow =3;hcl2=1;
//	}
//	motorPidSetSpeed(Speedleft,Speedright);
//}
