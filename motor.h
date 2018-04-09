/*
    Port P为输出端口 需要事先0定义为输入端口 1定义为输出端口
    选用P端口0通道与1通道同直流电机IN1与IN2相连 控制电机的正反转
*/

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
    PWME_PWME0 = 1;

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