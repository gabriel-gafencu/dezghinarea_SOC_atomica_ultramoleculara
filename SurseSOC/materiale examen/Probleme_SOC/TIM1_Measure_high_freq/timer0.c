#include"timer0.h"

unsigned int tim0softCount;

void TIMER0_init()
{
  TCCR0 = (1 << CS02) | (0 << CS01) | (1 << CS00) | (1 << WGM01) | (0 << WGM00);
  OCR0 = 196;
  TIMSK |= (1 << OCIE0);
}

__interrupt void isr_TIMER0_COMP_vect()
{
  
  tim0softCount++;
  if(tim0softCount > 20)
    tim0softCount = 0;
}