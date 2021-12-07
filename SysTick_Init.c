#include "msp.h"
#include "Functions_Lib.h"

void SysTick_Init()
{
  SysTick->LOAD = 0x00FFFFFF;
  SysTick->CTRL = 0;
  SysTick->VAL = 32;
  SysTick->CTRL = 0x00000005;
}
