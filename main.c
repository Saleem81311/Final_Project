/*****************************************************************************************************************
 * Authors: Figurski, Griggs-Taylor, Hoffmann
 * Date: 12/9/2021
 * Description: Program to combine all aspects of control system together.
 *****************************************************************************************************************/
#include "msp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define RS 0x20        // P6.4 mask
#define RW 0x40        // Goes to GND since always writing to LCD
#define EN 0x80        // P6.5 mask

int Read_Keypad (void);                         //function declarations
void TimerA1_PWM(int dutyCycle);
void Pin_Init(void);
void Set_Motor_Speed (void);
void Door_Control (int pos);
void TimerA2_PWM(int position);
void ADC14_init (void);
void TimerA3_PWM(float duty);
void ADC14_IRQHandler(void);
void SysTick_Init(void);
void pushNibble(uint8_t nibble);
void pulseEnablePin(void);
void intializeDisplay(void);
void delay_micro(unsigned micro);
void delay_ms(unsigned ms);
void pushByte(uint8_t byte);
void commandWrite(uint8_t command);
void dataWrite(uint8_t data);
void printString(char string[]);

float nADC, duty = 100;

void main(void)
{
     WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer

     Pin_Init();                                //initialize the pins and the timer
     ADC14_init( );
     SysTick_Init();
     intializeDisplay();

     NVIC_EnableIRQ(PORT6_IRQn);  // set up to run code
     NVIC_EnableIRQ(ADC14_IRQn);
     __enable_interrupts();

     while (1){
         
     }
}

int Read_Keypad (void){ //detects and reads key pressed
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

void TimerA1_PWM(int dutyCycle){               //runs timer given the specified duty cycle
    TIMER_A1-> CTL = 0b1001010100;                      //Count up using smclk, clears TAOR register, /2
    TIMER_A1-> CCR[0] = 37500 - 1;                      //TimerA will count up to 37500-1
    if (duty == 0)
        TIMER_A1-> CCR[4] = 0;
    else
        TIMER_A1-> CCR[4] = (37500 * dutyCycle / 100) - 1;   //Sets the duty cycle.
    TIMER_A1-> CCTL[4] = 0b11100000;                    //reset/set mode
}

void Set_Motor_Speed (void){
    int num = 0, speed = 0;
    num = Read_Keypad();                   //get key press from the keypad
    if ((num != 10) && (num != 12) && (num > 0) && (num < 13)){
        if (num == 11)
            num = 0;
        speed = num * 10;
        printf("Speed Entered: %d\n", num);    //confirms speed
        TimerA1_PWM(speed);                        //changes timer to new PWM
    }
}

void PORT6_IRQHandler(void){
    if(P6->IFG & BIT4){
        __delay_cycles(15000);
        if (!(P6->IN & BIT0)){
            printf("Emergency Stop!\n");
            TimerA1_PWM(0);
            P6->IFG  &= ~BIT0;  //clear the flag
        }
    }
}
}
