{
   if(P6->IFG & BIT4)
    {
      __delay_cycles(15000);
        if (!(P6->IN & BIT0))
        {
          printf("Emergency Stop!\n");
          TimerA1_PWM(0);
          P6->IFG  &= ~BIT0;  //clear the flag
        }
    }
} 
