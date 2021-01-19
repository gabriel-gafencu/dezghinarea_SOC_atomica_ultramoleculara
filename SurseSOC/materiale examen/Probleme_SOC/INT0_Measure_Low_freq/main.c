#include<iom16.h>
#include<inavr.h>
#include"usart.h"

unsigned int softCount;
unsigned int events;

#pragma vector = TIMER0_COMP_vect
__interrupt void isr_TIMER0_COMP_vect()
{
  softCount++;
  if(softCount > 20)
    softCount = 0;
}

#pragma vector = INT0_vect
__interrupt void isr_INT0_vect()
{
  events++; 
}

void main( void )
{
  GICR |= (1 << INT0);
  MCUCR |= (1 << ISC01) | (1 << ISC00);
  DDRD |= (0 << PD2);
  
  USART_init(BAUD_RATE);
  DDRD |= (1 << PD1) | (0 << PD0);
  
  TCCR0 |= (1 << WGM01) | (0 << WGM00) | (1 << CS02) | (0 << CS01) | (1 << CS00);
  OCR0 = 196;
  TIMSK |= (1 << OCIE0);
  __enable_interrupt();
  
  while(1)
  {
    if(softCount == 0)
    {
      softCount++;
      USART_transmit_uint(events);
      USART_transmit('H');
      USART_transmit('z');
      USART_transmit('\r');
      USART_transmit('\n');
      events = 0;
    }
  }
}
