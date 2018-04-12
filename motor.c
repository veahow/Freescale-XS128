/*
    Port P为输出端口 置0表示定义为输入端口 置1表示定义为输出端口
    事先将P0 P1端口全置1 表示定义为输出端口
    选用P端口0通道与1通道同直流电机IN1与IN2相连 控制电机的正反转
*/

/*
    函数名称:IncPidCalc(int NextPoint)
    功能描述:增量式PID控制
*/
int IncPIDCalc(int NextPoint)
{
    int iError, iIncPid;    // 当前误差
    iError = sptr->SetPoint - NextPoint;    // 增量计算
    iIncPid = sptr->Proportion * iError    // E[k]项
                - sptr->Integral * sptr->LastError    // E[k-1]项
                + sptr->Derivative * sptr->PrevError;    // E[k-2]项
    sptr->PrevError = sptr->LastError;    // 存储误差 用于下次计算
    sptr->LastError = iError;    // 存储误差 用于下次计算

    return iIncPid;
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