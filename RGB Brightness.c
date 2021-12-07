#include "msp.h"
#include <stdio.h>
#include <stdlib.h>

void b1pressed(void);     //declare functions
void SysTick_Delay(int delayms);
void RGB_init(void);
void TimerA0_PWM(int duty1, int duty2, int duty3);
int Read_Keypad(void);
int arrayToDuty(int code[]);

int dutyR, dutyG, dutyB;
int tempR, tempG, tempB;
int code[3];
int i, j;


enum states{                //enum for the switch states
    ENABLE,
    RED_ON,
    BLUE_ON,
    GREEN_ON,
    DISABLE
};


void main(void){
    int num;

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    RGB_init();

    enum states state = DISABLE;        //start with LED off

    while (1){

        switch(state){
            case DISABLE:
                tempR=dutyR;    tempG=dutyG;    tempB=dutyB;
                dutyR=0;    dutyG=0;    dutyB=0;
                
                TimerA0_PWM(dutyR, dutyG, dutyB);
                
                if(Read_Keypad() == 1){
                    SysTick_Delay(1000000);
                        if(Read_Keypad ==1)
                            state = RED_ON;
                }

                if(Read_Keypad() == 2){
                    SysTick_Delay(1000000);
                        if(Read_Keypad() ==2)
                            state = GREEN_ON;
                }
                if(Read_Keypad() == 3){
                    SysTick_Delay(1000000);
                        if(Read_Keypad() ==3)
                            state = BLUE_ON;
                }
                    break;

             case RED_ON:
             while(1){
                 num = Read_Keypad();

                        if ((num != 10) && (num < 13) && (num > 0)){                        //Continues if valid key pressed.
                            if (num != 12){                                                     //If the pound key is not pressed
                                if (i < 4){                                                         //the program stores the value in array.
                                    code[i] = num;                                              //If more that 4 digits are entered,
                                }                                                                   //the digits are shifted over
                                if (i > 3){                                                         //and the new value is stored.
                                    code[0] = code[1];
                                    code[1] = code[2];
                                    code[2] = code[3];
                                    code[3] = num;
                                    i = 3;
                                }
                                i++;
                            }
                        }
                        dutyR = arrayToDuty(code);
                        TimerA0_PWM(dutyR, dutyG, dutyB);
                        break;
                }

             case GREEN_ON:
             while(1){
                 num = Read_Keypad();

                        if ((num != 10) && (num < 13) && (num > 0)){                        //Continues if valid key pressed.
                            if (num != 12){                                                     //If the pound key is not pressed
                                if (i < 4){                                                         //the program stores the value in array.
                                    code[i] = num;                                              //If more that 4 digits are entered,
                                }                                                                   //the digits are shifted over
                                if (i > 3){                                                         //and the new value is stored.
                                    code[0] = code[1];
                                    code[1] = code[2];
                                    code[2] = code[3];
                                    code[3] = num;
                                    i = 3;
                                }
                                i++;
                            }
                        }
                        dutyG = arrayToDuty(code);
                        TimerA0_PWM(dutyR, dutyG, dutyB);
                        break;
                }

             case BLUE_ON:
             while(1){
                 num = Read_Keypad();

                        if ((num != 10) && (num < 13) && (num > 0)){                        //Continues if valid key pressed.
                            if (num != 12){                                                     //If the pound key is not pressed
                                if (i < 4){                                                         //the program stores the value in array.
                                    code[i] = num;                                              //If more that 4 digits are entered,
                                }                                                                   //the digits are shifted over
                                if (i > 3){                                                         //and the new value is stored.
                                    code[0] = code[1];
                                    code[1] = code[2];
                                    code[2] = code[3];
                                    code[3] = num;
                                    i = 3;
                                }
                                i++;
                            }
                        }
                        dutyB = arrayToDuty(code);
                        TimerA0_PWM(dutyR, dutyG, dutyB);
                        break;
                       }
                }
        }       
}


void b1pressed(void){                                 //button debouncing function for button 1
    //uint16_t x = 0;
    if (!(P1->IN & BIT7)){
        SysTick_Delay(15000);
        if (!(P1->IN & BIT7)&&((dutyR != 0)||(dutyG != 0)||(dutyB != 0))) {
            tempR=dutyR;    tempG=dutyG;    tempB=dutyB;
            dutyR=0;    dutyG=0;    dutyB=0;
            TimerA0_PWM(dutyR, dutyG, dutyB);
        }
        if (!(P1->IN & BIT7)&&((dutyR == 0)||(dutyG == 0)||(dutyB == 0))){
            dutyR = tempR;  dutyG = tempG;   dutyB = tempB;
            TimerA0_PWM(dutyR, dutyG, dutyB);
        }
    }
}

void SysTick_Delay(int delayms) {                       //delay function using system clock
    SysTick->LOAD = delayms-1;
    SysTick->VAL = 0;
    SysTick->CTRL |= BIT0;
    while (!(SysTick->CTRL & BIT(16)));
}

void RGB_init(void){
    P1->SEL0 &= ~(BIT7);      //GPIO
    P1->SEL1 &= ~(BIT7);
    P1->DIR &= ~(BIT7);       //set button to input
    P1->REN |= (BIT7);        //enable resistor
    P1->OUT |= (BIT7);        //use pull up resistor

    P4->SEL0 &= ~(BIT4|BIT5|BIT6);      //GPIO
    P4->SEL1 &= ~(BIT4|BIT5|BIT6);
    P4->DIR |= (BIT4|BIT5|BIT6);        //Set LED pins to Outputs
    P4->OUT |= (BIT4|BIT5|BIT6);       //Start with a low output
}

void TimerA0_PWM(int duty1, int duty2, int duty3){               //runs timer given the specified duty cycle
    TIMER_A0-> CTL = 0b1000010100;                      //Count up using smclk, clears TAOR register, /1
    TIMER_A0-> CCR[0] = 37500 - 1;                      //TimerA will count up to 63000-1

    if (duty1 == 0)
        TIMER_A0-> CCR[1] = 0;
    if (duty2 ==0)
        TIMER_A0-> CCR[2] = 0;
    if (duty3 == 0)
        TIMER_A0-> CCR[3] = 0;


    else {
        TIMER_A0-> CCR[1] = (37500 * duty1 / 100) - 1;
        TIMER_A0-> CCR[2] = (37500 * duty2 / 100) - 1;
        TIMER_A0-> CCR[3] = (37500 * duty3 / 100) - 1;
    }
    TIMER_A0-> CCTL[1] = 0b11100000;
    TIMER_A0-> CCTL[2] = 0b11100000;                    //reset/set mode
    TIMER_A0-> CCTL[3] = 0b11100000;
}

int Read_Keypad (void){                                                         //detects and reads key pressed
    int i, j, key = 0;
    for (i = 4; i < 7; i++){
        P4->DIR |= (0x01 << i);
        P4->OUT &= ~(0x01 << i);
        __delay_cycles(5000);
        for (j = 0; j < 4; j++){
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

int arrayToDuty(int code[]){
    int newDuty;

    newDuty = (code[0] * 100) + (code[1] * 10) + code[2];

    return newDuty;
}
