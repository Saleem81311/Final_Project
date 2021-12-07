#include "msp.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#define RS 0x20        // P6.4 mask
#define RW 0x40        // Goes to GND since always writing to LCD
#define EN 0x80        // P6.5 mask

void LCD_Command(unsigned char command);
void LCD_Data(unsigned char data);
void LCD_Init(void);
void SysTick_Init();
void SysTick_Delay(uint16_t delay);
void String(char Menu[]);

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	int i;
	int SIZE = 30;
	int Menu_Switch = 0;
	LCD_Init();
	SysTick_Init();
	LCD_Data(0x00);
	SysTick_Delay(100);

	while(1)
	{
        int Keypad_Num = 0;

	    switch(Menu_Switch)
	    {
	      case 0:       // Home screen case
	        LCD_Command(0x80);
	        Message("Choose your     ");
	        LCD_Command(0xC0);
	        Message("function.");
	        SysTick_Delay(2000);
	        LCD_Command(0x00);
	        Menu_Switch = 1;
	        break;

	      case 1:		// LCD displaying the different options available to choose from
	        LCD_Command(0x80);
	        Message("1 - Doors");
	        LCD_Command(0xC0);
	        Message("2 - Lights");
	        LCD_Command(0x90);
	        Message("3 - Motor");
	        SysTick_Delay(3000);
	        LCD_Command(0x00);
	        Menu_Switch = 2;

	        //if()      // If statement to receive either the enter button or switching the menu to the next message
	        break;

	      case 2:

	        if(Keypad_Num == 1)     // Keypad number message to choose a door to open/close
	         {
	           LCD_Command(0x80);
	           Message("Which door do");
	           LCD_Command(0xC0);
	           Message("you want?");
	           LCD_Command(0x90);
	           Message("1 - Left");
	           LCD_Command(0xD0);
	           Message("2 - Right");
	         }

	        if(Keypad_Num == 2)     // Keypad number to display lights activity
	        {
	            LCD_Command(0x80);
	            Message("Lights are ");
	            LCD_Command(0xC0);
	            Message("Active");
	        }

            if(Keypad_Num == 3)     // Keypad number to display the motor is running
            {
                LCD_Command(0x80);
                Message("Motor is running");
            }

	         break;
	    }
	}

}

void LCD_Command(unsigned char command)
{
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

void Message(char Menu[])
{
    int i;

    for(i = 0; i < strlen(Menu); i++)
    {
        LCD_Data(Menu[i]);
        if(i == (strlen(Menu)-1))       // strlen = String length meaning the amount of characters in the array and subtracting it by one so that we are sure it will go in this statement due to if i equals the string length it will not execute the statement any longer
        {
            LCD_Command(0x0C);      // Turn cursor off after it is done printing statements
        }
    }
}
