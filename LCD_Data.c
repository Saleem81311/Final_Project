#include "msp.h"
#include "Functions_Lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#define RS 0x20        // P6.4 mask
#define RW 0x40        // Goes to GND since always writing to LCD
#define EN 0x80        // P6.5 mask

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
