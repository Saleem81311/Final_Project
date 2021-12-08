#include "msp.h"
#include "Functions_Lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

void LCD_Function(int Menu_Switch)
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
