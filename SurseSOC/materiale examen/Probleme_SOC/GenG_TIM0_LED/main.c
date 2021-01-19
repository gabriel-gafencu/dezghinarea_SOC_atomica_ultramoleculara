#include "timer0.h"

extern unsigned int softCount;

void main( void )
{
  TIMSK |= (1 << TOIE0);
  __enable_interrupt();
  
  TIMER0_init();
  DDRB |= (1 << PB0);
  PORTB |= (1 << PB0);
  
  while(1)
  {
    if(softCount == 0)
    {
      PORTB = PORTB ^ (1 << PB0);
      softCount++;
    }
  }
}
