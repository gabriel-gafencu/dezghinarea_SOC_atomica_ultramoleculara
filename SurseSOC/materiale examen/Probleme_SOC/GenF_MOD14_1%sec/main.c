#include "timer1.h"
#include "usart.h"

extern unsigned int softCount;

void main( void )
{ 
  USART_init(BAUD_RATE);
  DDRD |= (1 << PD1);
  DDRD &= ~(1 << PD0);
  
  TIMSK |= (1 << TOIE0);
  __enable_interrupt();
  
  DDRD |= (1 << PD5);
  TIMER1_init();
  TIMER0_init();
  
  unsigned int direction = 1;
  unsigned int duty = 5;
  unsigned long int aux;
  while(1)
  {
    if(softCount == 0)
    {
      softCount++;
      if(duty >= 95) 
        direction = 0;
      if(duty <= 5) 
        direction = 1;
      if(direction == 1) duty++;
      else duty--;
      aux = ICR1;
      aux = aux * duty / 100;
      OCR1A = aux;
      
      USART_transmit('<');
      USART_transmit('F');
      USART_transmit('U');
      USART_transmit(':');
      USART_transmit_uint(duty);
      USART_transmit('>');
    }
  }
}
