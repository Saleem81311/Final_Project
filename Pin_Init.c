void Pin_Init()     // Pin initializations
{
  /*********************************************************** LEDs mimicking a running system ***********************************************************/
    P2->SEL0 &= ~0x38;
    P2->SEL1 &= ~0x38;
    P2->DIR |= 0x38;
    P2->OUT &= ~0x38;

/********************************************************************* Estop Button ********************************************************************/
    P1->SEL0 &= ~(Estop|Reset);
    P1->SEL1 &= ~(Estop|Reset);
    P1->DIR &= ~(Estop|Reset);
    P1->REN |= (Estop|Reset);
    P1->OUT |= (Estop|Reset);
    P1->IES |= (Estop|Reset);
    P1->IE  |= (Estop|Reset);
    P1->IFG = 0;
}
