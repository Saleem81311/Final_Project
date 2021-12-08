#include "msp.h"
#include "Functions_Lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

void SysTick_Init()
{
  SysTick->LOAD = 0x00FFFFFF;
  SysTick->CTRL = 0;
  SysTick->VAL = 32;
  SysTick->CTRL = 0x00000005;
}
