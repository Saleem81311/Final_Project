#include "Functions_Lib.h"

{
  if (ADC14->IFGR0 & BIT0){
  nADC = (ADC14->MEM[0] * 3.3) / 255;
  duty = nADC / 3.3;
  TimerA3_PWM((int)(duty * 100));
}
