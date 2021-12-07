#include "Functions_Lib.h"

void Set_Motor_Speed(void)
{
  int num = 0, speed = 0;
  num = Read_Keypad();                   //get key press from the keypad
    if ((num != 10) && (num != 12) && (num > 0) && (num < 13))
    {
        if (num == 11)
            num = 0;
        speed = num * 10;
        printf("Speed Entered: %d\n", num);    //confirms speed
        TimerA1_PWM(speed);                        //changes timer to new PWM
    }
}
