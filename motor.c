#include <hidef.h>
#include <MC9S12XS128.h>

void main(void)
{
    
}

/*
    A为输入端口 B为输出端口 0定义为输入端口 1定义为输出端口
    选用B端口0通道与1通道同直流电机IN1与IN2相连 控制电机正反转
*/
void Motor_Brake(void)
{
    for(;;)
    {
        PORTB_PB0 = 0;
        PORTB_PB1 = 0;
        PWMDTY1 = 0;
    }
}

/*
    函数名称:
    说明:电机正转加速
*/
void Motor_Forward_Fast(void)
{
    for(;;)
    {
        PORTB_PB0 = 1;    // 使能IN1
        PORTB_PB1 = 0;    // 禁止IN2
        PWME_PWME1 = 1;    // 使能ENA？？？ 不懂
        PWMDTY1 = 100;    // 设置占空比 可计算 具体见文档
    }
}

/*
    函数名称:
    说明:电机正转减速
*/
void Motor_Forward_Slow(void)
{
    for(;;)
    {
        PORTB_PB0 = 1;    // 使能IN1
        PORTB_PB1 = 0;    // 禁止IN2
        PWME_PWME1 = 1;    // 不懂
        PWMDTY1 = 30;    // 设置占空比 这里应该是设置的小一点 达到减速的效果
    }
}

/*
    函数名称:
    说明:电机反转加速
*/
void Motor_Back_Fast(void)
{
    for(;;)
    {
        PORTB_PB0 = 0;
        PORTB_PB1 = 1;
        PWME_PWME1 = 1;
        PWMDTY1 = 100;
    }
}

/*
    函数名称:
    说明:电机反转减速
*/
void Motor_Back_Slow(void)
{
    for(;;)
    {
        PORTB_PB0 = 0;
        PORTB_PB1 = 1;
        PWME_PWME1 = 1;
        PWMDTY1 = 30;
    }
}