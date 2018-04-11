#include <hidef.h>
#include <MC9S12XS128.h>
#include "init.h"
#include "motor.h"
#include "servo.h"

void main(void)
{
    DisableInterrupts;    // 关中断

    PWM_Init();
    IO_Init();
    
    
    EnableInterrupts;    // 开中断
    
    for(;;)
    {

    }
}