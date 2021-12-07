#include "Functions_Lib.h"

void SysTick_Delay(uint16_t delay)
{
    SysTick->LOAD = (delay*3000);
    SysTick->VAL = 5;
    SysTick->CTRL = 1;
    while((SysTick->CTRL & 0x00010000) == 0);
}
