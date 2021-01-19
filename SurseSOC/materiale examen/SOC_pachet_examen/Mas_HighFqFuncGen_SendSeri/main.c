#include<iom16.h>
#include<inavr.h>
#include "usart.h"

unsigned int nrOvf;
unsigned int softCount;

#pragma vector = TIMER0_COMP_vect
__interrupt void isr_TIMER0_COMP_vect()
{
  softCount++;
}

#pragma vector = TIMER1_OVF_vect
__interrupt void isr_TIMER1_OVF_vect()
{
  nrOvf++;
}

void main( void )
{
  USART_init(BAUD_RATE);
  DDRD |= (1 << PD1);
  
  TCCR0 |= (1 << WGM01) | (1 << CS02) | (1 << CS00);
  TIMSK |= (1 << OCIE0);
  OCR0 = 247;
  
  TCCR1B |= (1 << CS12) | (1 << CS11);
  TIMSK |= (1 << TOIE1);
   __enable_interrupt();
  
  unsigned long long fq;
  while(1)
  {
    if(softCount == 16)
    {     
      fq = nrOvf;
      fq = fq * 65536 + TCNT1;
      
      USART_transmit('<');
      USART_transmit('F');
      USART_transmit('Q');
      USART_transmit('=');
      USART_transmit_ulonglong(fq);
      USART_transmit('H');
      USART_transmit('z');
      USART_transmit('>');
      USART_transmit('\r');
      USART_transmit('\n');
      
      softCount = 0;
      nrOvf = 0;
      TCNT1 = 0;
    }
  }
}
