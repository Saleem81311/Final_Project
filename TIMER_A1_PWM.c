#include "msp.h"
#include "Functions_Lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

void TimerA1_PWM(int dutyCycle)
{               //runs timer given the specified duty cycle
  TIMER_A1-> CTL = 0b1001010100;                      //Count up using smclk, clears TAOR register, /2
  TIMER_A1-> CCR[0] = 37500 - 1;                      //TimerA will count up to 37500-1
    if (duty == 0)
        TIMER_A1-> CCR[4] = 0;
  else
  TIMER_A1-> CCR[4] = (37500 * dutyCycle / 100) - 1;   //Sets the duty cycle.
  TIMER_A1-> CCTL[4] = 0b11100000;                    //reset/set mode
}
