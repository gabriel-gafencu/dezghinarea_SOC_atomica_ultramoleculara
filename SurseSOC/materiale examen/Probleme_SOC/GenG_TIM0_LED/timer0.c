#include "timer0.h"

unsigned int softCount;

void TIMER0_init()
{
  TCCR0 = (0 << WGM01) | (0 << WGM00) | (1 << CS02) | (0 << CS01) | (1 << CS00);
}

__interrupt void isr_TIMER0_OVF_vect()
{
  if(softCount < 8)
    softCount++;
  else
    softCount = 0;
}