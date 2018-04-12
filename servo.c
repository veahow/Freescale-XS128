/*
    Port P为输出端口 置0表示定义为输入端口 置1表示定义为输出端口
    事先将P2端口置1 表示定义为输出端口
    选用P端口2通道与舵机信号线相连 控制舵机转动 从而改变小车运行方向
*/
void Servo_Turn_Right(void)
{
    // 端口输出低电平 停止驱动舵机 舵机应该保持不动
    PTP_PTP2 = 0;

    // 占空比为10% 向右转45°
    PWME_PWME2 = 0;
    PWMDTY2 = 0x04;
    PWMPER2 = 0x28;    // 频率50Hz
    PWME_PWME2 = 1;

    PTP_PTP2 = 1;
}

void Servo_Turn_Left(void)
{
    PTP_PTP2 = 0;

    // 占空比为5% 向左转45°
    PWME_PWME2 = 0;
    PWMDTY2 = 0x02;
    PWMPER2 = 0x28;    // 频率50Hz
    PWME_PWME2 = 1;

    PTP_PTP2 = 1;
}
