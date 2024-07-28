#ifndef __PID_H_
#define __PID_H_
#include "main.h"
#include "motor.h"
#include "encoder.h"

int32_t PID_Speed_A(float target_speed, float fact_speed);
int32_t PID_Speed_B(float target_speed, float fact_speed);
int32_t PID_Location_A(int32_t target_Location, int32_t fact_Location);
int32_t PID_Location_B(int32_t target_Location, int32_t fact_Location);
void Car_PID_Ctrl(void);
void Car_PID_Ctrl_Speed(void);
void Car_PID_Ctrl_Speed_Circle(void);
void Car_PID_GRAY(int target);
int32_t PID_Angle(int32_t target_Angle, int32_t fact_Angle);
int32_t PID_Gray(int32_t fact_Gray);
void Car_PID_Angle(void);
#endif

