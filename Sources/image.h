#ifndef _image_H
#define _image_H

/*
    ͼ���������
*/
// ����ʵ�ʵ�Ƭ��ʵ�������ٶ������Ĳɼ���С
#define ROW 40    // ͼ��ɼ�����
#define COLUMN 120    // ͼ��ɼ�����
#define BLACK 0xFF
#define WHITE 0x00

unsigned char ImageData[ROW][COLUMN];    // ͼ������
unsigned char ImageBitData[ROW][COLUMN];    // ��ֵ��ͼ������

unsigned int VSYNCount = 0;    // ����ͳ��
unsigned int LineCount = 0;    // ����ͳ��
unsigned char m = 0;

unsigned char Interval;    // �ɼ���Ч�������
unsigned char ThresholdValue = 0x78;    // ��ֵ��ͼ����ֵ �������ָ��ݻ����������

void Image_Processing(void);

#endif // _image_H