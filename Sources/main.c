#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "init.h"
#include "motor.h"
#include "servo.h"

#define ROW 40
#define COLUMN 120
#define HAVE_NEW_VELOCITY 0x01

extern unsigned char ImageData[ROW][COLUMN];
extern unsigned int VSYNCount;
extern unsigned int LineCount;
extern unsigned char Interval;
extern unsigned char m;

extern unsigned char CurrentVelocity;
extern unsigned char flag;





void main(void) {
  /* put your own code here */
  DisableInterrupts;    // ���ж�
  
  // ��ʼ������
  PWM_Init();
  GPIO_Init();
  PACTL = 0x51;
  
  
  EnableInterrupts;    // ���ж�
  
  
  for(;;)
  {
        /*
        if(ͼ��ɼ����){
            ͼ����();
            ����ʶ��();
            �������();
            �ٶȿ���();
        }
        */
   }


	EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}


/*
    �жϴ�����    
*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED    //ָʾ�ó����ڲ���ҳ��
/*
    ��������:PIT0()
    ��������:10ms��ʱ����

    ��������Ҫ�о�һ��
*/
void interrupt 66 PIT0(void)    // ��׽�жϺ�Ϊ66���¼����д���
{
    // Ŀǰ��ֱ�Ӳ��� ��ʱ��Ϊ10ms 
    CurrentVelocity = PACNT;    // PT7�ڲ��ٶ�ӦPACNT�Ĵ���                  
    PACNT = 0x0000;    // PACNT����  
    flag |= HAVE_NEW_VELOCITY;

    /*
    vTmpPIT++;
    if(vTmpPIT == ??) vTmpPIT = 0;
    // �������ڸı䶨ʱ�жϵ�ʱ�� ������Ĭ�ϵĶ�ʱ
    
    */
    PITTF_PTF0 = 1;    //���жϱ�־λ
}        

#pragma CODE_SEG NON_BANNKED    // �жϺ������ڷǷ�ҳ��
#pragma CODE_SEG __NEAR_SEG NON_BANKED    //ָʾ�ó����ڲ���ҳ��
/*
    ��������:���жϴ�����
    ��������:���жϴ���
*/
interrupt 8 void HREF_Count(void)    // ��׽�жϺ�Ϊ8�¼����д���
{
    TFLG1_C0F = 1;
    m++;
    if ( m < 6 || m > 240 ) return;    //�ж��Ƿ���µ�һ����ʼ
    
    Interval = 6;
    if(m % Interval == 0)
    {
        ImageData[LineCount][0] = PORTA;
        ImageData[LineCount][1] = PORTA;
        ImageData[LineCount][2] = PORTA;
        ImageData[LineCount][3] = PORTA;
        ImageData[LineCount][4] = PORTA;
        ImageData[LineCount][5] = PORTA;
        ImageData[LineCount][6] = PORTA;
        ImageData[LineCount][7] = PORTA;
        ImageData[LineCount][8] = PORTA;
        ImageData[LineCount][9] = PORTA;
        ImageData[LineCount][10] = PORTA;
        ImageData[LineCount][11] = PORTA;
        ImageData[LineCount][12] = PORTA;
        ImageData[LineCount][13] = PORTA;
        ImageData[LineCount][14] = PORTA;
        ImageData[LineCount][15] = PORTA;
        ImageData[LineCount][16] = PORTA;
        ImageData[LineCount][17] = PORTA;
        ImageData[LineCount][18] = PORTA;
        ImageData[LineCount][19] = PORTA;
        ImageData[LineCount][20] = PORTA;
        ImageData[LineCount][21] = PORTA;
        ImageData[LineCount][22] = PORTA;
        ImageData[LineCount][23] = PORTA;
        ImageData[LineCount][24] = PORTA;
        ImageData[LineCount][25] = PORTA;
        ImageData[LineCount][26] = PORTA;
        ImageData[LineCount][27] = PORTA;
        ImageData[LineCount][28] = PORTA;
        ImageData[LineCount][29] = PORTA;
        ImageData[LineCount][30] = PORTA;
        ImageData[LineCount][31] = PORTA;
        ImageData[LineCount][32] = PORTA;
        ImageData[LineCount][33] = PORTA;
        ImageData[LineCount][34] = PORTA;
        ImageData[LineCount][35] = PORTA;
        ImageData[LineCount][36] = PORTA;
        ImageData[LineCount][37] = PORTA;
        ImageData[LineCount][38] = PORTA;
        ImageData[LineCount][39] = PORTA;
        ImageData[LineCount][40] = PORTA;
        ImageData[LineCount][41] = PORTA;
        ImageData[LineCount][42] = PORTA;
        ImageData[LineCount][43] = PORTA;
        ImageData[LineCount][44] = PORTA;
        ImageData[LineCount][45] = PORTA;
        ImageData[LineCount][46] = PORTA;
        ImageData[LineCount][47] = PORTA;
        ImageData[LineCount][48] = PORTA;
        ImageData[LineCount][49] = PORTA;
        ImageData[LineCount][50] = PORTA;
        ImageData[LineCount][51] = PORTA;
        ImageData[LineCount][52] = PORTA;
        ImageData[LineCount][53] = PORTA;
        ImageData[LineCount][54] = PORTA;
        ImageData[LineCount][55] = PORTA;
        ImageData[LineCount][56] = PORTA;
        ImageData[LineCount][57] = PORTA;
        ImageData[LineCount][58] = PORTA;
        ImageData[LineCount][59] = PORTA;
        ImageData[LineCount][60] = PORTA;
        ImageData[LineCount][61] = PORTA;
        ImageData[LineCount][62] = PORTA;
        ImageData[LineCount][63] = PORTA;
        ImageData[LineCount][64] = PORTA;
        ImageData[LineCount][65] = PORTA;
        ImageData[LineCount][66] = PORTA;
        ImageData[LineCount][67] = PORTA;
        ImageData[LineCount][68] = PORTA;
        ImageData[LineCount][69] = PORTA;
        ImageData[LineCount][70] = PORTA;
        ImageData[LineCount][71] = PORTA;
        ImageData[LineCount][72] = PORTA;
        ImageData[LineCount][73] = PORTA;
        ImageData[LineCount][74] = PORTA;
        ImageData[LineCount][75] = PORTA;
        ImageData[LineCount][76] = PORTA;
        ImageData[LineCount][77] = PORTA;
        ImageData[LineCount][78] = PORTA;
        ImageData[LineCount][79] = PORTA;
        ImageData[LineCount][80] = PORTA;
        ImageData[LineCount][81] = PORTA;
        ImageData[LineCount][82] = PORTA;
        ImageData[LineCount][83] = PORTA;
        ImageData[LineCount][84] = PORTA;
        ImageData[LineCount][85] = PORTA;
        ImageData[LineCount][86] = PORTA;
        ImageData[LineCount][87] = PORTA;
        ImageData[LineCount][88] = PORTA;
        ImageData[LineCount][89] = PORTA;
        ImageData[LineCount][90] = PORTA;
        ImageData[LineCount][91] = PORTA;
        ImageData[LineCount][92] = PORTA;
        ImageData[LineCount][93] = PORTA;
        ImageData[LineCount][94] = PORTA;
        ImageData[LineCount][95] = PORTA;
        ImageData[LineCount][96] = PORTA;
        ImageData[LineCount][97] = PORTA;
        ImageData[LineCount][98] = PORTA;
        ImageData[LineCount][99] = PORTA;
        ImageData[LineCount][100] = PORTA;
        ImageData[LineCount][101] = PORTA;
        ImageData[LineCount][102] = PORTA;
        ImageData[LineCount][103] = PORTA;
        ImageData[LineCount][104] = PORTA;
        ImageData[LineCount][105] = PORTA;
        ImageData[LineCount][106] = PORTA;
        ImageData[LineCount][107] = PORTA;
        ImageData[LineCount][108] = PORTA;
        ImageData[LineCount][109] = PORTA;
        ImageData[LineCount][110] = PORTA;
        ImageData[LineCount][111] = PORTA;
        ImageData[LineCount][112] = PORTA;
        ImageData[LineCount][113] = PORTA;
        ImageData[LineCount][114] = PORTA;
        ImageData[LineCount][115] = PORTA;
        ImageData[LineCount][116] = PORTA;
        ImageData[LineCount][117] = PORTA;
        ImageData[LineCount][118] = PORTA;
        ImageData[LineCount][119] = PORTA;
        
        LineCount++;
    }

    if(LineCount == ROW) TIE = 0x02;    // �����ж�
  
}

/*
    ��������:���жϴ�����
    ��������:���жϴ���
*/
interrupt 9 void VSYN_Interrupt(void)
{
    TFLG1_C1F = 1;    //�峡�ж�
    TFLG1_C0F = 1;    //�����ж�
    
    LineCount = 0;    //�м�����
    VSYNCount = ++VSYNCount;
    m = 0;

    TIE_C0I = 1;
    TIE_C1I = 0;    // �����ж�
}


#pragma CODE_SEG DEFAULT    // �����������ڷ�ҳ��
