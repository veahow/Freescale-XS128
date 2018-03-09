#include <hidef.h>
#include <MC9S12XS128.h>

void main(void)
{
    
}

/*
    函数名称:PWM_Init()
    功能描述:
    说明:
*/
void PWM_Init(void)
{
    PWME = 0;    // 关闭PWM所有通道
    PWMPRCLK = 0x77;    // PWMPRCLK寄存器单独给时钟源A与B进行预分频

    // 时钟SA SB分别通过PWMSCLA PWMSCLB寄存器的设置分频产生
    PWMSCLA = 10;    // SA = A/2/10=1000Hz
    PWMSCLB = 10;    // SB = B/2/10=1000Hz

    // PWMCLK 选择时钟源
    PWMCLK_PCLK1 = 1;    // PWM1通道选择时钟源SA
    PWMPOL_PPOL1 = 1;    // Duty = High Time 设置PWMPOL选择输出通道波形的翻转

    // 设置PWMCAE控制通道输出的对齐方式 只有输出通道关闭时才能对其设置 1为居中对齐 0为左对齐
    PWMCAE_CAE1 = 0;    // 控制PWM1通道为左对齐
    PWMCTL = 0x00;    // 级联控制 PWMCTL还不清楚干嘛的

    // PWMPER 周期寄存器 有计算方法
    PWMPER1 = 255;
    
    // PWMDTY为占空比常数寄存器
    PWMDTY1 = 100;    // 

    PWME_PWME1 = 1;    // 使能PWM1通道
}