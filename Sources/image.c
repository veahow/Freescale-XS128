
#include "image.h"
#include "derivative.h"

void Image_Processing(void) 
{
    unsigned char i, j;
    
    
    
    
    // ��ֵ����������ImageBitData
    for(i = 0; i < ROW; ++i){
        for(j = 0; j < COLUMN; ++j){
            if(ImageData[i][j] > ThresholdValue) ImageBitData[i][j] = WHITE;
            else ImageBitData[i][j] = BLACK;
        }
    }

    // �ڰ׽��� ȥ�����
    
    for(i = 0; i < ROW; ++i){
        for(j = 0; j < COLUMN; ++j){
            if(ImageBitData[i][j] &
             !ImageBitData[i-1][j] &
             !ImageBitData[i+1][j] &
             !ImageBitData[i][j-1] &
             !ImageBitData[i][j+1]) ImageBitData[i][j] = WHITE;
        }
    }
    
}


/*
    ��������:���ڷ���˳��� SCIO_Transmit
    ��������:����ͼ��ɼ����ص���λ�� �������λ��ΪDemokTool(�̼��ṩ �Ĭ�Ƽ�)
    ˵��:����ͼ������ǰ��Ҫ����Э�鷢��0xFFͼ��ͷ
*/
void SCI0_Transmit(void)
{
    byte temp;
    unsigned char i, j;
    temp = SCI0SR1;    // ???
    SCI0DRH = 0;    // ???

    SCI0DRL = 0xFF;    // ͼ��ͷ0xFF

    while (!(SCI0SR1&0x80));
    
    for(i = 0; i < ROW; ++i) 
    {
        for(j = 0; j < COLUMN; ++j) 
        { 
            //---------------���Ͷ�ֵ�����ݵ�SSCOM---------------------//
            //if(Image_Data[i][j]>THRESHOLD && Image_Data[i][j+1]>THRESHOLD && Image_Data[i][j-1]>THRESHOLD)  SCI0DRL = 0x30;
            //if(Image_Data[i][j]>THRESHOLD)  SCI0DRL = 0x30;
            // else SCI0DRL = 0X31;    //����
            //---------------���Ͷ�ֵ�����ݵ�SSCOM---------------------//
            
            //---------------�����������ݵ�DEMOKTOOL---------------------//
            if(ImageData[i][j] == 0xFF)  ImageData[i][j] = 0xFE;  // ��Ϊͼ��ͷ �Լ�
            SCI0DRL =  ImageData[i][j];
            while(!(SCI0SR1&0x80)); 
            //---------------�����������ݵ�DEMOKTOOL---------------------//
        }
        
        //SCI0DRL = 0X0A;           //�س�
        //while (!(SCI0SR1&0x80));
        //SCI0DRL = 0X0D;           //����
        //while (!(SCI0SR1&0x80));
    }
}