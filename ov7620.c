#include <hidef.h>
#include <MC9S12XS128.h>

#include "derivative.h"

#define ROW 40
#define COLUMN 120

unsigned char ImageData[ROW][COLUMN];

unsigned char VSYNCount = 0;
unsigned int LineCount = 0;
unsigned char m = 0;

unsigned char Interval;
unsigned char Threshold = 0x45;    // 二值化图像阈值 具体数字根据环境情况调动

void PLL_Init(void);
void TIM_Init(void);
void SCI0_Init(void);
void SCI0_Transmit(void);

/*
    函数名称:main
    功能描述：主函数
*/
void main(void)
{
    /* 初始化代码段 */
    DisableInterrupts;    // 关中断
    DDRA = 0x00;    // 端口A的8个寄存器设置为输入模式 0表示输入
    DDRB - 0xFF;    // 端口B的8个寄存器设置为输出模式 1表示输出
    PORTB = 0xFF;    // 将端口B的8个寄存器值全置为1
    PLL_Init();
    SCI0_Init();
    TIM_Init();

    EnableInterrupts;    // 开中断
    for(;;)
    {
        if(VSYNCount)
        {
            SCI0_Transmit();

            VSYNCount = 0;

        }
    }
}

/*
    函数名称:PLL_Init
    功能描述:时钟初始化函数，运用锁相环使系统更加快速稳定运行
    说明:提高总线频率，加快单片机运行速度，俗称超频
    外部晶振频率OSCCLK = 16M 经过系列计算与PLL稳定 得到总线时钟24M
*/
void PLL_Init(void)
{
    CLKSEL = 0x00;    // CLKSEL为时钟选择寄存器 不使用锁相环时总线频率等于晶振频率一半 即8M
    
    // 仍然有疑惑 为什么是0xC0相或？？？？？
    SYNR = 0xC0 | 0x05;    // 同步寄存器 Synthesizer Register 0-5位决定SYNDIV SYNDIV=5
    REFDV = 0xC0 | 0x03;    // Reference Divider Register 0-5位决定REFDIV    REFDIV=3
    PLLCTL_PLLON = 1;    // 0:关闭PLL电路 1:打开PLL电路
    POSTDIV = 0x00;    // 当POSTDIV等于0时 PLL频率等于VCO频率

    /*
        短暂延时，等待PLL时钟频率稳定
        nop为空等待汇编指令 执行时什么都不做 起延时作用
        一条语句的执行时间为一个时钟周期
    */
    asm(nop);
    asm(nop);

    while(0 == CRGFLG_LOCK);    // 锁相环锁定判别 若为0则锁定 为1则执行下一步
    CLKSEL_PLLSEL = 1;    // CLKSEL寄存器的第7位PLLSEL 又称PLL选择位 PLLSEL=1时 总线频率等于锁相环频率的一半
}


/*
    函数名称:TIM_Init
    功能描述:行场中断初始化函数
    说明:行中断上升沿触发 场中断下降沿触发
*/
void TIM_Init(void)
{
    // 结合手册 本函数已完全了解

    TIOS = 0x00;    // 定时器通道0,1为输入捕捉 0为输入 1为输出
    TSCR1 = 0x80;    // 第7位TEM为1 其余位为0 定时器使能
    TCTL4 = 0x09;    // 第3位EDG1B为1 第0位EDG0A为1 通道0捕捉上升沿 通道1捕捉下降沿 其他通道都不捕捉
    TIE = 0x03;    // 通道0和1（C0I C1I）置为1 中断使能
    TFLG1 = 0xFF;    // 1表示清除中断标志位 此语句清全部中断标志位
}

/*
    函数名称:串口初始化函数 SCI0_Init
    功能描述:串口1初始化函数？？到底是串口1还是串口0 函数都那样叫了 应该是串口0吧
    说明:波特率115200 串口SCI0 波特率的计算 波特率的概念也需要了解一下
*/
void SCI0_Init(void)
{
    SCI0BDL = (byte)((24000000 /* PLL freq / 2 */) / 115200 /* baud rate 波特率*/ / 16 /*factor*/);
    SCI0CR1 = 0x00;    /* normal, no parity 正常8位 无奇偶校验 */
    // 复位后 在SCICR2的TE和RE置1前 波特率发生器是关闭的
    SCI0CR2 = 0x0C;    /* RIE = 1？？？不应该是RIE=0吗, TE = 1, RE = 1 接收中断允许 */
    // 此时波特率发生器打开
}

/*
    函数名称:串口发射端程序 SCIO_Transmit
    功能描述:发送图像采集像素到上位机 这里的上位机为DemokTool
    说明:发送图像数据前需要按照协议发送0xFF图像头
*/
void SCI0_Transmit(void)
{
    byte temp;
    unsigned char i, j;
    temp = SCI0SR1;    // ???
    SCI0DRH = 0;    // ???

    // 这里有点懵逼 需要详细了解了解
}


// 中断处理函数
/*
    
*/

#pragma CODE_SEG NON_BANNKED    // 中断函数置于非分页区

/*
    函数名称:行中断处理函数
    功能描述:行中断处理
*/
interrupt 8 void HREF_Count(void)    // 捕捉中断号为8事件进行处理
{
    // 暂不清楚原理
}

/*
    函数名称:场中断处理函数
    功能描述:场中断处理
*/
interrupt 9 void VSYN_Interrupt(void)
{
    // 暂不清楚原理
}


#pragma CODE_SEG DEFAULT    // 后续代码置于分页区
