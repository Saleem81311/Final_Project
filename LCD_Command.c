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

void LCD_Command(unsigned char command)
{
  SysTick_Init();
   P6->OUT &= ~(RS|RW);      // RS = 0, RW = 0
   P9->OUT = command;      // Put command on data bus
   P6->OUT |= EN;      // Pulse EN HIGH
   SysTick_Delay(1);
   P6->OUT &= ~EN;     // Clear EN
   if(command < 4)
      SysTick_Delay(4);    // Desired command speed
   else
      SysTick_Delay(1);    // Any others need 40 us
}
