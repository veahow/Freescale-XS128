/*
    函数名称:PLL_Init(void)
    功能描述:时钟初始化函数，运用锁相环使系统更加快速稳定运行
    说明:提高总线频率，加快单片机运行速度，俗称超频
    外部晶振频率OSCCLK = 16M 经过系列计算与PLL稳定 得到总线时钟24M

    这里应该没啥问题 还需要重新理解下 建立编程模型
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
    函数名称:PWM_Init(void)
    功能描述:PWM模块初始化函数
    说明:初始化PWM通道输出PWM波 驱动电机默认占空比为53% 驱动舵机默认频率50Hz且占空比为7.5%
*/
void PWM_Init(void)
{
    PWME = 0;    // 关闭PWM所有通道
    
    // 没有使用PLL_Init()时总线频率为8MHz（预分频后为500K） 使用PLL_Init()后总线频率为24MHz（预分频后为1500K）
    // PWMCLK 选择时钟源
    PWMPRCLK = 0x44;    // PWMPRCLK寄存器单独给时钟源进行16预分频 A、B时钟均为500KHz
    PWMSCLA = 0x7D;    // SA时钟为2000Hz 计算公式：A时钟频率/2/SCLA
    PWMSCLB = 0x7D;    // SB时钟为2000Hz 计算公式同上
    
    // PWM通道选择时钟源
    PWMCLK_PCLK0 = 1;    // PWM通道0设SA为时钟源 
    PWMCLK_PCLK2 = 1;    // PWM通道2设SB为时钟源
    
    PWMPOL_PPOL0 = 1;    // 设置PWMPOL选择输出通道波形的翻转 这里选择上升沿翻转
    PWMPOL_PPOL2 = 1;
    PWMCAE_CAE0 = 0;    // 左对齐输出
    PWMCAE_CAE2 = 0;
    
    // 控制占空比最重要的寄存器操作 电机可控占空比为53% 54% 55% 57% 58% 60%以上（60%已经转得非常快了）
    PWMDTY0 = 0x00;    // PWM0占空比为0
    PWMPER0 = 0x0F;    // PWM0输出频率为133Hz的波
    PWMDTY1 = 0x00;    // PWM1占空比为0
    PWMPER1 = 0x0F;    // PWM1输出频率为133Hz的波

    // 舵机频率、占空比设置
    PWMDTY2 = 0x00;    // PWM2占空比为
    PWMPER2 = 0x28;    // PWM2输出频率为50Hz的波

    PWMCNT = 0x00;    // 0通道计数器清0 本语句暂无用处

    PWME_PWME0 = 1;    // PWM0通道使能 P0通道为输出通道 控制电机IN1口
    PWME_PWME1 = 1;    // PWM1通道使能 P1通道为输出通道 控制电机IN2口
    PWME_PWME2 = 1;    // PWM2通道使能 P2通道为输出通道 控制舵机
}


/*
    函数名称:TIM_Init()
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
    函数名称:PIT_Init()
    功能描述:定时中断初始化函数
    说明:可进行定时中断设置 默认为10ms定时中断
*/
void PIT_Init(void)
{
    PITCFLMT_PITE = 0;    // 关闭PIT模块
    PITCE_PCE0 = 1;    // 定时器通道0使能
    PITMUX_PMUX0 = 0;    // 定时器通道0使用微计数器0

    PITMTLD0 = 80 - 1;    // 8位定时器初值设定 对总线时钟进行80分频得到0.01ms 没有使用PLL时总线时钟为8MHz 
    PITLD0 = 1000 - 1;    // 16位定时器初值设定 1000*0.01 = 10ms
    // 定时10ms

    PITINTE_PINTE0 = 1;    // 定时器中断通道0中断使能(溢出中断)
    PITCFLMT_PITE = 1;    // 定时器通道0使能
}

void PID_Init(void)
{
    sptr->LastError = 0;    // Error[-1]
    sptr->PrevError = 0;    // Error[-2]
    sptr->Proportion = P_DATA;    // 比例常数P
    sptr->Integral = I_DATA;    // 积分常数I
    sptr->Derivative = D_DATA;    // 微分常数D
    sptr->SetPoint = 40;    // 目标为40 即速度v=40*4.2=168cm/s
}

/*
    函数名称:GPIO_Init()
    功能描述:IO口初始化函数
    说明:
*/
void GPIO_Init(void)
{
    DDRP = 0x07;    // 端口P的P0、P1、P2寄存器设置为输出模式 1表示输出

    /*
        Port P为输出端口 需要事先定义为输出端口
        选用P端口0通道与1通道同直流电机IN1与IN2相连 控制电机的正反转
        选用P端口2通道与舵机信号线相连 控制舵机转动
        默认电机为正转
    */
    PTP_PTP0 = 1;
    PTP_PTP1 = 0;
    PTP_PTP2 = 1;
}

/*
    函数名称:串口初始化函数 SCI0_Init
    功能描述:串口SCI0初始化 选用PS0和PS1端口分别连接外部串口模块的RXD和TXD
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

