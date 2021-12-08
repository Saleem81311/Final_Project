#include "msp.h"
#include "Functions_Lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

void TimerA3_PWM(float duty)
{
  TIMER_A3-> CTL = 0b1000010100;                      //Count up using smclk, clears TAOR register, /1
  TIMER_A3-> CCR[0] = 10000 - 1;                      //300 Hz
    if (duty < 3)
        TIMER_A3-> CCR[2] = 0;
    else
        TIMER_A3-> CCR[2] = (10000 * (duty / 100)) - 1;
  TIMER_A3-> CCTL[2] = 0b11100000;                    //reset/set
}
