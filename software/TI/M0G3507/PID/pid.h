
#ifndef __PID_H_
#define __PID_H_
#include "ti_msp_dl_config.h"

int PID_Speed_A(float target_speed, float fact_speed);
int PID_Speed_B(float target_speed, float fact_speed);
void Car_PID_Ctrl_Speed(void);
void Car_PID_Ctrl_Speed_Circle(void);
void Car_PID_GRAY(int target);
int PID_Angle(int target_Angle, int fact_Angle);
int PID_Angle_single(int target_Angle, int fact_Angle);
int PID_Gray(int fact_Gray);
void Car_PID_Angle(int target_angle, float true_angle);
void Car_PID_Angle_single(int target_angle, float true_angle);
int PD_Angle(float target_angle, float true_angle);
void Car_PD_Angle(int target_angle, float true_angle);
void Car_PID_Ctrl_Speed_single(void) ;
#endif