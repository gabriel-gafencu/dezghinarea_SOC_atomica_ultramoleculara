#include<iom16.h>
#include<inavr.h>

#define F_OSC 4010400

#pragma vector = TIMER1_COMPA_vect
__interrupt void isr_TIMER1_COMPA_vect()
{
  PORTB = 0; //PORTB ^ (1 << PB0);
}

void main( void )
{
  TCCR1B |= (1 << WGM12) | (1 << CS12) | (0 << CS11) | (1 << CS10);
  TIMSK |= (1 << OCIE1A);
  OCR1A = 784;
  __enable_interrupt();
  DDRB = (1 << PB0);
  PORTB = (1 << PB0);
  
  WDTCR |= (1 << WDP2) | (0 << WDP1) | (1 << WDP0);
  WDTCR |= (1 << WDE);
  
  while(1)
    ;
}
