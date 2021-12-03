/*****************************************************************************************************************
 * Editors: Saleem Griggs-Taylor, Kevin Figurski, Tennison Hoffmann
 * Start Date: 11/4/2021
 * Last modified: 11/4/2021
 * Description: Uses ADC to input from potentiometer and output a ratio of that input as the duty cycle for PWM.
 *****************************************************************************************************************/

#include "msp.h"
#include <stdio.h>
#include <stdlib.h>

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

void Pin_Init(void){
    P6->SEL0 |= BIT1;                     // configure pin 6.1 for A14 input
    P6->SEL1 |= BIT1;
    P6->DIR  &= ~BIT1;                      //Analog INPUT

    P8-> SEL0 |= BIT2;
    P8-> SEL1 &=~ BIT2;
    P8-> DIR |= BIT2;                                 //sets pin 8.2 as timerA3.2
}

void delaysetup (void)                  // sets up the countdown timer
{
    SysTick-> CTRL = 0;
    SysTick-> LOAD = 0x00FFFFFF;
    SysTick-> VAL    = 0;
    SysTick-> CTRL = 0x00000005;
}

void delayms(uint16_t delay)                // timer function set to run a 1 ms increments
{
    SysTick->LOAD = ((delay * 3000)-1);
    SysTick->VAL = 0;
    while((SysTick-> CTRL & 0x00010000) == 0);
}

void ADC14_init (void)
{
    /*ADC14->CTL0 &=~ 0x00000002;          // disable ADC14ENC during configuration
    ADC14->CTL0 |=   0x04400110;          // S/H pulse mode, SMCLK, 16 sample clocks
    ADC14->CTL1 =   0x00000030;          // 14 bit resolution
    ADC14->CTL1 |=   0;          // Selecting ADC14CSTARTADDx mem0 REGISTER
    ADC14->MCTL[0] = 0;            // ADC14INCHx = 0 for mem[0]
    ADC14->CTL0 |= 0x00000002;          // enable ADC14ENC, starts the ADC after configuration*/
    ADC14->CTL0 &=~ 0x00000002;          // disable ADC14ENC during configuration
    ADC14->CTL0 = 0b11000101111001000111011110010000;
    ADC14->CTL1 = 000000000000000110000;
    ADC14->MCTL[0] = 14 | BIT7;
    ADC14->CTL0 |= BIT1;
    ADC14->CTL0 |= BIT0;
    ADC14->IER0 |= BIT0;
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