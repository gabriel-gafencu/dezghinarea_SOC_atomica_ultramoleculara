#include<iom16.h>
#include<inavr.h>
#include"usart.h"

__no_init unsigned long long softCount;
__no_init unsigned char pinVal;             //variabila pt verificare

#pragma vector = TIMER0_COMP_vect
__interrupt void isr_TIMER0_COMP_vect()
{
  softCount++;
}

void main( void )
{ 
  //fac verificare a timpului cu SALEAE LOGIC
  DDRB = (1 << PB0);
  if(pinVal == 0)
  {
    PORTB = PORTB ^ (1 << PB0);
    pinVal = 1;
  }
  else
  {
    PORTB = PORTB ^ (1 << PB0);;
    pinVal = 0;
  }
  //end verificare
  
  USART_init(BAUD_RATE);
  DDRD |= (1 << PD1);
  
  unsigned long long aux = softCount / 1000;
  USART_transmit('<');
  if(aux > 0)
    USART_transmit_ulonglong(aux);
  else
    USART_transmit('0');
  aux = softCount % 1000;
  USART_transmit('.');
  if(aux / 100 > 0)
    USART_transmit_ulonglong(aux);
  else
  {
    USART_transmit('0');
    if(aux / 10 > 0)
      USART_transmit_ulonglong(aux);
    else
    {
      USART_transmit('0');
      USART_transmit_ulonglong(aux);
    }
  }
  USART_transmit('>');
  USART_transmit('\r');
  USART_transmit('\n');
  
  softCount = 0;
  
  TCCR0 = (1 << WGM01) | (0 << WGM00) | (0 << CS02) | (1 << CS01) | (1 << CS00);
  OCR0 = 63;
  TIMSK |= (1 << OCIE0);
  __enable_interrupt();
  
  while(1)
  {
   if(softCount % 1000 == 0)
   {
     aux = softCount / 1000;
     USART_transmit('<');
     if(aux > 0)
       USART_transmit_ulonglong(aux);
     else
       USART_transmit('0');
     aux = softCount % 1000;
     USART_transmit('.');
     if(aux / 100 > 0)
       USART_transmit_ulonglong(aux);
     else
     {
       USART_transmit('0');
       if(aux / 10 > 0)
         USART_transmit_ulonglong(aux);
       else
       {
         USART_transmit('0');
         USART_transmit_ulonglong(aux);
       }
     }
     USART_transmit('>');
     USART_transmit('\r');
     USART_transmit('\n');
   }
  }
}
