#include "Functions_Lib.h"

void delay_micro(unsigned micro)
{
  SysTick->LOAD = micro*3 - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = 5;
  while(!(SysTick->CTRL & BIT(16)));
}
