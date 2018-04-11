#define ROW 40
#define COLUMN 120

unsigned char ImageData[ROW][COLUMN];

unsigned char VSYNCount = 0;
unsigned int LineCount = 0;
unsigned char m = 0;

unsigned char Interval;
unsigned char Threshold = 0x45;    // 二值化图像阈值 具体数字根据环境情况调动

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
