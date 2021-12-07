#include "Functions_Lib.h"

void TimerA2_PWM(int position)
{
  TIMER_A2-> CTL = 0b1000010100;                      //Count up using smclk, clears TAOR register, /1
  TIMER_A2-> CCR[0] = 63000 - 1;                      //TimerA will count up to 63000-1
   if (position == 0)
   {
      TIMER_A2-> CCR[4] = 2700 - 1;
      TIMER_A2-> CCR[3] = 2700 - 1;
      TIMER_A2-> CCR[2] = 6300 - 1;
   }
  
   if (position == 1)
   {
      TIMER_A2-> CCR[4] = 6300 - 1;
      TIMER_A2-> CCR[3] = 6300 - 1;
      TIMER_A2-> CCR[2] = 2700 - 1;
   }
  TIMER_A2-> CCTL[4] = 0b11100000;
  TIMER_A2-> CCTL[3] = 0b11100000;                    //reset/set mode
  TIMER_A2-> CCTL[2] = 0b11100000;
}
