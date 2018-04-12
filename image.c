void Image_Processing(void) 
{
    unsigned char i, j;

    // 二值化存入数组ImageBitData
    for(i = 0; i < ROW; ++i){
        for(j = 0; j < COLUMN; ++j){
            if(ImageData[i][j] > ThresholdValue) ImageBitData[i][j] = WHITE;
            else ImageBitData[i][j] = BLACK;
        }
    }

    // 黑白噪点均值滤波 待定
    /*
    for(i = 0; i < ROW; ++i){
        for(j = 0; j < COLUMN; ++j){

        }
    }
    */
}


/*
    函数名称:串口发射端程序 SCIO_Transmit
    功能描述:发送图像采集像素到上位机 这里的上位机为DemokTool(商家提供 岱默科技)
    说明:发送图像数据前需要按照协议发送0xFF图像头
*/
void SCI0_Transmit(void)
{
    byte temp;
    unsigned char i, j;
    temp = SCI0SR1;    // ???
    SCI0DRH = 0;    // ???

    SCI0DRL = 0xFF;    // 图像头0xFF

    while (!(SCI0SR1&0x80));
    
    for(i = 0; i < ROW; ++i) 
    {
        for(j = 0; j < COLUMN; ++j) 
        { 
            //---------------发送二值化数据到SSCOM---------------------//
            //if(Image_Data[i][j]>THRESHOLD && Image_Data[i][j+1]>THRESHOLD && Image_Data[i][j-1]>THRESHOLD)  SCI0DRL = 0x30;
            //if(Image_Data[i][j]>THRESHOLD)  SCI0DRL = 0x30;
            // else SCI0DRL = 0X31;    //黑线
            //---------------发送二值化数据到SSCOM---------------------//
            
            //---------------发送像素数据到DEMOKTOOL---------------------//
            if(ImageData[i][j] == 0xFF)  ImageData[i][j] = 0xFE;  // 若为图像头 自减
            SCI0DRL =  ImageData[i][j];
            while(!(SCI0SR1&0x80)); 
            //---------------发送像素数据到DEMOKTOOL---------------------//
        }
        
        //SCI0DRL = 0X0A;           //回车
        //while (!(SCI0SR1&0x80));
        //SCI0DRL = 0X0D;           //换行
        //while (!(SCI0SR1&0x80));
    }
}


