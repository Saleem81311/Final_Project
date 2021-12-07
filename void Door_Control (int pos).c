#include "Functions_Lib.h"

{
  if (pos == 1)
  {
    P3->OUT &= ~BIT6;
    P3->OUT |= BIT7;
    TimerA2_PWM(1);
  }
  if (pos == 0)
  {
    P3->OUT &= ~BIT7;
    P3->OUT |= BIT6;
    TimerA2_PWM(0);
  }
}
