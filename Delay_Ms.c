#include "Functions_Lib.h"

void delay_ms(unsigned ms)
{
  SysTick->LOAD = ms*3000 - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = 5;
  while(!(SysTick->CTRL & BIT(16)))
}
