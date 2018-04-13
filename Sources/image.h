#ifndef _image_H
#define _image_H

/*
    图像处理变量集
*/
// 根据实际单片机实际运行速度来更改采集大小
#define ROW 40    // 图像采集行数
#define COLUMN 120    // 图像采集列数
#define BLACK 0xFF
#define WHITE 0x00

unsigned char ImageData[ROW][COLUMN];    // 图像数组
unsigned char ImageBitData[ROW][COLUMN];    // 二值化图像数组

unsigned int VSYNCount = 0;    // 场数统计
unsigned int LineCount = 0;    // 行数统计
unsigned char m = 0;

unsigned char Interval;    // 采集有效行数间隔
unsigned char ThresholdValue = 0x78;    // 二值化图像阈值 具体数字根据环境情况调动

void Image_Processing(void);

#endif // _image_H