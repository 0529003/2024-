#include "pid.h"
#include "motor.h"
#include "math.h"
int car_speed_1 = 0; // 电机1速度
int car_speed_2 = 0; // 电机2速度

extern int weight;

int car_location_1;
int car_location_2;

float PWM_Output_A = 0;
float PWM_Output_B = 0;
extern int left, right;
extern float pitch, roll, yaw; // 欧拉角
extern int timeout;
extern int progress ;
extern int black ;
#define MOTOR_PWM_MAX 60000 // 速度最大39
#define MOTOR_SPEED_MAX 100
#define GRAY_PWM_MAX 55100
#define Angle_PWM_MAX 100000
#define Angle_Location_MAX 40  //10
//--------------------------速度环参考系---------------------
//---------------速度环(PID)----------------内环
int Target_Speed = 0; // 目标速度
float Target_Speed_A = 0;
float Target_Speed_B = 0;
float Kp_Speed = 83; // 速度环比例系数    45   320  250
float Ki_Speed = 10; // 速度环积分系数    0.15   2.5  0.88
float Kd_Speed = 0;  // 速度环微分系数   0  1  0.7

float Kp_Speed_A = 140; // 速度环比例系数    10ms 150 3 0
float Ki_Speed_A = 3;   // 速度环积分系数
float Kd_Speed_A = 0;   // 速度环微分系数

float Kp_Speed_B = 140; // 速度环比例系数    10ms 150 3 0
float Ki_Speed_B = 3;   // 速度环积分系数   2.8
float Kd_Speed_B = 9;   // 速度环微分系数

//--------------------------位置环参考系---------------------
//---------------位置环(PID)----------------外环
int Target_Location = 0;
float Kp_Location = 0.01;      // 位置环比例系数
float Ki_Location = 0.0002330; // 位置环积分系数
float Kd_Location = 0.001;     // 位置环微分系数
//--------------------------循迹环参考系---------------------
//---------------循迹环(PID)----------------外环
int Target_Gray = 0;
float Kp_Gray = 3.03;   // 循迹环比例系数  2.5
float Ki_Gray = 0.0023; // 循迹环积分系数   0.0023
float Kd_Gray = 0.2;    // 循迹环微分系数    0.2

//--------------------------转向环参考系---------------------
//---------------转向环(PID)----------------外环
int Target_Angle = 0; // 100000
float Kp_Angle = 0.232; // 转向环比例系数                0.12      0.25 0.165
                        // 0.119   0.117     0.217      
float Ki_Angle = 0.001; // 转向环积分系数   0.0111   0.001    0.0005 0.0008
                        // 0.0008  0.0007    0.001
float Kd_Angle = 0.01; // 转向环微分系数                       0.0003 0.0003
                       // 0.0002      0.0002   0.0004

//--------------------------转向环参考系---------------------
//---------------转向环(PID)----------------外环

float Kp_Angle_0 = 0.235; // 转向环比例系数                0.12      0.25 0.165
                          // 0.119   0.227       0.232
float Ki_Angle_0 = 0.00; // 转向环积分系数   0.0111   0.001    0.0005 0.0008     0.0008  0.001
float Kd_Angle_0 = 0.01; // 转向环微分系数                       0.0003 0.0003     0.0002 0.012

//--------------------------位置式转向环参考系---------------------
//---------------转向环(PID)----------------外环

float Kp_Angle_Location = 0.4632; 
float Kd_Angle_Location =0.0;
//第三问斜线
// float Kp_Angle_Location = 0.4632; 
// float Kd_Angle_Location =0.0;


/**
 * @description: 电机A速度环
 * @param {int} target_speed
 * @param {int} fact_speed
 * @return {*}
 */
int PID_Speed_A(float target_speed, float fact_speed) {
  static int Speed_PWM_Out = 0, Err_Speed = 0, L_Speed_Err = 0,
             L_L_Speed_Err = 0, Err_Speed_Sum = 0; // 速度环输出

  // 速度误差
  Err_Speed = target_speed - fact_speed; // 计算速度误差

  // 死区限制
  if (Err_Speed > -1 && Err_Speed < 1) // 2
  {
    Err_Speed = 0;
  }
  // printf("Err_Speed_a %d\n",Err_Speed);
  // PID计算(增量)
  Speed_PWM_Out = Speed_PWM_Out + Kp_Speed_A * (Err_Speed - L_Speed_Err) +
                  Ki_Speed_A * Err_Speed +
                  Kd_Speed_A * (Err_Speed - 2 * L_Speed_Err + L_L_Speed_Err);

  // 限幅
  if (Speed_PWM_Out > MOTOR_PWM_MAX) {
    Speed_PWM_Out = MOTOR_PWM_MAX;
  } else if (Speed_PWM_Out < -MOTOR_PWM_MAX) {
    Speed_PWM_Out = -MOTOR_PWM_MAX;
  }
  // 保存误差
  L_Speed_Err = Err_Speed;     // 保存上一次的误差
  L_L_Speed_Err = L_Speed_Err; // 保存上上次的误差
  // printf("Speed_PWM_Out_a:%d\n",Speed_PWM_Out);
  return Speed_PWM_Out;
}

/**
 * @description: 电机B速度环
 * @param {int} target_speed
 * @param {int} fact_speed
 * @return {*}
 */
int PID_Speed_B(float target_speed, float fact_speed) {
  static int Speed_PWM_Out = 0, Err_Speed = 0, L_Speed_Err = 0,
             L_L_Speed_Err = 0; // 速度环输出
                                // 计算速度误差
  Err_Speed = target_speed - fact_speed;

  // 消抖
  if (Err_Speed > -1 && Err_Speed < 1) // 2
  {
    Err_Speed = 0;
  }
  // printf("Err_Speed_b %d\n",Err_Speed);
  // PID计算(增量)
  Speed_PWM_Out = Speed_PWM_Out + Kp_Speed_B * (Err_Speed - L_Speed_Err) +
                  Ki_Speed_B * Err_Speed +
                  Kd_Speed_B * (Err_Speed - 2 * L_Speed_Err + L_L_Speed_Err);

  // 限幅
  if (Speed_PWM_Out > MOTOR_PWM_MAX) {
    Speed_PWM_Out = MOTOR_PWM_MAX;
  } else if (Speed_PWM_Out < -MOTOR_PWM_MAX) {
    Speed_PWM_Out = -MOTOR_PWM_MAX;
  }
  // 保存误差
  L_Speed_Err = Err_Speed;     // 保存上一次的误差
  L_L_Speed_Err = L_Speed_Err; // 保存上上次的误差
  // printf("Speed_PWM_Out_b:%d\n",Speed_PWM_Out);
  return Speed_PWM_Out;
}


/**
 * @description: 循迹环
 * @param {int} target_Gray
 * @param {int} fact_Gray
 * @return {*}
 */
int PID_Gray(int fact_Gray) {
  static int Gray_PWM_Out = 0, Err_Gray = 0, L_Gray_Err = 0, L_L_Gray_Err = 0,
             Err_Gray_Sum = 0; // 位置环输出

  // 位置误差
  Err_Gray = 0 - fact_Gray; // 计算误差

  // 死区限制
  if (Err_Gray > -2 && Err_Gray < 2) // 2
  {
    Err_Gray = 0;
  }

  // PID计算(增量)
  Gray_PWM_Out = Gray_PWM_Out + Kp_Gray * (Err_Gray - L_Gray_Err) +
                 Ki_Gray * Err_Gray +
                 Kd_Gray * (Err_Gray - 2 * L_Gray_Err + L_L_Gray_Err);

  // 限幅
  if (Gray_PWM_Out > GRAY_PWM_MAX) {
    Gray_PWM_Out = GRAY_PWM_MAX;
  } else if (Gray_PWM_Out < -GRAY_PWM_MAX) {
    Gray_PWM_Out = -GRAY_PWM_MAX;
  }
  // 保存误差
  L_Gray_Err = Err_Gray;     // 保存上一次的误差
  L_L_Gray_Err = L_Gray_Err; // 保存上上次的误差
  return Gray_PWM_Out;
}

/**
 * @description: 转向环
 * @param {int} target_Angle
 * @param {int} fact_Angle
 * @return {*}
 */
int PID_Angle(int target_Angle, int fact_Angle) {
  static int Angle_PWM_Out = 0, Err_Angle = 0, L_Angle_Err = 0,
             L_L_Angle_Err = 0, Err_Angle_Sum = 0; // 位置环输出

  // 计算误差
  Err_Angle = target_Angle - fact_Angle;

  // 死区限制
  if (Err_Angle > -5 && Err_Angle < 5) // 2
  {
    Err_Angle = 0;
  }

  // PID计算(增量)
  Angle_PWM_Out = Angle_PWM_Out + Kp_Angle * (Err_Angle - L_Angle_Err) +
                  Ki_Angle * Err_Angle +
                  Kd_Angle * (Err_Angle - 2 * L_Angle_Err + L_L_Angle_Err);
  // if(timeout % 500==0)
  // {
  //     Angle_PWM_Out = 0;
  // }

  // 限幅
  if (Angle_PWM_Out > Angle_PWM_MAX) {
    Angle_PWM_Out = Angle_PWM_MAX;
  } else if (Angle_PWM_Out < -Angle_PWM_MAX) {
    Angle_PWM_Out = -Angle_PWM_MAX;
  }
  // 保存误差
  L_Angle_Err = Err_Angle;     // 保存上一次的误差
  L_L_Angle_Err = L_Angle_Err; // 保存上上次的误差
  return Angle_PWM_Out;
}

int PID_Angle_single(int target_Angle, int fact_Angle) {
  static int Angle_PWM_Out = 0, Err_Angle = 0, L_Angle_Err = 0,
             L_L_Angle_Err = 0, Err_Angle_Sum = 0; // 位置环输出

  // 计算误差
  Err_Angle = target_Angle - fact_Angle;

  // 死区限制
  if (Err_Angle > -5 && Err_Angle < 5) // 2
  {
    Err_Angle = 0;
  }

  // PID计算(增量)
  Angle_PWM_Out = Angle_PWM_Out + Kp_Angle_0 * (Err_Angle - L_Angle_Err) +
                  Ki_Angle_0 * Err_Angle +
                  Kd_Angle_0 * (Err_Angle - 2 * L_Angle_Err + L_L_Angle_Err);
  // 	if(timeout % 500==0)
  // {
  //     Angle_PWM_Out = 0;
  // }

  // 限幅
  if (Angle_PWM_Out > Angle_PWM_MAX) {
    Angle_PWM_Out = Angle_PWM_MAX;
  } else if (Angle_PWM_Out < -Angle_PWM_MAX) {
    Angle_PWM_Out = -Angle_PWM_MAX;
  }
  // 保存误差
  L_Angle_Err = Err_Angle;     // 保存上一次的误差
  L_L_Angle_Err = L_Angle_Err; // 保存上上次的误差
  return Angle_PWM_Out;
}


/**
 * @description: 小车PID总输出 速度环
 * @return {*}
 */
void Car_PID_Ctrl_Speed(void) {

  // 速度环
  float PWM_Out_Speed_A = PID_Speed_A(Target_Speed, left);
  float PWM_Out_Speed_B = PID_Speed_B(Target_Speed, right);
  ;
  //		DEBUG_printf("pid","PWM_Out_Speed_A:%d
  //,PWM_Out_Speed_B:%d",PWM_Out_Speed_A,PWM_Out_Speed_B);

  float PWM_Output_A = PWM_Out_Speed_A;
  float PWM_Output_B = PWM_Out_Speed_B;

  // 防止0速噪音
  //    if (PWM_Output_A<= 170 && PWM_Output_A>= -170)
  //    {
  //        PWM_Output_A = 0;
  //    }
  //

  //    if (PWM_Output_B<= 170 && PWM_Output_B >= -170)
  //    {
  //        PWM_Output_B = 0;
  //    }

  // 限幅
  if (PWM_Output_A >= MOTOR_PWM_MAX) {
    PWM_Output_A = MOTOR_PWM_MAX;
  } else if (PWM_Output_A <= -MOTOR_PWM_MAX) {
    PWM_Output_A = -MOTOR_PWM_MAX;
  }
  if (PWM_Output_B >= MOTOR_PWM_MAX) {
    PWM_Output_B = MOTOR_PWM_MAX;
  } else if (PWM_Output_B <= -MOTOR_PWM_MAX) {
    PWM_Output_B = -MOTOR_PWM_MAX;
  }
  // 作用到电机
  Motor_Set(PWM_Output_A, PWM_Output_B);
}

/**
 * @description: 小车PID总输出 循迹环+速度环
 * @return {*}
 */
void Car_PID_GRAY(int target) {

  // 循迹环
  int PWM_Out_Gray = PID_Gray(weight) * (0.5);

  // 速度环
  int PWM_Out_Speed_A = PID_Speed_A(Target_Speed_A + PWM_Out_Gray, left);    // 18
  int PWM_Out_Speed_B = PID_Speed_B(Target_Speed_B - (PWM_Out_Gray), right); // 16

  int PWM_Output_A = PWM_Out_Speed_A;
  int PWM_Output_B = PWM_Out_Speed_B;

  // 防止0速噪音
  //    if (PWM_Output_A<= 1 && PWM_Output_A>= -1)
  //    {
  //        PWM_Output_A = 0;
  //    }

  //    if (PWM_Output_B <= 1 && PWM_Output_B>= -1)
  //    {
  //        PWM_Output_B = 0;
  //    }

  // 限幅
  if (PWM_Output_A >= MOTOR_PWM_MAX) {
    PWM_Output_A = MOTOR_PWM_MAX;
  } else if (PWM_Output_A <= -MOTOR_PWM_MAX) {
    PWM_Output_A = -MOTOR_PWM_MAX;
  }
  if (PWM_Output_B >= MOTOR_PWM_MAX) {
    PWM_Output_B = MOTOR_PWM_MAX;
  } else if (PWM_Output_B <= -MOTOR_PWM_MAX) {
    PWM_Output_B = -MOTOR_PWM_MAX;
  }
  // 作用到电机
  Motor_Set(PWM_Output_A, PWM_Output_B);
}

/**
 * @description: 小车PID总输出 角度环+速度环
 * @return {*}
 */
void Car_PID_Angle(int target_angle, float true_angle) {

  // 角度环
  int PWM_Out_Angle = PID_Angle(target_angle, true_angle);
  // printf("ANGLE:%d",PWM_Out_Angle);

  // 速度环
  // int PWM_Out_Speed_A=PID_Speed_A(-PWM_Out_Angle+Target_Speed, car_speed_1);
  // int PWM_Out_Speed_B=PID_Speed_B((PWM_Out_Angle+Target_Speed), car_speed_2);
  // int PWM_Out_Speed_A=PID_Speed_A(Target_Speed-PWM_Out_Angle, car_speed_1);
  int PWM_Out_Speed_A = PID_Speed_A(Target_Speed, car_speed_1);
  int PWM_Out_Speed_B =
      PID_Speed_B(Target_Speed + (PWM_Out_Angle), car_speed_2);

  int PWM_Output_A = PWM_Out_Speed_A;
  int PWM_Output_B = PWM_Out_Speed_B;

  // 防止0速噪音
  if (PWM_Output_A <= 1 && PWM_Output_A >= -1) {
    PWM_Output_A = 0;
  }

  if (PWM_Output_B <= 1 && PWM_Output_B >= -1) {
    PWM_Output_B = 0;
  }

  // 限幅
  if (PWM_Output_A >= Angle_PWM_MAX) {
    PWM_Output_A = Angle_PWM_MAX;
  } else if (PWM_Output_A <= -Angle_PWM_MAX) {
    PWM_Output_A = -Angle_PWM_MAX;
  }
  if (PWM_Output_B >= Angle_PWM_MAX) {
    PWM_Output_B = Angle_PWM_MAX;
  } else if (PWM_Output_B <= -Angle_PWM_MAX) {
    PWM_Output_B = -Angle_PWM_MAX;
  }
  // 作用到电机
  Motor_Set(PWM_Output_A, PWM_Output_B);
}

void Car_PID_Angle_single(int target_angle, float true_angle) {

  // 角度环
  int PWM_Out_Angle = PID_Angle_single(target_angle, true_angle);
  // printf("ANGLE:%d",PWM_Out_Angle);

  // 速度环
  // int PWM_Out_Speed_A=PID_Speed_A(-PWM_Out_Angle+Target_Speed, car_speed_1);
  // int PWM_Out_Speed_B=PID_Speed_B((PWM_Out_Angle+Target_Speed), car_speed_2);
  // int PWM_Out_Speed_A=PID_Speed_A(Target_Speed-PWM_Out_Angle, car_speed_1);
  int PWM_Out_Speed_A = PID_Speed_A(Target_Speed, car_speed_1);
  int PWM_Out_Speed_B = PID_Speed_B(Target_Speed + (PWM_Out_Angle), car_speed_2);

  int PWM_Output_A = PWM_Out_Speed_A;
  int PWM_Output_B = PWM_Out_Speed_B;

  // 防止0速噪音
  if (PWM_Output_A <= 1 && PWM_Output_A >= -1) {
    PWM_Output_A = 0;
  }

  if (PWM_Output_B <= 1 && PWM_Output_B >= -1) {
    PWM_Output_B = 0;
  }

  // 限幅
  if (PWM_Output_A >= Angle_PWM_MAX) {
    PWM_Output_A = Angle_PWM_MAX;
  } else if (PWM_Output_A <= -Angle_PWM_MAX) {
    PWM_Output_A = -Angle_PWM_MAX;
  }
  if (PWM_Output_B >= Angle_PWM_MAX) {
    PWM_Output_B = Angle_PWM_MAX;
  } else if (PWM_Output_B <= -Angle_PWM_MAX) {
    PWM_Output_B = -Angle_PWM_MAX;
  }
  // 作用到电机
  Motor_Set(PWM_Output_A, PWM_Output_B);
}

// 位置式pid
int PD_Angle(float target_angle, float true_angle) {
  static float error_current, error_last;
  float ek, ek1;
  int Angle_out;
  error_current = target_angle - true_angle;

  if(fabsf(target_angle - true_angle)<1)
  {
    error_current = 0;
  }

  ek = error_current;
  ek1 = error_current - error_last;
//   if((error_current <= 10)&(error_current>=(-10))){
//       Angle_out = 0.3 * ek +  1* ek1;
//   }
//   else if(fabs(error_current)>10&fabs(error_current)<=40)
//   {
//       Angle_out = 0.4 * ek + 0.3 * ek1;
//   }
//   else if (fabs(error_current)>40&fabs(error_current)<60) {
//     Angle_out = 0.2 * ek + 0.2 * ek1;
  
//   }
//   else {
//     Angle_out = 0.05 * ek + 0.1* ek1;
//   }
  Angle_out = 0.55 * ek + 0.55* ek1;        //0.55  0.5
//   if(progress == 0 & black == 1)
//   {
//     Angle_out = 0;
//   }
  error_last = error_current;



  if (Angle_out >= Angle_Location_MAX) // 限幅处理
    Angle_out = Angle_Location_MAX;
  else if (Angle_out <= -Angle_Location_MAX) // 限幅处理
    Angle_out = -Angle_Location_MAX;
  return (int)Angle_out;
}

void Car_PD_Angle(int target_angle, float true_angle) {

  // 角度环
  int PWM_Out_Angle = PD_Angle(target_angle, true_angle);


  // 速度环
  int PWM_Out_Speed_A = PID_Speed_A(Target_Speed  , car_speed_1);
  int PWM_Out_Speed_B = PID_Speed_B(Target_Speed + (PWM_Out_Angle), car_speed_2);

  int PWM_Output_A = PWM_Out_Speed_A;
  int PWM_Output_B = PWM_Out_Speed_B;

  // 防止0速噪音
  if (PWM_Output_A <= 1 && PWM_Output_A >= -1) {
    PWM_Output_A = 0;
  }

  if (PWM_Output_B <= 1 && PWM_Output_B >= -1) {
    PWM_Output_B = 0;
  }

  // 限幅
  if (PWM_Output_A >= Angle_PWM_MAX) {
    PWM_Output_A = Angle_PWM_MAX;
  } else if (PWM_Output_A <= -Angle_PWM_MAX) {
    PWM_Output_A = -Angle_PWM_MAX;
  }
  if (PWM_Output_B >= Angle_PWM_MAX) {
    PWM_Output_B = Angle_PWM_MAX;
  } else if (PWM_Output_B <= -Angle_PWM_MAX) {
    PWM_Output_B = -Angle_PWM_MAX;
  }
  // 作用到电机
  Motor_Set(PWM_Output_A, PWM_Output_B);
}


void Car_PID_Ctrl_Speed_single(void) {

  // 速度环
  float PWM_Out_Speed_A = PID_Speed_A(0, left);
  float PWM_Out_Speed_B = PID_Speed_B(10, right);
  ;
  //		DEBUG_printf("pid","PWM_Out_Speed_A:%d
  //,PWM_Out_Speed_B:%d",PWM_Out_Speed_A,PWM_Out_Speed_B);

  float PWM_Output_A = PWM_Out_Speed_A;
  float PWM_Output_B = PWM_Out_Speed_B;



  // 限幅
  if (PWM_Output_A >= MOTOR_PWM_MAX) {
    PWM_Output_A = MOTOR_PWM_MAX;
  } else if (PWM_Output_A <= -MOTOR_PWM_MAX) {
    PWM_Output_A = -MOTOR_PWM_MAX;
  }
  if (PWM_Output_B >= MOTOR_PWM_MAX) {
    PWM_Output_B = MOTOR_PWM_MAX;
  } else if (PWM_Output_B <= -MOTOR_PWM_MAX) {
    PWM_Output_B = -MOTOR_PWM_MAX;
  }
  // 作用到电机
  Motor_Set(PWM_Output_A, PWM_Output_B);
}