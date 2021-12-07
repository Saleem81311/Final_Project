/*****************************************************************************************************************
 * Authors: Figurski, Griggs-Taylor, Hoffmann
 * Date: 12/9/2021
 * Description: Program to combine all aspects of control system together.
 *****************************************************************************************************************/
#include "msp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void Door_Control (int pos){
    if (pos == 1){
        P3->OUT &= ~BIT6;
        P3->OUT |= BIT7;
        TimerA2_PWM(1);
    }
    if (pos == 0){
        P3->OUT &= ~BIT7;
        P3->OUT |= BIT6;
        TimerA2_PWM(0);
    }
}

void TimerA2_PWM(int position){               //runs timer given the specified duty cycle
    TIMER_A2-> CTL = 0b1000010100;                      //Count up using smclk, clears TAOR register, /1
    TIMER_A2-> CCR[0] = 63000 - 1;                      //TimerA will count up to 63000-1
    if (position == 0){
        TIMER_A2-> CCR[4] = 2700 - 1;
        TIMER_A2-> CCR[3] = 2700 - 1;
        TIMER_A2-> CCR[2] = 6300 - 1;

    }
    if (position == 1){
        TIMER_A2-> CCR[4] = 6300 - 1;
        TIMER_A2-> CCR[3] = 6300 - 1;
        TIMER_A2-> CCR[2] = 2700 - 1;
    }
    TIMER_A2-> CCTL[4] = 0b11100000;
    TIMER_A2-> CCTL[3] = 0b11100000;                    //reset/set mode
    TIMER_A2-> CCTL[2] = 0b11100000;
}

void ADC14_init (void)
{
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
    commandWrite(0x0C);
    delay_micro(100);
    commandWrite(1);
    delay_micro(100);
    commandWrite(6);
    delay_ms(10);
}

void SysTick_Init(){                    //initialize system timer
    SysTick->CTRL &= ~BIT0;             //clears enable to stop the counter
    SysTick->LOAD = 0x00FFFFFF;         //sets the period... note: (3006600/1000 - 1) = 1ms
    SysTick->VAL = 0;                   //clears the value
    SysTick->CTRL = 0x00000005;         //enable SysTick, no uint8_terrupts
}

void pulseEnablePin()                   //pulses enable pin
{
    P9->OUT |=  BIT1;
    delay_ms(10);
    P9->OUT &= ~BIT1;
}

void delay_micro(unsigned micro){       //delays desired time in us
    SysTick->LOAD = micro*3 - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = 5;
    while(!(SysTick->CTRL & BIT(16)));
}

void delay_ms(unsigned ms){             //delays desired time in ms
    SysTick->LOAD = ms*3000 - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = 5;
    while(!(SysTick->CTRL & BIT(16)));
}

void pushNibble(uint8_t nibble)         //pushes 4 bits onto pins
{
    P9->OUT &= (0x0F);
    P9->OUT |= ((nibble & 0x0F) << 4);
    pulseEnablePin();
}

void pushByte(uint8_t byte){            //splits byte into 2 nibbles and sends to push nibble
    pushNibble(byte >> 4);
    pushNibble(byte & (0x0F));
}

void commandWrite(uint8_t command){     //sends byte of command to lcd
    P9->OUT &= ~BIT0;
    delay_ms(10);
    pushByte(command);
}

void dataWrite(uint8_t data){           //sends byte of data to lcd
    P9->OUT |= BIT0;
    delay_ms(10);
    pushByte(data);
}

void printString(char string[]){        //prints a string of characters
    int i;
    for (i = 0; i < (strlen(string)); i++){
            dataWrite(string[i]);
    }
}
