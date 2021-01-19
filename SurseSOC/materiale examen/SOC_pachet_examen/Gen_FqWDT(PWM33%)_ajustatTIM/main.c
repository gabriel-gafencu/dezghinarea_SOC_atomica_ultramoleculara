#include<iom16.h>
#include<inavr.h>

unsigned int nrOvf;

#pragma vector = TIMER0_COMP_vect
__interrupt void isr_TIMER0_overflow()
{
  nrOvf++;
}

__no_init unsigned char valAnt;

void main( void )
{
  DDRD |= (1 << PD5);
  if(valAnt == 0)
    PORTD |= (1 << PD5);
  
  TCCR0 |= (1<<WGM01) | (0<<WGM00) | (1 << CS01) | (1 << CS00);
  TIMSK |= (1 << OCIE0);
  __enable_interrupt();
  
  OCR0=253;

  if(valAnt == 1)
  {
    while(1)
    {
      if(nrOvf == 2)
      {
        nrOvf++;
        valAnt = 0;
        WDTCR |= (0 << WDP2) | (0 << WDP1) | (1 << WDP0) | (1 << WDE);
      }
    }
  }
  else
  {
    while(1)
    {
      if(nrOvf == 1)
      {
        nrOvf++;
        valAnt = 1;
        PORTD |= (1 << PD5); 
        WDTCR |= (0 << WDP2) | (0 << WDP0) | (1 << WDE);
      }
    }
  }
}
