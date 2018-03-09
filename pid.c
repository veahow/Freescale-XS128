#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

//-----------------------------------------------------
//定义PID结构体
//-----------------------------------------------------
typedef struct PID
{
    int SetPoint; //设定目标Desired Value
    double Proportion; //比例常数Proportional Const
    double Integral; //积分常数Integral Const
    double Derivative; //微分常数Derivative Const
    int LastError; //Error[-1]
    int PrevError; //Error[-2]
} PID;


//-----------------------------------------------------
//声明PID实体
//-----------------------------------------------------
static PID sPID;
static PID *sptr = &sPID;

//-----------------------------------------------------
//定义相关宏
//-----------------------------------------------------
#define P_DATA 10
#define I_DATA 0
#define D_DATA 0
#define HAVE_NEW_VELOCITY 0X01

int g_CurrentVelocity;        //速度
int g_Flag;                   //标志位


//-----------------------------------------------------
//定义全局变量
//-----------------------------------------------------
unsigned int  CurrentVelocity=0;
unsigned int  WholeVelocity=0;
unsigned char flag=0;
unsigned char vTmpPIT=0;

//-----------------------------------------------------
//-------------时钟初始化程序---------------------------
//-----------------------------------------------------
void PLL_Init(void)   //PLLCLK=2*OSCCLK*(SYNR+1)/(REFDV+1)
{                     
  
   CLKSEL=0x00;  //48mhz 
   PLLCTL_PLLON=1; 
   SYNR=0XC0 | 0X05; 
   REFDV=0X80 | 0X01; 
   POSTDIV=0X00; 
   _asm(nop); 
   _asm(nop); 
   while(0==CRGFLG_LOCK);//锁相环锁定 
   CLKSEL_PLLSEL=1;//选定外部时钟  
}
//-----------------------------------------
//--------------行场中断初始化-------------
//----------------------------------------- 
void TIM_Init(void) 
{ 
   TIOS  = 0x00;          //T0、T1设置为输入捕捉,摄像头行场中断---关输出比较通道7 
   TCTL1 = 0x00;         //OC7与逻辑通道断开
   TCTL4 = 0x09;         //通道0上升沿触发,通道 1 下降沿触发
   OC7M  = 0x00;         //通道7不管理其他通道
   TSCR1 = 0x80;         //使能定时器
   PACTL = 0x40;         //使能PA,门计数模式,上升沿,中断禁止   0101 0000
   //TIE   = 0x03;         //通道 0，1 中断使能 
   TFLG1 = 0xFF;         //清中断标志位 
   
} 

//-----------------------------------------
//-------------定时中断初始化--------------
//-----------------------------------------
void PIT_Init(void)//定时中断初始化函数 0.01s=10ms定时中断设置 
{
    PITCFLMT_PITE=0; //定时中断通道0关
    PITCE_PCE0=1;    //定时器通道0使能
    PITMTLD0=240-1;  //8位定时器初值设定。240*4000/48000000=20ms
    PITLD0=4000-1;//16位定时器初值设定。PITTIME*0.01MS
    PITINTE_PINTE0=1;//定时器中断通道0中断使能
    PITCFLMT_PITE=1;//定时器通道0中断使能 
}
//-----------------------------------------
//--------------PWM初始化------------------
//-----------------------------------------
void PWM_Init(void)
{    
    //CH01  motor in1
    //CH23  motor in2
    //CH45  servo 
    //SB,B for ch2367
    //SA,A for ch0145   
    PWME  = 0x00;       //PWM禁止
    PWMCTL = 0x70;		  //CH01、23、45级联成16位PWM；
    PWMCAE = 0x00;			//选择输出模式为左对齐输出模式
    PWMPOL = 0xFF;			//先输出高电平，计数到DTY时，反转电平
    PWMPRCLK = 0x33;    //ClockA 8分频=BusClock/8=6MHz ;ClockB 8分频=BusClock/8=6MHz  
    PWMSCLA = 0x03;     //ClockSA=ClockA/(2*3)=1MHz
    PWMSCLB = 0x03;     //ClockSB=ClockB/(2*3)=1MHz      
    PWMCLK  = 0xFF;     //CH0145选择clock SA做时钟源;CH2367选择clock SB做时钟源

    PWMPER01=12000;             //电机设置 1，3 通
    PWMDTY01=0;  
    PWMPER23=12000;   
    PWMDTY23=0;    
    
    PWMCNT01 = 0;	
    PWMCNT23 = 0;	
    PWMCNT45 = 0;		    //计数器清零；
    PWME = 0x2A;     //PWM使能
} 


//-----------------------------------------
//PID参数初始化
//-----------------------------------------
void PID_Init(void)
{
  sptr->LastError = 0;       //Error[-1]
  sptr->PrevError = 0;       //Error[-2]
  sptr->Proportion =P_DATA;  //比例常数Proportional Const
  sptr->Integral =I_DATA;    //积分常数Integral Const
  sptr->Derivative =D_DATA;  //微分常数Derivative Const
  sptr->SetPoint =20;       //目标是40  V=40*4.2=168CM/S
}


//-----------------------------------------
//增量式PID控制设计
//-----------------------------------------
int IncPIDCalc(int NextPoint)
{
   int iError, iIncpid;                  //当前误差
   iError = sptr->SetPoint - NextPoint;            //增量计算
   iIncpid = sptr->Proportion * iError             //E[k]项
             - sptr->Integral * sptr->LastError    //E[k－1]项
             + sptr->Derivative * sptr->PrevError; //E[k－2]项
    sptr->PrevError = sptr->LastError;  //存储误差，用于下次计算
    sptr->LastError = iError;           //存储误差，用于下次计算
    return(iIncpid);                    //返回增量值
}

//-----------------------------------------
//---------主函数--------------------------
//-----------------------------------------
void main(void)
{

  int PWM_Value;
  //DisableInterrupts;
  
  PLL_Init();
  PWM_Init();
  TIM_Init();
  PIT_Init();
  PID_Init();
  DDRB=0XFF;
  g_CurrentVelocity=0;            //全局变量也初始化
  g_Flag=0;                       //全局变量也初始化
  
  EnableInterrupts;


  for(;;) 
  {
    if (g_Flag& HAVE_NEW_VELOCITY)
   {
  
    PWM_Value=IncPIDCalc(CurrentVelocity);
    PWMDTY01+=PWM_Value;
    g_Flag&=~ HAVE_NEW_VELOCITY;
   }
  }
}

  
//---------10ms定时进行速度采样---------//
#pragma CODE_SEG __NEAR_SEG NON_BANKED 
void interrupt 66 PIT0(void) 
{

  CurrentVelocity=PACNT;                   
  PACNT=0x0000;  
  g_Flag|= HAVE_NEW_VELOCITY;                           
  //WholeVelocity += CurrentVelocity;         
   

  vTmpPIT++;
  if(vTmpPIT==100)  //2秒
  {
  PORTB=~PORTB;//输出取反
  vTmpPIT=0;
  }
    PITTF_PTF0=1;//清中断标志位
}        