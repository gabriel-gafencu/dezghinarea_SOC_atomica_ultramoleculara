#include<iom16.h>
#include<inavr.h>
#include "usart.h"

unsigned char changeFlag;

#pragma vector = TIMER0_COMP_vect
__interrupt void isr_TIMER0_COMP_vect()
{
  changeFlag = 1;
}

void main( void )
{
  USART_init(BAUD_RATE);
  DDRD |= (1 << PD5) | (1 << PD1);
  
  TCCR0 |= (1 << WGM01) | (1 << CS02) | (1 << CS00);
  OCR0 = 138;
  TIMSK |= (1 << OCIE0);
  __enable_interrupt();
  
  TCCR1A |= (1 << COM1A1) | (1 << WGM11);
  TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS10);
  ICR1 = 200;
  
  unsigned char direction = 1;
  unsigned int duty = 5;
  while(1)
  {
    if(changeFlag == 1)
    {
      changeFlag = 0;
      if(direction == 1) duty += 3;
      else duty -= 3;
      
      if(duty >= 95) direction = 0;
      if(duty <= 5) direction = 1;
      
      unsigned long aux = ICR1;
      aux = aux * duty / 100;
      OCR1A = aux;
      
      USART_transmit('<');
      USART_transmit('F');
      USART_transmit('U');
      USART_transmit('=');
      USART_transmit_uint(duty);
      USART_transmit('>');
      USART_transmit('\r');
      USART_transmit('\n');
    }
  }
}
