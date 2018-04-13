/*
    Port P为输出端口 置0表示定义为输入端口 置1表示定义为输出端口
    事先将P0 P1端口全置1 表示定义为输出端口
    选用P端口0通道与1通道同直流电机IN1与IN2相连 控制电机的正反转
*/
#include "derivative.h"
#include "motor.h"

/* 
    PID控制变量集
*/
// 定义PID结构体
typedef struct PID
{
    int SetPoint;    // 设定目标值
    
    // PID参数
    double Proportion;    // 比例常数 Proportional Const
    double Integral;    // 积分常数 Integral Const
    double Derivative;    // 微分常数 Derivative Const
    
    // 误差
    int LastError;    // Error[-1]
    int PrevError;    // Error[-2]
} PID;

static PID motorPID;
static PID *sptr = &motorPID;

#define P_DATA 10
#define I_DATA 0
#define D_DATA 0
#define HAVE_NEW_VELOCITY 0x01

unsigned char CurrentVelocity = 0;    // 小车当前速度
unsigned char vTmpPIT = 0;    // PIT中间变量
unsigned char flag = 0;    // 标志位

/*
    函数名称:PID_Init(void)
    功能描述:PID初始化函数
*/
void PID_Set(void)
{
    sptr->LastError = 0;    // Error[-1]
    sptr->PrevError = 0;    // Error[-2]
    sptr->Proportion = P_DATA;    // 比例常数P
    sptr->Integral = I_DATA;    // 积分常数I
    sptr->Derivative = D_DATA;    // 微分常数D
    sptr->SetPoint = 40;    // 默认目标为40 即速度v=40*4.2=168cm/s
}


int IncPIDCalc(int NextPoint) {
    int iError, iIncPid;    // 当前误差
    iError = sptr->SetPoint - NextPoint;    // 增量计算
    iIncPid = sptr->Proportion * iError - sptr->Integral * sptr->LastError + sptr->Derivative * sptr->PrevError;    // E[k-2]项
    sptr->PrevError = sptr->LastError;    // 存储误差 用于下次计算
    sptr->LastError = iError;    // 存储误差 用于下次计算
    return iIncPid;
}

void Motor_Speed_Change(void)
{
    if(flag & HAVE_NEW_VELOCITY){
        PWMDTY0 += IncPIDCalc(CurrentVelocity);
        flag &=~ HAVE_NEW_VELOCITY;
    }
}

/*
    函数名称:Motor_Brake(void)
    说明:电机停止
*/
void Motor_Brake(void)
{
    // 端口输出低电平 停止驱动电机 考虑这部分要不要删除
    PTP_PTP0 = 0;
    PTP_PTP1 = 0;
    
    // 占空比为0
    PWME_PWME0 = 0;
    PWMDTY0 = 0x00;
    PWMPER0 = 0x00;

    // 电机停止后要重新启动需要重新调用函数IO_Init()
}

/*
    函数名称:Motor_Forward_Fast(void)
    说明:电机正转加速
*/
void Motor_Forward_Fast(void)
{
    // 占空比为58%
    PWME_PWME0 = 0;    
    PWMDTY0 = 0x07;
    PWMPER0 = 0x0C;
    PWME_PWME0 = 1;  
}

/*
    函数名称:Motor_Forward_Slow(void)
    说明:电机正转减速
*/
void Motor_Forward_Slow(void)
{
    // 占空比为53%
    PWME_PWME0 = 0;    
    PWMDTY0 = 0x08;
    PWMPER0 = 0x0F;
    PWME_PWME0 = 1;
}

/*
    函数名称:Motor_Back_Fast(void)
    说明:电机反转加速
*/
void Motor_Back_Fast(void)
{
    PTP_PTP0 = 0;
    PTP_PTP1 = 1;

    // 占空比为58%
    PWME_PWME1 = 0;    
    PWMDTY1 = 0x07;
    PWMPER1 = 0x0C;
    PWME_PWME1 = 1;
}

/*
    函数名称:Motor_Back_Slow(void)
    说明:电机反转减速
*/
void Motor_Back_Slow(void)
{
    PTP_PTP0 = 0;
    PTP_PTP1 = 1;

    // 占空比为53%
    PWME_PWME1 = 0;    
    PWMDTY1 = 0x08;
    PWMPER1 = 0x0F;
    PWME_PWME1 = 1;
}