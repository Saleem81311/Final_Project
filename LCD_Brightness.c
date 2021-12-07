/*****************************************************************************************************************
 * Editors: Saleem Griggs-Taylor, Kevin Figurski, Tennison Hoffmann
 * Start Date: 11/4/2021
 * Last modified: 11/4/2021
 * Description: Uses ADC to input from potentiometer and output a ratio of that input as the duty cycle for PWM.
 *****************************************************************************************************************/

#include "msp.h"
#include <stdio.h>
#include <stdlib.h>
#include "Functions_Lib.h"

void delaysetup(void);
void delayms(uint16_t delay);
void ADC14_init (void);
void TimerA3_PWM(float duty);
void Pin_Init(void);
void ADC14_IRQHandler(void);

float nADC, duty = 100;

main(void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;   // stop watchdog timer

    Pin_Init();
    delaysetup ( );
    ADC14_init( );

    NVIC_EnableIRQ(ADC14_IRQn);
    __enable_interrupts();

    while(1){
        printf("hello\n");
        delayms(2000);
    }
}

void TimerA3_PWM(float duty){               //runs timer given the specified duty cycle
    TIMER_A3-> CTL = 0b1000010100;                      //Count up using smclk, clears TAOR register, /1
    TIMER_A3-> CCR[0] = 10000 - 1;                      //300 Hz
    if (duty < 3)
        TIMER_A3-> CCR[2] = 0;
    else
        TIMER_A3-> CCR[2] = (10000 * (duty / 100)) - 1;
    TIMER_A3-> CCTL[2] = 0b11100000;                    //reset/set
}

void ADC14_IRQHandler(void){
    if (ADC14->IFGR0 & BIT0){
        nADC = (ADC14->MEM[0] * 3.3) / 255;
        duty = nADC / 3.3;
        TimerA3_PWM((int)(duty * 100));
    }
}
