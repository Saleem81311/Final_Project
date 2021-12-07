#include "msp.h"
#include "Functions_Lib.h"

int Read_Keypad (void)      //detects and reads key pressed
{ 
    int i, j, key = 0;
    for (i = 4; i < 7; i++)
    {
        P4->DIR |= (0x01 << i);
        P4->OUT &= ~(0x01 << i);
        __delay_cycles(5000);

        for (j = 0; j < 4; j++)
        {
            if (!(P4->IN & BIT(j))){
                while (!(P4->IN & BIT(j)));
                if (j == 0)
                    key = i - 3;
                else if (j == 1)
                    key = i;
                else if (j == 2)
                    key = i + 3;
                else
                    key = i + 6;
         }
      }
        P4->DIR &= ~(0x7F);
        P4->OUT |= 0x7F;
    }
    return key;
}
