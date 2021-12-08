#include "msp.h"
#include "Functions_Lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

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
