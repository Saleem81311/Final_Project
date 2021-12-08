#include "msp.h"
#include <stdio.h>
#include <stdlib.h>

void RGB_init(void);
void TimerA0_PWM(int duty1, int duty2, int duty3);
int Read_Keypad(void);
int arrayToDuty(int code[]);
void PORT1_IRQHandler(void);

int dutyR = 100, dutyG = 100, dutyB = 100;
int tempR = 0, tempG = 0, tempB = 0;

enum states{                //enum for the switch states
    DEFAULT,
    RED_ON, //red menu
    BLUE_ON, //blue menu
    GREEN_ON //green menu
};

void main(void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    int num, i = 0;
    int code[3];
    enum states state = DEFAULT;

    TimerA0_PWM(dutyR,dutyG,dutyB);
    RGB_init();
    NVIC_EnableIRQ(PORT1_IRQn);
    __enable_interrupts();

    while (1){
        switch(state){
             case RED_ON:
                 i = 0;
                 printf("\nRed - Enter 3 digit duty cycle\n");
                 while(1){
                    num = Read_Keypad();

                    if ((num != 10) && (num < 12) && (num > 0)){                        //Continues if valid key pressed.
                        if (num == 11)
                            num = 0;
                        code[i] = num;             //the program stores the value in array.
                        i++;
                        if (i == 3)
                            break;
                        }
                    }

                dutyR = arrayToDuty(code);
                TimerA0_PWM(dutyR, dutyG, dutyB);
                state = DEFAULT;
                break;

             case GREEN_ON:
                 i = 0;
                 printf("\nGreen - Enter 3 digit duty cycle\n");
                 while(1){
                    num = Read_Keypad();

                    if ((num != 10) && (num < 12) && (num > 0)){                        //Continues if valid key pressed.
                        if (num == 11)
                            num = 0;
                        code[i] = num;             //the program stores the value in array.
                        i++;
                        if (i == 3)
                            break;
                        }
                    }

                dutyG = arrayToDuty(code);
                TimerA0_PWM(dutyR, dutyG, dutyB);
                state = DEFAULT;
                break;

             case BLUE_ON:
                 i = 0;
                 printf("\nBlue - Enter 3 digit duty cycle\n");
                 while(1){
                    num = Read_Keypad();

                    if ((num != 10) && (num < 12) && (num > 0)){                        //Continues if valid key pressed.
                        if (num == 11)
                            num = 0;
                        code[i] = num;             //the program stores the value in array.
                        i++;
                        if (i == 3)
                            break;
                    }
                 }


                dutyB = arrayToDuty(code);
                TimerA0_PWM(dutyR, dutyG, dutyB);
                state = DEFAULT;
                break;

             case DEFAULT:
                 printf("\nPress 1 for Red, 2 for Green, or 3 for blue\n");
                 while (1) {
                     num = Read_Keypad();
                     if ((num != 10) && (num < 12) && (num > 0)){                        //Continues if valid key pressed.
                         if (num == 11)
                             num = 0;
                         if(num == 1){
                             state = RED_ON;
                             break;
                         }
                         else if(num == 2){
                             state = GREEN_ON;
                             break;
                         }
                         else if(num == 3){
                             state = BLUE_ON;
                             break;
                         }
                         else
                             continue;
                     }
                 }

                 break;

             default:
                 state = DEFAULT;
        }
    }
}

void RGB_init(void){
    P1-> SEL0 &=~ BIT7;
    P1-> SEL1 &=~ BIT7;
    P1-> DIR &=~ BIT7;
    P1-> REN |= BIT7; //pullup resistor
    P1-> OUT |= BIT7;
    P1-> IE |= BIT7; //enable interupts for P1.1
    P1-> IES |= BIT7; // Set pin to interrupt to trigger when it goes high->low
    P1-> IFG &= ~BIT7; //clear P1 interrupt flags

    P2->SEL0 |= (BIT4|BIT5|BIT6);      //GPIO
    P2->SEL1 &= ~(BIT4|BIT5|BIT6);
    P2->DIR |= (BIT4|BIT5|BIT6);        //Set LED pins to Outputs
    P2->OUT &= ~(BIT4|BIT5|BIT6);       //Start with a low output

    P4->SEL0 &= ~(0x7F);                                                        //initialize all pins to GPIO
    P4->SEL1 &= ~(0x7F);                                                        //as inputs with pull up resistors
    P4->DIR &= ~(0x7F);
    P4->REN |= 0x7F;
    P4->OUT |= 0x7F;

}

void TimerA0_PWM(int duty1, int duty2, int duty3){               //runs timer given the specified duty cycle
    TIMER_A0-> CTL = 0b1000010100;                      //Count up using smclk, clears TAOR register, /1
    TIMER_A0-> CCR[0] = 50000 - 1;                      //TimerA will count up to 63000-1

    if (duty1 == 0)
        TIMER_A0-> CCR[1] = 0;
    else
        TIMER_A0-> CCR[1] = (50000 * duty1 / 100) - 1;

    if (duty2 ==0)
        TIMER_A0-> CCR[2] = 0;
    else
        TIMER_A0-> CCR[2] = (50000 * duty2 / 100) - 1;

    if (duty3 == 0)
        TIMER_A0-> CCR[3] = 0;
    else
        TIMER_A0-> CCR[3] = (50000 * duty3 / 100) - 1;

    TIMER_A0-> CCTL[1] = 0b11100000;
    TIMER_A0-> CCTL[2] = 0b11100000;                    //reset/set mode
    TIMER_A0-> CCTL[3] = 0b11100000;
}

int Read_Keypad (void){                                                         //detects and reads key pressed
    int i, j, key = 0;
    for (i = 4; i < 7; i++){
        P4->DIR |= (0x01 << i);
        P4->OUT &= ~(0x01 << i);
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

void PORT1_IRQHandler(void){
    if(P1->IFG & BIT7){
        while(!(P1->IN & BIT7));
        if (dutyR + dutyG + dutyB){
            printf("Disabled\n");
            tempR=dutyR;    tempG=dutyG;    tempB=dutyB;
            dutyR=0;    dutyG=0;    dutyB=0;
            TimerA0_PWM(0,0,0);
        }
        else{
            printf("Enabled\n");
            dutyR = tempR;  dutyG=tempG;    dutyB=tempB;
            TimerA0_PWM(dutyR, dutyG, dutyB);
        }
    P1->IFG = 0;
    }
}
