/*****************************************************************************************************************
 * Authors: Figurski, Griggs-Taylor, Hoffmann
 * Date: 12/9/2021
 * Description: Program to combine all aspects of control system together.
 *****************************************************************************************************************/
#include "msp.h"
#include "Functions_Lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
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

     while(1)
      {
         
      }

}
