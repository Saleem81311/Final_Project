#include "msp.h"
#include "Functions_Lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

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
