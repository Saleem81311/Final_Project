#include "msp.h"
#include "Functions_Lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

void ADC14_init (void)
{
  ADC14->CTL0 &=~ 0x00000002;          // disable ADC14ENC during configuration
  ADC14->CTL0 = 0b11000101111001000111011110010000;
  ADC14->CTL1 = 000000000000000110000;
  ADC14->MCTL[0] = 14 | BIT7;
  ADC14->CTL0 |= BIT1;
  ADC14->CTL0 |= BIT0;
  ADC14->IER0 |= BIT0;
}
