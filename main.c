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
float nADC;
float duty = 100;
int dutyR = 100, dutyG = 100, dutyB = 100;
int tempR = 0, tempG = 0, tempB = 0;
int arrayToDuty(int code[]);
void PORT1_IRQHandler(void);
void RGB_init(void);
void TimerA0_PWM(int duty1, int duty2, int duty3);
//int Read_Keypad(void);

void ADC14_IRQHandler(void);
void ADC14_init (void);
void Door_Control (int pos);
char EStop_Function(char Operation_State);   // This is the EStop function to stop everything in case of an emergency
int Read_Keypad (void);      //detects and reads key pressed
void LCD_Command(unsigned char command);
void LCD_Data (unsigned char data);
void LCD_Function(int Menu_Switch);
void Message(char Menu[]);
void Set_Motor_Speed(void);
void PORT6_IRQHandler(void);
void Pin_Init();     // Pin initializations
void SysTick_Delay(uint16_t delay);
void SysTick_Init();
void TimerA1_PWM(int dutyCycle);
void TimerA2_PWM(int position);
void TimerA3_PWM(float duty);
void pushNibble(uint8_t nibble);
void pulseEnablePin();
void intializeDisplay();
void delay_micro(unsigned micro);
void delay_ms(unsigned ms);
void pushByte(uint8_t byte);
void commandWrite(uint8_t command);
void dataWrite(uint8_t data);
void printString(char string[]);

enum states     //enum for the switch states
{
    DEFAULT_LIGHTS,
    RED_ON, //red menu
    BLUE_ON, //blue menu
    GREEN_ON, //green menu
    LCD_MENU,
    LIGHTS,
    DOORS,
    MOTOR

};

void main(void)
{

     WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer
     Pin_Init();                                //initialize the pins and the timer
     ADC14_init();
     SysTick_Init();
     TimerA0_PWM(dutyR,dutyG,dutyB);
     RGB_init();
     intializeDisplay();
     int num, i = 0;
     int code[3];
     enum states state = LCD_MENU;

     NVIC_EnableIRQ(PORT6_IRQn);        // Enabling PORT6 IRQ interrupt
     NVIC_EnableIRQ(ADC14_IRQn);        // Enabling ADC14 IRQ interrupt
     NVIC_EnableIRQ(PORT1_IRQn);        // Enabling PORT1 IRQ interrupt for RGB LEDs
     __enable_interrupts();

         //TimerA1_PWM(100);
     while(1)
      {
        switch(state)
          {
            case LCD_MENU:     // Display menu case
               commandWrite(1);
               printString("Hello");
               break;

            case LIGHTS:
               printString("Lights are active");
               state = DEFAULT_LIGHTS;
               break;

            case DOORS:
                printString("Opening doors..");
                break;

            case RED_ON:      // RED LED toggle case
               i = 0;
               printf("\nRed - Enter 3 digit duty cycle\n");
               while(1)
                 {
                   num = Read_Keypad();
                   if((num != 10) && (num < 12) && (num > 0))        //Continues if valid key pressed.
                     {
                       if(num == 11)
                          num = 0;
                          code[i] = num;             //the program stores the value in array.
                          i++;

                       if(i == 3)
                         break;
                     }
                  }

                  dutyR = arrayToDuty(code);
                  TimerA0_PWM(dutyR, dutyG, dutyB);
                  state = DEFAULT_LIGHTS;
                  break;

              case GREEN_ON:      // GREEN LED toggle case
                 i = 0;
                 printf("\nGreen - Enter 3 digit duty cycle\n");
                 while(1)
                 {
                   num = Read_Keypad();

                   if((num != 10) && (num < 12) && (num > 0))        //Continues if valid key pressed.
                     {
                       if(num == 11)
                          num = 0;
                          code[i] = num;             //the program stores the value in array.
                          i++;
                       if(i == 3)
                          break;
                     }
                  }

                    dutyG = arrayToDuty(code);
                    TimerA0_PWM(dutyR, dutyG, dutyB);
                    state = DEFAULT_LIGHTS;
                    break;

                case BLUE_ON:     // BLUE LED toggle case
                   i = 0;
                   printf("\nBlue - Enter 3 digit duty cycle\n");
                   while(1)
                       {
                         num = Read_Keypad();

                         if((num != 10) && (num < 12) && (num > 0))        //Continues if valid key pressed.
                           {
                             if(num == 11)
                                num = 0;
                                code[i] = num;             //the program stores the value in array.
                                i++;
                             if(i == 3)
                              break;
                            }
                        }

                      dutyB = arrayToDuty(code);
                      TimerA0_PWM(dutyR, dutyG, dutyB);
                      state = DEFAULT_LIGHTS;
                      break;

                  case DEFAULT_LIGHTS:     // Default RGB case
                     printf("\nPress 1 for Red, 2 for Green, or 3 for blue\n");
                     while(1)
                        {
                          num = Read_Keypad();
                          if((num != 10) && (num < 12) && (num > 0))     //Continues if valid key pressed.
                            {
                              if(num == 11)
                                 num = 0;
                              if(num == 1)
                                {
                                  state = RED_ON;
                                  break;
                                }
                              else if(num == 2)
                                 {
                                   state = GREEN_ON;
                                   break;
                                 }
                              else if(num == 3)
                                 {
                                   state = BLUE_ON;
                                   break;
                                 }
                              else
                                  continue;
                           }
                         }

                         break;

                   default:
                      state = LCD_MENU;
                      break;
             };
         //Set_Motor_Speed();
        /* Door_Control(1);
         __delay_cycles(3000000);
         Door_Control(0);
         __delay_cycles(3000000);   */

      }
}

void ADC14_IRQHandler(void)     // ADC14 IRQ Handler for LCD backlight
{
  if(ADC14->IFGR0 & BIT0)
    {
      nADC = (ADC14->MEM[0] * 3.3) / 255;
      duty = nADC / 3.3;
      TimerA3_PWM((int)(duty * 100));
    }
}

void PORT6_IRQHandler(void)     // PORT6 IRQ Handler for motor
{
   if(P6->IFG & BIT0)
    {
      __delay_cycles(15000);
        if(!(P6->IN & BIT0))       // if statement for checking if the button is pressed or not
        {
          printf("Emergency Stop!\n");
          TimerA1_PWM(0);
          P6->IFG &= ~BIT0;     //clearing the interrupt flag
        }
    }
}

void RGB_init(void)
{
    P1-> SEL0 &=~ BIT7;
    P1-> SEL1 &=~ BIT7;
    P1-> DIR &=~ BIT7;
    P1-> REN |= BIT7; //pullup resistor
    P1-> OUT |= BIT7;
    P1-> IE |= BIT7; //enable interupts for P1.7
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

void TimerA0_PWM(int duty1, int duty2, int duty3)       //runs timer given the specified duty cycle
{
    TIMER_A0-> CTL = 0b1000010100;                      //Count up using smclk, clears TAOR register, /1
    TIMER_A0-> CCR[0] = 50000 - 1;                      //TimerA will count up to 63000-1

    if(duty1 == 0)
        TIMER_A0-> CCR[1] = 0;
    else
        TIMER_A0-> CCR[1] = (50000 * duty1 / 100) - 1;

    if(duty2 ==0)
        TIMER_A0-> CCR[2] = 0;
    else
        TIMER_A0-> CCR[2] = (50000 * duty2 / 100) - 1;

    if(duty3 == 0)
        TIMER_A0-> CCR[3] = 0;
    else
        TIMER_A0-> CCR[3] = (50000 * duty3 / 100) - 1;

    TIMER_A0-> CCTL[1] = 0b11100000;
    TIMER_A0-> CCTL[2] = 0b11100000;                    //reset/set mode
    TIMER_A0-> CCTL[3] = 0b11100000;
}

int Read_Keypad (void)      //detects and reads key pressed
{
    int i, j, key = 0;
    for (i = 4; i < 7; i++)
        {
          P5->DIR |= (0x01 << i);
          P5->OUT &= ~(0x01 << i);
          for(j = 0; j < 4; j++)
             {
               if(!(P5->IN & BIT(j)))
                 {
                   while (!(P5->IN & BIT(j)));
                   if(j == 0)
                   key = i - 3;
                   else if(j == 1)
                   key = i;
                   else if(j == 2)
                   key = i + 3;
                   else
                   key = i + 6;
                  }
              }
          P5->DIR &= ~(0x7F);
          P5->OUT |= 0x7F;
        }
        return key;
}

int arrayToDuty(int code[])
{
    int newDuty;

    newDuty = (code[0] * 100) + (code[1] * 10) + code[2];

    return newDuty;
}

void PORT1_IRQHandler(void)
{
    if(P1->IFG & BIT7)
      {
        while(!(P1->IN & BIT7));
        if(dutyR + dutyG + dutyB)
          {
            printf("Disabled\n");
            tempR=dutyR;    tempG=dutyG;    tempB=dutyB;
            dutyR=0;    dutyG=0;    dutyB=0;
            TimerA0_PWM(0,0,0);
          }
     else
       {
         printf("Enabled\n");
         dutyR = tempR;  dutyG=tempG;    dutyB=tempB;
         TimerA0_PWM(dutyR, dutyG, dutyB);
       }
         P1->IFG = 0;
      }
}

void Set_Motor_Speed(void)
{
  int num = 0, speed = 100;
  TimerA1_PWM(speed);
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

void Pin_Init()     // Pin initializations
{
  /****************************************************************** RGB Estop Button **************************************************************/
    P1-> SEL0 &=~ BIT7;                                         //emergency stop
    P1-> SEL1 &=~ BIT7;
    P1-> DIR &=~ BIT7;
    P1-> REN |= BIT7; //pullup resistor
    P1-> OUT |= BIT7;
    P1-> IE |= BIT7; //enable interupts for P6.4
    P1-> IES |= BIT7; // Set pin to interrupt to trigger when it goes high->low
    P1-> IFG &=~ BIT7; //clear P1 interrupt flags

  /**************************************************************** RGB Initializations *************************************************************/

    P2-> SEL0 |= (BIT4|BIT5|BIT6);                                  //motor
    P2-> SEL1 &= ~(BIT4|BIT5|BIT6);
    P2-> DIR |= (BIT4|BIT5|BIT6);                                 //sets port 2 for timerA0
    P2-> OUT &= ~(BIT4|BIT5|BIT6);                                //off to start

  /**************************************************************** RG LEDs for doors****************************************************************/
    P3-> SEL0 &= ~(BIT6|BIT7);                                  //LEDs
    P3-> SEL1 &= ~(BIT6|BIT7);
    P3-> DIR |= (BIT6|BIT7);                                 //sets LEDs for GPIO as an output
    P3-> OUT &= ~(BIT6|BIT7);

  /***************************************************************** Motor Initializations **********************************************************/
    P7-> SEL0 |= BIT7;                                  //motor
    P7-> SEL1 &=~ BIT7;
    P7-> DIR |= BIT7;                                 //sets pin 7.4 as timerA1.4
    P7-> OUT &= ~BIT7;                                //off to start

  /****************************************************************** Keypad Initializations ********************************************************/
    P5-> SEL0 &= ~(0x7F);                               //keypad
    P5-> SEL1 &= ~(0x7F);                               //initialize all pins to GPIO
    P5-> DIR &= ~(0x7F);                                //as inputs with pull up resistors
    P5-> REN |= 0x7F;
    P5-> OUT |= 0x7F;

  /*************************************************************** Motor Estop **********************************************************************/
    P6-> SEL0 &=~ BIT0;                                         //emergency stop
    P6-> SEL1 &=~ BIT0;
    P6-> DIR &=~ BIT0;
    P6-> REN |= BIT0; //pullup resistor
    P6-> OUT |= BIT0;
    P6-> IE |= BIT0; //enable interupts for P6.0
    P6-> IES |= BIT0; // Set pin to interrupt to trigger when it goes high->low
    P6-> IFG &=~ BIT0; //clear P1 interrupt flags

  /************************************************************* Door servo initializations ********************************************************/
    P6-> SEL0 |= (BIT6|BIT7);                                   //servos 1 and 2
    P6-> SEL1 &=~ (BIT6|BIT7);
    P6-> DIR |= (BIT6|BIT7);                                 //sets pin 6.6 and 6.7 as timerA2
    P6-> OUT &= ~(BIT6|BIT7);                                //off to start

  /************************************************************* LCD Brightness with potentiometer *************************************************/
    P6-> SEL0 |= BIT1;                     //potentiometer input
    P6-> SEL1 |= BIT1;                       // configure pin 6.1 for A14 input
    P6-> DIR  &= ~BIT1;                      //Analog INPUT

  /************************************************************* LCD Backlight PWM *****************************************************************/
    P8-> SEL0 |= BIT2;                              //lcd backlight PWM output
    P8-> SEL1 &=~ BIT2;
    P8-> DIR |= BIT2;                                 //sets pin 8.2 as timerA3.2

  /******************************************************* LCD initializations **********************************************************************/
    P4->SEL0 &= ~(0xFF);            //P4.1-P4.7 GPIO, Outputs, default of LOW
    P4->SEL1 &= ~(0xFF);
    P4->DIR  |=  (0xFF);
    P4->OUT  &= ~(0xFF);

}

void TimerA1_PWM(int dutyCycle)     //runs timer given the specified duty cycle
{
  TIMER_A1-> CTL = 0b1001010100;                      //Count up using smclk, clears TAOR register, /2
  TIMER_A1-> CCR[0] = 37500 - 1;                      //TimerA will count up to 37500-1
    if (dutyCycle == 0)
        TIMER_A1-> CCR[1] = 0;
  else
  TIMER_A1-> CCR[1] = (37500 * dutyCycle / 100) - 1;   //Sets the duty cycle.
  TIMER_A1-> CCTL[1] = 0b11100000;                    //reset/set mode
}

void TimerA2_PWM(int position)
{
  TIMER_A2-> CTL = 0b1000010100;                      //Count up using smclk, clears TAOR register, /1
  TIMER_A2-> CCR[0] = 63000 - 1;                      //TimerA will count up to 63000-1
   if (position == 0)
   {
      TIMER_A2-> CCR[4] = 2700 - 1;
      TIMER_A2-> CCR[3] = 2700 - 1;
      TIMER_A2-> CCR[2] = 6300 - 1;
   }

   if (position == 1)
   {
      TIMER_A2-> CCR[4] = 6300 - 1;
      TIMER_A2-> CCR[3] = 6300 - 1;
      TIMER_A2-> CCR[2] = 2700 - 1;
   }
  TIMER_A2-> CCTL[4] = 0b11100000;
  TIMER_A2-> CCTL[3] = 0b11100000;                    //reset/set mode
  TIMER_A2-> CCTL[2] = 0b11100000;
}

void TimerA3_PWM(float duty)
{
  TIMER_A3-> CTL = 0b1000010100;                      //Count up using smclk, clears TAOR register, /1
  TIMER_A3-> CCR[0] = 10000 - 1;                      //300 Hz
    if (duty < 3)
        TIMER_A3-> CCR[2] = 0;
    else
        TIMER_A3-> CCR[2] = (10000 * (duty / 100)) - 1;
  TIMER_A3-> CCTL[2] = 0b11100000;                    //reset/set
}

void Door_Control (int pos)
{
  if (pos == 1)
  {
    P3->OUT &= ~BIT6;
    P3->OUT |= BIT7;
    TimerA2_PWM(1);
  }
  if (pos == 0)
  {
    P3->OUT &= ~BIT7;
    P3->OUT |= BIT6;
    TimerA2_PWM(0);
  }
}

void ADC14_init(void)
{
  ADC14->CTL0 &=~ 0x00000002;          // disable ADC14ENC during configuration
  ADC14->CTL0 = 0b11000101111001000111011110010000;
  ADC14->CTL1 = 000000000000000110000;
  ADC14->MCTL[0] = 14 | BIT7;
  ADC14->CTL0 |= BIT1;
  ADC14->CTL0 |= BIT0;
  ADC14->IER0 |= BIT0;
}

void SysTick_Init()
{
  SysTick->CTRL = 0;
  SysTick->LOAD = 0x00FFFFFF;
  SysTick->VAL = 32;
  SysTick->CTRL = 0x00000005;
}
/*
void SysTick_Delay(uint16_t delay)
{
    SysTick->LOAD = (delay*3000);
    SysTick->VAL = 5;
    SysTick->CTRL = 1;
    while((SysTick->CTRL & 0x00010000) == 0);
}   */

void intializeDisplay()             //function to initialize lcd
{
    commandWrite(3);
    delay_ms(10);
    commandWrite(3);
    delay_micro(200);
    commandWrite(3);
    delay_ms(10);

    commandWrite(2);
    delay_micro(100);
    commandWrite(2);
    delay_micro(100);

    commandWrite(8);
    delay_micro(100);
    commandWrite(0x0F);
    delay_micro(100);
    commandWrite(1);
    delay_micro(100);
    commandWrite(6);
    delay_ms(10);
}

void pulseEnablePin()                   //pulses enable pin
{
    P4->OUT |= BIT2;
    delay_ms(10);
    P4->OUT &= ~BIT2;
}

void delay_micro(unsigned micro)        //delays desired time in us
{
    SysTick->LOAD = micro*3 - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = 5;
    while(!(SysTick->CTRL & BIT(16)));
}

void delay_ms(unsigned ms)      //delays desired time in ms
{
    SysTick->LOAD = ms*3000 - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = 5;
    while(!(SysTick->CTRL & BIT(16)));
}

void pushNibble(uint8_t nibble)         //pushes 4 bits onto pins
{
    P4->OUT &= (0x0F);
    P4->OUT |= ((nibble & 0x0F) << 4);
    pulseEnablePin();
}

void pushByte(uint8_t byte)     //splits byte into 2 nibbles and sends to push nibble
{
    pushNibble(byte >> 4);
    pushNibble(byte & (0x0F));
}

void commandWrite(uint8_t command)      //sends byte of command to lcd
{
    P4->OUT &= ~BIT3;
    delay_ms(10);
    pushByte(command);
}

void dataWrite(uint8_t data)        //sends byte of data to lcd
{
    P4->OUT |= BIT3;
    delay_ms(10);
    pushByte(data);
}

void printString(char string[])     //prints a string of characters
{
    int i;
    for(i = 0; i < (strlen(string)); i++)
      {
        dataWrite(string[i]);
      }
}
