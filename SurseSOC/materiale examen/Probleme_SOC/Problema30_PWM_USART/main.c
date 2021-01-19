#include<iom16.h>
#include<inavr.h>
#include"usart.h"

#pragma vector = USART_RXC_vect
__interrupt void isr_USART_RXC_vect()
{
  UCSRB = UCSRB ^ (1 << RXCIE);     //opresc intreruperea
  unsigned long duty = 0;
  unsigned char rec = USART_receive();
  while(rec != '%')
  {
    duty = duty * 10 + (unsigned int)(rec - '0');
    rec = USART_receive();
  }
  OCR1A = duty * ICR1 / 100;
  UCSRB |= (1 << RXCIE);    //repornesc intreruperea
}

void main( void )
{
  USART_init(BAUD_RATE);
  DDRD |= (1 << PD1) | (0 << PD0);
  UCSRB |= (1 << RXCIE);
  __enable_interrupt(); 
  
  TCCR1A = (1 << COM1A1) | (0 << COM1A0) | (1 << WGM11) | (0 << WGM10);
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (0 << CS12) | (0 << CS11) | (1 << CS10);
  ICR1 = 785;
  OCR1A = ICR1 * 5 / 10;
  DDRD |= (1 << PD5);
  
  while(1)
    ;
}
