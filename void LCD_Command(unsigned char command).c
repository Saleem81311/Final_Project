{
  SysTick_Init();
   P6->OUT &= ~(RS|RW);      // RS = 0, RW = 0
   P9->OUT = command;      // Put command on data bus
   P6->OUT |= EN;      // Pulse EN HIGH
   SysTick_Delay(1);
   P6->OUT &= ~EN;     // Clear EN
   if(command < 4)
      SysTick_Delay(4);    // Desired command speed
   else
      SysTick_Delay(1);    // Any others need 40 us
}
