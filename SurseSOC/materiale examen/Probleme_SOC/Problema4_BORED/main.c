#include<iom16.h>
#include<inavr.h>
#include"usart.h"

unsigned char softFlag;

#pragma vector = TIMER0_COMP_vect
__interrupt void isr_TIMER0_COMP_vect()
{
  softFlag = 1;
}

void main( void )
{
  USART_init(BAUD_RATE);
  DDRD = (1 << PD1);
  
  TCCR0 = (1 << WGM01) | (0 << WGM00) | (1 << CS02) | (0 << CS01) | (1 << CS00);
  OCR0 = 135;
  TIMSK |= (1 << OCIE0);
  __enable_interrupt();
  
  TCCR1A = (1 << COM1A1) | (0 << COM1A0) | (1 << WGM11) | (0 << WGM10);
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (0 << CS12) | (0 << CS11) | (1 << CS10);
  ICR1 = 667;
  unsigned long aux = ICR1;
  unsigned int duty = 5;
  aux = aux * duty / 100;
  OCR1A = aux;
  
  DDRD |= (1 << PD5);
  unsigned char direction = 1;
  while(1)
  {
    if(softFlag == 1)
    {
      softFlag = 0;
      if(direction == 1)
        duty += 3;
      else
        duty -= 3;
      
      if(duty >= 95)
        direction = 0;
      if(duty <= 5)
        direction = 1;
      
      aux = ICR1;
      aux = aux * duty / 100;
      OCR1A = aux;
      
      USART_transmit('<');
      USART_transmit('F');
      USART_transmit('=');
      if(duty/10 > 0)
        USART_transmit_uint(duty);
      else
      {
        USART_transmit('0');
        USART_transmit_uint(duty);
      }
      USART_transmit('%');
      USART_transmit('>');
    }
  }
}
