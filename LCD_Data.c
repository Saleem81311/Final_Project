#include "msp.h"
#include "Functions_Lib.h"

void LCD_Data (unsigned char data)
{
  P6->OUT |= RS;      // RS = 1
  P6->OUT &= ~RW;     // RW = 0
  P9->OUT = data;     // Put data on bus
  P6->OUT |= EN;      // Pulse EN
  SysTick_Delay(1);
  P6->OUT &= ~EN;     // Clear EN
  SysTick_Delay(2);   // Wait for controller to do the display
}
