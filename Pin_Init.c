#include "msp.h"
#include "Functions_Lib.h"

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
    P7-> SEL0 |= BIT4;                                  //motor
    P7-> SEL1 &=~ BIT4;
    P7-> DIR |= BIT4;                                 //sets pin 7.4 as timerA1.4
    P7-> OUT &= ~BIT4;                                //off to start

  /****************************************************************** Keypad Initializations ********************************************************/
    P4-> SEL0 &= ~(0x7F);                               //keypad
    P4-> SEL1 &= ~(0x7F);                               //initialize all pins to GPIO
    P4-> DIR &= ~(0x7F);                                //as inputs with pull up resistors
    P4-> REN |= 0x7F;
    P4-> OUT |= 0x7F;

  /*************************************************************** Motor Estop **********************************************************************/
    P6-> SEL0 &=~ BIT0;                                         //emergency stop
    P6-> SEL1 &=~ BIT0;
    P6-> DIR &=~ BIT0;
    P6-> REN |= BIT0; //pullup resistor
    P6-> OUT |= BIT0;
    P6-> IE |= BIT0; //enable interupts for P6.4
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
    
  /************************************************************* LCD Initializations **************************************************************/
    P6->DIR |= (RS|EN); // Make P6 pins output for control
    P9->DIR = 0xFF;     // Make P9 pins output for data

    SysTick_Delay(30);      // Initialization sequence
    LCD_Command(0x30);
    SysTick_Delay(10);
    LCD_Command(0x30);
    SysTick_Delay(1);
    LCD_Command(0x30);
    LCD_Command(0x38);      // Set 8-bit data, 2-line, 5x7 font
    LCD_Command(0x06);      // Move cursor right after each char
    LCD_Command(0x01);      // Clear screen, Move cursor to home
    LCD_Command(0x0F);      // Turn on display, Cursor blink
}
