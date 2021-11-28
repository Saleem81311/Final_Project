#include "Functions_Lib.h"
char EStop_Function(char EStop_State)   // This is the EStop function to stop everything in case of an emergency
{
 void Pin_Init();        // Custom function defining
void SysTick_Init();        // Custom function defining
void SysTick_Delay(uint16_t delay);     // Custom function defining

void main()
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    char Operation_State = 'A';     // Initializing the first start of the system to be in the Active state
    int count1 = 3;     // Beginning the system count down value at 3 for when the system goes into the count down state
    int count2 = 0;
    int i;
    Pin_Init();     // Calling the pin initializations
    SysTick_Init();     // Calling the SysTick initializations

    while(1)
    {
        if(!(P1->IN & BIT6))        // Checking to see if the Estop is pressed
          {
            Operation_State = 'E';      // Sending the state of the button to ther Estop state
            count2 = 1;     // Setting the count2 to 1 for going into the count state before the system resumes operation
          }
        else
          {
            if(count2 == 1)     // If the count2 value is 1 it sends the current state to the count
              Operation_State = 'C';
          }
         SysTick_Delay(50);     // Estop Button debouncing

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
              count2 = 0;       // Setting count2 back to 0 for the next time the Estop is pressed to come back into the count down state first
              count1 = 3;       // Resetting the count timer for the system in the count down state
              P2->OUT &= ~0x38;     // Turning off all LEDs on their respective BIT locations
              Operation_State = 'A';        // Setting the system back into active operation
              break;
        }

    }

}

void Pin_Init()     // Pin initializations
{
/*********************************************************** LEDs mimicking a running system ***********************************************************/
    P2->SEL0 &= ~0x38;
    P2->SEL1 &= ~0x38;
    P2->DIR |= 0x38;
    P2->OUT &= ~0x38;

/********************************************************************* Estop Button ********************************************************************/
    P1->SEL0 &= ~BIT6;
    P1->SEL1 &= ~BIT6;
    P1->DIR &= BIT6;
    P1->REN |= BIT6;
    P1->OUT |= BIT6;
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
