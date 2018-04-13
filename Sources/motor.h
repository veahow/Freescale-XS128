#ifndef _motor_H
#define _motor_H

void PID_Set(void);
int IncPIDCalc(int NextPoint);
void Motor_Speed_Change(void);
void Motor_Brake(void);
void Motor_Forward_Fast(void);
void Motor_Forward_Slow(void);
void Motor_Back_Fast(void);
void Motor_Back_Slow(void);

#endif // _motor_H