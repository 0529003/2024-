#ifndef __MOTOR_H
#define __MOTOR_H

#include "ti_msp_dl_config.h"

void Motor_Set(int MotorL, int MotorR);
void carturn(int jiaodu);
void carwalk(int juli);
void carrun(float Speedleft, float Speedright);

#endif
