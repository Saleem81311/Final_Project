#include "msp.h"
#include "Functions_Lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

void PORT6_IRQHandler(void)
{
   if(P6->IFG & BIT4)
    {
      __delay_cycles(15000);
        if (!(P6->IN & BIT0))
        {
          printf("Emergency Stop!\n");
          TimerA1_PWM(0);
          P6->IFG  &= ~BIT0;  //clear the flag
        }
    }
} 
