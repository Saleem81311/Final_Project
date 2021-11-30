/**********************************************************************************************************
* Developer(s): Tennison Hoffman, Saleem Griggs-Taylor, Kevin Figurski
* Start Date: 11/15/21
* Last modified: 11/15/21
* Description: This code was developed to interface and model the MSP432 full capabilities by programming
* a control system that will operate an LCD screen, motors, keypad, and LEDs.
***********************************************************************************************************/
#include "Functions_Lib.h"
#include "msp.h"
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>


int main()
{
  WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
}
