/*
    Port PΪ����˿� ��0��ʾ����Ϊ����˿� ��1��ʾ����Ϊ����˿�
    ���Ƚ�P2�˿���1 ��ʾ����Ϊ����˿�
    ѡ��P�˿�2ͨ�������ź������� ���ƶ��ת�� �Ӷ��ı�С�����з���
*/

#include "derivative.h"
#include "servo.h"

void Servo_Turn_Right(void)
{
    // �˿�����͵�ƽ ֹͣ������� ���Ӧ�ñ��ֲ���
    PTP_PTP2 = 0;

    // ռ�ձ�Ϊ10% ����ת45��
    PWME_PWME2 = 0;
    PWMDTY2 = 0x04;
    PWMPER2 = 0x28;    // Ƶ��50Hz
    PWME_PWME2 = 1;

    PTP_PTP2 = 1;
}

void Servo_Turn_Left(void)
{
    PTP_PTP2 = 0;

    // ռ�ձ�Ϊ5% ����ת45��
    PWME_PWME2 = 0;
    PWMDTY2 = 0x02;
    PWMPER2 = 0x28;    // Ƶ��50Hz
    PWME_PWME2 = 1;

    PTP_PTP2 = 1;
}