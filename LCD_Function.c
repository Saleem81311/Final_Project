#include "Functions_Lib.h"
  #include "msp.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#define RS 0x20        // P1.5 mask
#define RW 0x40        // P1.6 mask
#define EN 0x80        // P1.7 mask
#define Estop 0x40     // Estop button BIT on Port 1
#define Reset 0x80     // Reset button BIT on Port 1
void LCD_Function()   // This function operates the LCD
{

/************************************************************* LCD Code*******************************************************/
void LCD_Command(unsigned char command);
void LCD_Data(unsigned char data);
void LCD_Init(void);
void SysTick_Init();
void SysTick_Delay(uint16_t delay);

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	LCD_Init();
	SysTick_Init();
	LCD_Data(0x00);
	SysTick_Delay(100);

	while(1)
	{
	    LCD_Command(0x80);
	    LCD_Data('H');
	    SysTick_Delay(1000);
	}

}

void LCD_Init(void)
{
    P3->DIR |= (RS|RW|EN); // Make P5 pins output for control
    P4->DIR = 0xFF;     // Make P6 pins output for data

    SysTick_Delay(30);      // Initialization sequence
    LCD_Command(0x30);
    SysTick_Delay(10);
    LCD_Command(0x30);
    SysTick_Delay(1);
    LCD_Command(0x30);
    LCD_Command(0x38);      // Set 8-bit data, 2-line, 5x7 font
    LCD_Command(0x06);      // Move cursor right after each char
    LCD_Command(0x01);      // Clear screen, Move cursor to home
    LCD_Command(0x0F);      // Turn on display, Cursor blink
}

void LCD_Command(unsigned char command)
{
    P3->OUT &= ~(RS|RW);      // RS = 0, RW = 0
    P4->OUT = command;      // Put command on data bus
    P3->OUT |= EN;      // Pulse EN HIGH
    SysTick_Delay(1);
    P3->OUT &= ~EN;     // Clear EN
    if(command < 4)
      SysTick_Delay(4);    // Desired command speed
    else
      SysTick_Delay(1);    // Any others need 40 us
}

void LCD_Data(unsigned char data)
{
    P3->OUT |= RS;      // RS = 1
    P3->OUT &= ~RW;     // RW = 0
    P4->OUT = data;     // Put data on bus
    P3->OUT |= EN;      // Pulse EN
    SysTick_Delay(1);
    P3->OUT &= ~EN;     // Clear EN
    SysTick_Delay(2);   // Wait for controller to do the display
}

}

void SysTick_Init()     // SysTick Initializations
{
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->CTRL = 0;
    SysTick->VAL = 32;
    SysTick->CTRL = 0x00000005;
}

void SysTick_Delay(uint16_t delay)      // SysTick calculations for the delay time in ms
{
    SysTick->LOAD = (delay*3000);
    SysTick->VAL = 5;
    while((SysTick->CTRL & 0x00010000) == 0);
}
