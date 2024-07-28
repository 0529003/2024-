#include "pid.h"
#include "main.h"
#include "uart_1.h"



int32_t car_speed_1=0; // 电机1速度
int32_t car_speed_2=0; // 电机2速度

extern int32_t car_speed_sum_A; 
extern int32_t car_speed_sum_B;
extern int weight_sum ;

extern int32_t car_location_1; 
extern int32_t car_location_2;

float PWM_Output_A = 0;
float PWM_Output_B = 0;

extern float pitch,roll,yaw; 		    //欧拉角

#define MOTOR_PWM_MAX 1000000
#define MOTOR_SPEED_MAX 100
#define GRAY_PWM_MAX 5510
#define Angle_PWM_MAX 1000000
//--------------------------速度环参考系---------------------
//---------------速度环(PID)----------------内环
float Target_Speed = 32; // 目标速度
float Target_Speed_A = 32; 
float Target_Speed_B =32; 
float Kp_Speed = 31;      // 速度环比例系数  29.5
float Ki_Speed = 0.55;        // 速度环积分系数  0.49
float Kd_Speed = 0;      // 速度环微分系数   0



//--------------------------位置环参考系---------------------
//---------------位置环(PID)----------------外环
int32_t Target_Location=0;
float Kp_Location =0;      // 位置环比例系数  
float Ki_Location = 0;        // 位置环积分系数   
float Kd_Location = 0;      // 位置环微分系数    
//--------------------------循迹环参考系---------------------
//---------------循迹环(PID)----------------外环
int32_t Target_Gray=0;
float Kp_Gray =33;      // 循迹环比例系数  47 33
float Ki_Gray = 0.0000;        // 循迹环积分系数   
float Kd_Gray = 0.1;      // 循迹环微分系数    

//--------------------------转向环参考系---------------------
//---------------转向环(PID)----------------外环
int32_t Target_Angle=0;
float Kp_Angle =0.4;      // 转向环比例系数  
float Ki_Angle = 0.00511;        // 转向环积分系数   0.0111
float Kd_Angle = 0;      // 转向环微分系数 



/**
 * @description: 电机A速度环
 * @param {int32_t} target_speed
 * @param {int32_t} fact_speed
 * @return {*}
 */
int32_t PID_Speed_A(float target_speed, float fact_speed)
{
    static int32_t Speed_PWM_Out = 0, Err_Speed = 0, L_Speed_Err = 0, L_L_Speed_Err = 0, Err_Speed_Sum = 0; // 速度环输出

    // 速度误差
    Err_Speed = target_speed - fact_speed; // 计算速度误差

    // 死区限制
    if (Err_Speed > -2 && Err_Speed < 2)  //2
    {
        Err_Speed = 0;
    }

    // PID计算(增量)
    Speed_PWM_Out =Speed_PWM_Out +  Kp_Speed * (Err_Speed - L_Speed_Err) + Ki_Speed * Err_Speed + Kd_Speed * (Err_Speed - 2 * L_Speed_Err + L_L_Speed_Err);
		
    // 限幅
   if (Speed_PWM_Out > MOTOR_PWM_MAX)
   {
       Speed_PWM_Out = MOTOR_PWM_MAX;
   }
   else if (Speed_PWM_Out < -MOTOR_PWM_MAX)
   {
       Speed_PWM_Out = -MOTOR_PWM_MAX;
   }
    // 保存误差
    L_Speed_Err = Err_Speed;     // 保存上一次的误差
    L_L_Speed_Err = L_Speed_Err; // 保存上上次的误差
    return Speed_PWM_Out;
}

/**
 * @description: 电机B速度环
 * @param {int32_t} target_speed
 * @param {int32_t} fact_speed
 * @return {*}
 */
int32_t PID_Speed_B(float target_speed, float fact_speed)
{
    static int32_t Speed_PWM_Out = 0, Err_Speed = 0, L_Speed_Err = 0, L_L_Speed_Err = 0; // 速度环输出
                                                                                         // 计算速度误差
    Err_Speed = target_speed - fact_speed;  
    
    // 消抖                                            
     if (Err_Speed > -2 && Err_Speed < 2)  //2
    {
        Err_Speed = 0;
    }

    // PID计算(增量)
    Speed_PWM_Out =Speed_PWM_Out +  Kp_Speed * (Err_Speed - L_Speed_Err) + Ki_Speed * Err_Speed + Kd_Speed * (Err_Speed - 2 * L_Speed_Err + L_L_Speed_Err);

    //限幅
    if (Speed_PWM_Out > MOTOR_PWM_MAX)
    {
        Speed_PWM_Out = MOTOR_PWM_MAX;
    }
    else if (Speed_PWM_Out < -MOTOR_PWM_MAX)
    {
        Speed_PWM_Out = -MOTOR_PWM_MAX;
    }
    // 保存误差
    L_Speed_Err = Err_Speed;     // 保存上一次的误差
    L_L_Speed_Err = L_Speed_Err; // 保存上上次的误差
    return Speed_PWM_Out;
}


/**
 * @description: 电机A位置环
 * @param {int32_t} target_Location
 * @param {int32_t} fact_Location
 * @return {*}
 */
int32_t PID_Location_A(int32_t target_Location, int32_t fact_Location)
{
    static int32_t Location_PWM_Out = 0, Err_Location = 0, L_Location_Err = 0, L_L_Location_Err = 0, Err_Location_Sum = 0; // 位置环输出

    // 位置误差
    Err_Location = target_Location - fact_Location; // 计算位置误差

    // 死区限制
    if (Err_Location > -3 && Err_Location < 3)  //2
    {
        Err_Location = 0;
    }

    // PID计算(增量)
    Location_PWM_Out = Location_PWM_Out + Kp_Location * (Err_Location - L_Location_Err) + Ki_Location* Err_Location + Kd_Location * (Err_Location - 2 * L_Location_Err + L_L_Location_Err);   
		

    // 限幅
   if (Location_PWM_Out > MOTOR_SPEED_MAX)
   {
       Location_PWM_Out = MOTOR_SPEED_MAX;
   }
   else if (Location_PWM_Out < -MOTOR_SPEED_MAX)
   {
       Location_PWM_Out = -MOTOR_SPEED_MAX;
   }
    // 保存误差
    L_Location_Err = Err_Location;     // 保存上一次的误差
    L_L_Location_Err = L_Location_Err; // 保存上上次的误差
    return Location_PWM_Out;
}

/**
 * @description: 电机B位置环
 * @param {int32_t} target_speed
 * @param {int32_t} fact_speed
 * @return {*}
 */
int32_t PID_Location_B(int32_t target_Location, int32_t fact_Location)
{
    static int32_t Location_PWM_Out = 0, Err_Location = 0, L_Location_Err = 0, L_L_Location_Err = 0, Err_Location_Sum = 0; // 位置环输出

    // 位置误差
    Err_Location = target_Location - fact_Location; // 计算位置误差

    // 死区限制
    if (Err_Location > -3 && Err_Location < 3)  //2
    {
        Err_Location = 0;
    }

    // PID计算(增量)
    Location_PWM_Out = Location_PWM_Out + Kp_Location * (Err_Location - L_Location_Err) + Ki_Location* Err_Location + Kd_Location * (Err_Location - 2 * L_Location_Err + L_L_Location_Err);   
		

    // 限幅
   if (Location_PWM_Out > MOTOR_SPEED_MAX)
   {
       Location_PWM_Out = MOTOR_SPEED_MAX;
   }
   else if (Location_PWM_Out < -MOTOR_SPEED_MAX)
   {
       Location_PWM_Out = -MOTOR_SPEED_MAX;
   }
    // 保存误差
    L_Location_Err = Err_Location;     // 保存上一次的误差
    L_L_Location_Err = L_Location_Err; // 保存上上次的误差
    return Location_PWM_Out;
}


/**
 * @description: 循迹环
 * @param {int32_t} target_Gray
 * @param {int32_t} fact_Gray
 * @return {*}
 */
int32_t PID_Gray(int32_t fact_Gray)
{
    static int32_t Gray_PWM_Out = 0, Err_Gray = 0, L_Gray_Err = 0, L_L_Gray_Err = 0, Err_Gray_Sum = 0; // 位置环输出

    // 位置误差
    Err_Gray = 0 - fact_Gray; // 计算误差

    // 死区限制
    if (Err_Gray > -2 && Err_Gray < 2)  //2
    {
        Err_Gray = 0;
    }

    // PID计算(增量)
    Gray_PWM_Out = Gray_PWM_Out + Kp_Gray * (Err_Gray - L_Gray_Err) + Ki_Gray* Err_Gray + Kd_Gray * (Err_Gray - 2 * L_Gray_Err + L_L_Gray_Err);   
		

    // 限幅
   if (Gray_PWM_Out > GRAY_PWM_MAX)
   {
       Gray_PWM_Out = GRAY_PWM_MAX;
   }
   else if (Gray_PWM_Out < -GRAY_PWM_MAX)
   {
       Gray_PWM_Out = -GRAY_PWM_MAX;
   }
    // 保存误差
    L_Gray_Err = Err_Gray;     // 保存上一次的误差
    L_L_Gray_Err = L_Gray_Err; // 保存上上次的误差
    return Gray_PWM_Out;
}

/**
 * @description: 转向环
 * @param {int32_t} target_Angle
 * @param {int32_t} fact_Angle
 * @return {*}
 */
int32_t PID_Angle(int32_t target_Angle, int32_t fact_Angle)
{
    static int32_t Angle_PWM_Out = 0, Err_Angle = 0, L_Angle_Err = 0, L_L_Angle_Err = 0, Err_Angle_Sum = 0; // 位置环输出

    // 计算误差
    Err_Angle = target_Angle - fact_Angle; 

    // 死区限制
    if (Err_Angle > -2 && Err_Angle < 2)  //2
    {
        Err_Angle = 0;
    }

    // PID计算(增量)
    Angle_PWM_Out = Angle_PWM_Out + Kp_Angle * (Err_Angle - L_Angle_Err) + Ki_Angle* Err_Angle + Kd_Angle * (Err_Angle - 2 * L_Angle_Err + L_L_Angle_Err);   
		

    // 限幅
   if (Angle_PWM_Out > Angle_PWM_MAX)
   {
       Angle_PWM_Out = Angle_PWM_MAX;
   }
   else if (Angle_PWM_Out < -Angle_PWM_MAX)
   {
       Angle_PWM_Out = -Angle_PWM_MAX;
   }
    // 保存误差
    L_Angle_Err = Err_Angle;     // 保存上一次的误差
    L_L_Angle_Err = L_Angle_Err; // 保存上上次的误差
    return Angle_PWM_Out;
}





/**
 * @description: 小车PID总输出 位置环+速度环
 * @return {*}
 */
void Car_PID_Ctrl(void)
{
	
		//位置环
		int32_t PWM_Out_Location_A = PID_Location_A(Target_Location, car_location_1);
		int32_t PWM_Out_Location_B = PID_Location_B(Target_Location, car_location_2);
	
//	  DEBUG_printf("pid","PWM_Out_Location_A:%d ,PWM_Out_Location_B:%d",PWM_Out_Location_A,PWM_Out_Location_B);
	  
		
    // 速度环
		int32_t PWM_Out_Speed_A=PID_Speed_A(PWM_Out_Location_A, car_speed_1);
		int32_t PWM_Out_Speed_B=PID_Speed_B(PWM_Out_Location_B, car_speed_2);

    int32_t PWM_Output_A = PWM_Out_Speed_A;
    int32_t PWM_Output_B = PWM_Out_Speed_B;


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
    if (PWM_Output_A >= MOTOR_PWM_MAX)
    {
        PWM_Output_A = MOTOR_PWM_MAX;
    }
    else if (PWM_Output_A <= -MOTOR_PWM_MAX)
    {
        PWM_Output_A = -MOTOR_PWM_MAX;
    }
    if (PWM_Output_B >= MOTOR_PWM_MAX)
    {
        PWM_Output_B = MOTOR_PWM_MAX;
    }
    else if (PWM_Output_B <= -MOTOR_PWM_MAX)
    {
        PWM_Output_B = -MOTOR_PWM_MAX;
    }
    // 作用到电机
    motorset(PWM_Output_A, PWM_Output_B);
}






/**
 * @description: 小车PID总输出 速度环
 * @return {*}
 */
void Car_PID_Ctrl_Speed(void)
{
		
    // 速度环
    float PWM_Out_Speed_A = PID_Speed_A(Target_Speed, car_speed_1);
    float PWM_Out_Speed_B = PID_Speed_B(Target_Speed, car_speed_2);;
//		DEBUG_printf("pid","PWM_Out_Speed_A:%d ,PWM_Out_Speed_B:%d",PWM_Out_Speed_A,PWM_Out_Speed_B);


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
    if (PWM_Output_A >= MOTOR_PWM_MAX)
    {
        PWM_Output_A = MOTOR_PWM_MAX;
    }
    else if (PWM_Output_A <= -MOTOR_PWM_MAX)
    {
        PWM_Output_A = -MOTOR_PWM_MAX;
    }
    if (PWM_Output_B >= MOTOR_PWM_MAX)
    {
        PWM_Output_B = MOTOR_PWM_MAX;
    }
    else if (PWM_Output_B <= -MOTOR_PWM_MAX)
    {
        PWM_Output_B = -MOTOR_PWM_MAX;
    }
    // 作用到电机
    motorset(PWM_Output_A, PWM_Output_B);
}


/**
 * @description: 小车PID总输出 循迹环+速度环
 * @return {*}
 */
void Car_PID_GRAY(int target)
{
	
		//循迹环
		int32_t PWM_Out_Gray = PID_Gray(weight_sum)*(0.07);


    // 速度环
		int32_t PWM_Out_Speed_A=PID_Speed_A(target+PWM_Out_Gray, car_speed_1);
//		DEBUG_printf("xunji","gray:%d",PWM_Out_Gray);
		int32_t PWM_Out_Speed_B=PID_Speed_B(target-(PWM_Out_Gray), car_speed_2);

    int32_t PWM_Output_A = PWM_Out_Speed_A;
    int32_t PWM_Output_B = PWM_Out_Speed_B;


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
    if (PWM_Output_A >= MOTOR_PWM_MAX)
    {
        PWM_Output_A = MOTOR_PWM_MAX;
    }
    else if (PWM_Output_A <= -MOTOR_PWM_MAX)
    {
        PWM_Output_A = -MOTOR_PWM_MAX;
    }
    if (PWM_Output_B >= MOTOR_PWM_MAX)
    {
        PWM_Output_B = MOTOR_PWM_MAX;
    }
    else if (PWM_Output_B <= -MOTOR_PWM_MAX)
    {
        PWM_Output_B = -MOTOR_PWM_MAX;
    }
    // 作用到电机
    motorset(PWM_Output_A, PWM_Output_B);
}

/**
 * @description: 小车PID总输出 角度环+速度环
 * @return {*}
 */
void Car_PID_Angle(void)
{
	
		//角度环
		int32_t PWM_Out_Angle = PID_Angle(90,yaw);

//		DEBUG_printf("MPU","ANGLE:%d",PWM_Out_Angle);

    // 速度环
		int32_t PWM_Out_Speed_A=PID_Speed_A(-PWM_Out_Angle, car_speed_1);
		int32_t PWM_Out_Speed_B=PID_Speed_B((PWM_Out_Angle), car_speed_2);

    int32_t PWM_Output_A = PWM_Out_Speed_A;
    int32_t PWM_Output_B = PWM_Out_Speed_B;


    // 防止0速噪音
    if (PWM_Output_A<= 1 && PWM_Output_A>= -1)
    {
        PWM_Output_A = 0;
    }

    if (PWM_Output_B <= 1 && PWM_Output_B>= -1)
    {
        PWM_Output_B = 0;
    }

    // 限幅
    if (PWM_Output_A >= MOTOR_PWM_MAX)
    {
        PWM_Output_A = MOTOR_PWM_MAX;
    }
    else if (PWM_Output_A <= -MOTOR_PWM_MAX)
    {
        PWM_Output_A = -MOTOR_PWM_MAX;
    }
    if (PWM_Output_B >= MOTOR_PWM_MAX)
    {
        PWM_Output_B = MOTOR_PWM_MAX;
    }
    else if (PWM_Output_B <= -MOTOR_PWM_MAX)
    {
        PWM_Output_B = -MOTOR_PWM_MAX;
    }
    // 作用到电机
    motorset(PWM_Output_A, PWM_Output_B);
}

