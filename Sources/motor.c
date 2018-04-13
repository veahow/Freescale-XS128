/*
    Port PΪ����˿� ��0��ʾ����Ϊ����˿� ��1��ʾ����Ϊ����˿�
    ���Ƚ�P0 P1�˿�ȫ��1 ��ʾ����Ϊ����˿�
    ѡ��P�˿�0ͨ����1ͨ��ֱͬ�����IN1��IN2���� ���Ƶ��������ת
*/
#include "derivative.h"
#include "motor.h"

/* 
    PID���Ʊ�����
*/
// ����PID�ṹ��
typedef struct PID
{
    int SetPoint;    // �趨Ŀ��ֵ
    
    // PID����
    double Proportion;    // �������� Proportional Const
    double Integral;    // ���ֳ��� Integral Const
    double Derivative;    // ΢�ֳ��� Derivative Const
    
    // ���
    int LastError;    // Error[-1]
    int PrevError;    // Error[-2]
} PID;

static PID motorPID;
static PID *sptr = &motorPID;

#define P_DATA 10
#define I_DATA 0
#define D_DATA 0
#define HAVE_NEW_VELOCITY 0x01

unsigned char CurrentVelocity = 0;    // С����ǰ�ٶ�
unsigned char vTmpPIT = 0;    // PIT�м����
unsigned char flag = 0;    // ��־λ

/*
    ��������:PID_Init(void)
    ��������:PID��ʼ������
*/
void PID_Set(void)
{
    sptr->LastError = 0;    // Error[-1]
    sptr->PrevError = 0;    // Error[-2]
    sptr->Proportion = P_DATA;    // ��������P
    sptr->Integral = I_DATA;    // ���ֳ���I
    sptr->Derivative = D_DATA;    // ΢�ֳ���D
    sptr->SetPoint = 40;    // Ĭ��Ŀ��Ϊ40 ���ٶ�v=40*4.2=168cm/s
}


int IncPIDCalc(int NextPoint) {
    int iError, iIncPid;    // ��ǰ���
    iError = sptr->SetPoint - NextPoint;    // ��������
    iIncPid = sptr->Proportion * iError - sptr->Integral * sptr->LastError + sptr->Derivative * sptr->PrevError;    // E[k-2]��
    sptr->PrevError = sptr->LastError;    // �洢��� �����´μ���
    sptr->LastError = iError;    // �洢��� �����´μ���
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
    ��������:Motor_Brake(void)
    ˵��:���ֹͣ
*/
void Motor_Brake(void)
{
    // �˿�����͵�ƽ ֹͣ������� �����ⲿ��Ҫ��Ҫɾ��
    PTP_PTP0 = 0;
    PTP_PTP1 = 0;
    
    // ռ�ձ�Ϊ0
    PWME_PWME0 = 0;
    PWMDTY0 = 0x00;
    PWMPER0 = 0x00;

    // ���ֹͣ��Ҫ����������Ҫ���µ��ú���IO_Init()
}

/*
    ��������:Motor_Forward_Fast(void)
    ˵��:�����ת����
*/
void Motor_Forward_Fast(void)
{
    // ռ�ձ�Ϊ58%
    PWME_PWME0 = 0;    
    PWMDTY0 = 0x07;
    PWMPER0 = 0x0C;
    PWME_PWME0 = 1;  
}

/*
    ��������:Motor_Forward_Slow(void)
    ˵��:�����ת����
*/
void Motor_Forward_Slow(void)
{
    // ռ�ձ�Ϊ53%
    PWME_PWME0 = 0;    
    PWMDTY0 = 0x08;
    PWMPER0 = 0x0F;
    PWME_PWME0 = 1;
}

/*
    ��������:Motor_Back_Fast(void)
    ˵��:�����ת����
*/
void Motor_Back_Fast(void)
{
    PTP_PTP0 = 0;
    PTP_PTP1 = 1;

    // ռ�ձ�Ϊ58%
    PWME_PWME1 = 0;    
    PWMDTY1 = 0x07;
    PWMPER1 = 0x0C;
    PWME_PWME1 = 1;
}

/*
    ��������:Motor_Back_Slow(void)
    ˵��:�����ת����
*/
void Motor_Back_Slow(void)
{
    PTP_PTP0 = 0;
    PTP_PTP1 = 1;

    // ռ�ձ�Ϊ53%
    PWME_PWME1 = 0;    
    PWMDTY1 = 0x08;
    PWMPER1 = 0x0F;
    PWME_PWME1 = 1;
}