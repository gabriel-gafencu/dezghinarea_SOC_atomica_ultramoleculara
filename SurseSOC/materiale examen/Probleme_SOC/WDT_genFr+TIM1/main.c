#include<iom16.h>
#include<inavr.h>

__no_init unsigned char flag;

#pragma vector = TIMER1_COMPA_vect
__interrupt void isr_TIMER1_COMPA_vect()
{
 if(flag == 0)
 {
   WDTCR = (0 << WDP2) | (1 << WDP1) | (1 << WDP0);
   WDTCR |= (1 << WDE);
   flag = 1;
   TIMSK = (0 << OCIE1A);
 }
 else
 {
   WDTCR = (0 << WDP2) | (1 << WDP1) | (0 << WDP0);
   WDTCR |= (1 << WDE);
   flag = 0;
   TIMSK = (0 << OCIE1A);
 }
}


void main( void )
{
  TIMSK |= (1 << OCIE1A);
  __enable_interrupt();
  DDRB = (1 << PB0);
  if(flag == 0)
  {
    PORTB = (1 << PB0);
    TCCR1A = (1 << WGM11) | (1 << WGM10);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS11) | (0 << CS10);
    OCR1A = 16804; //22665;
    TCNT1 = 0;
  }
  else
  {
    PORTB = (0 << PB0);
    TCCR1A = (1 << WGM11) | (1 << WGM10);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS11) | (0 << CS10);
    OCR1A = 9380; //12335;
    TCNT1 = 0;
  } 
  while(1)
    ;
}
