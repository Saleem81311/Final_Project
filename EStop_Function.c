#include "Functions_Lib.h"
#include "msp.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#define Estop 0x40     // Estop button BIT on Port 1
#define Reset 0x80     // Reset button BIT on Port 1

char EStop_Function(char Operation_State)   // This is the EStop function to stop everything in case of an emergency
{
  void Pin_Init();        // Custom function defining
  void SysTick_Init();        // Custom function defining
  void SysTick_Delay(uint16_t delay);     // Custom function defining

volatile char Operation_State;
volatile int count1;     // Beginning the system count down value at 3 for when the system goes into the count down state
void main()
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    Operation_State = 'A';     // Initializing the first start of the system to be in the Active state

    int i;
    Pin_Init();     // Calling the pin initializations
    SysTick_Init();     // Calling the SysTick initializations
    count1 = 3;

    NVIC->ISER[1]=1<<((PORT1_IRQn) & 31);
    __enable_interrupt();

    while(1)
    {
        switch(Operation_State)     // System operation state
        {
          case 'A':     // System Active State

              printf("System is active.\n");
              P2->OUT |= BIT3;      // Turning on the Orange LED
              SysTick_Delay(500);       // Delay time for the LED to be lit in ms
              P2->OUT &= ~0x38;     // Turning off all LEDs on their respective BIT locations
              P2->OUT |= BIT4;      // Turning on the Yellow LED
              SysTick_Delay(500);   // Delay time for the LED to be lit in ms
              P2->OUT &= ~0x38;     // Turning off all LEDs on their respective BIT locations
              break;

          case 'E':     // Estop activation state

              printf("Estop is active.\n");
              P2->OUT |= BIT5;      // Turning on the Red LED to signify Estop is active and the system is no longer running
              break;

          case 'C':     // Count down state before the system resumes work

              printf("System continues in: \n");
              for(i = 0; i < 3; i++)        // Count down loop
                 {
                   printf("%d\n",count1);       // Displaying the count number
                   count1--;        // Counting down
                   SysTick_Delay(1000);     // Delay time for the count down in ms
                 }
              count1 = 3;       // Resetting the count timer for the system in the count down state
              P2->OUT &= ~0x38;     // Turning off all LEDs on their respective BIT locations
              Operation_State = 'A';        // Setting the system back into active operation
              break;
        }

    }

}

void Pin_Init()     // Pin initializations
{
/**************************************************************** LEDs mimicking a running system ****************************************************************/
    P2->SEL0 &= ~0x38;
    P2->SEL1 &= ~0x38;
    P2->DIR |= 0x38;
    P2->OUT &= ~0x38;

/********************************************************************* Estop and Reset Button ********************************************************************/
    P1->SEL0 &= ~(Estop|Reset);
    P1->SEL1 &= ~(Estop|Reset);
    P1->DIR &= ~(Estop|Reset);
    P1->REN |= (Estop|Reset);
    P1->OUT |= (Estop|Reset);
    P1->IES |= (Estop|Reset);
    P1->IE  |= (Estop|Reset);
    P1->IFG = 0;
}

void PORT1_IRQHandler()
{
    if(P1->IFG & Estop)     // If statement for looking for the Estop interrupt then sending it to Estop state
    {
        Operation_State = 'E';
    }
    SysTick_Delay(100);

    if(P1->IFG & Reset)     // If statement for looking for the Reset interrupt then sending it to count down state
    {
        Operation_State = 'C';
    }
    SysTick_Delay(100);
    P1->IFG = 0;
}

void SysTick_Init()     // SysTick Initializations
{
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->CTRL = 0;
    SysTick->VAL = 32;
    SysTick->CTRL = 0x00000005;
}

void SysTick_Delay(uint16_t delay)      // SysTick calculations for the delay time in ms
{
    SysTick->LOAD = (delay*3000);
    SysTick->VAL = 5;
    while((SysTick->CTRL & 0x00010000) == 0);
}

}
