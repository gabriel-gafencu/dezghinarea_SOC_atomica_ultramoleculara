#include"usart.h"

void main( void )
{
  unsigned char string[]="Hello world!", aux;
  unsigned int i=0;
  USART_initialize(BAUD_RATE);
  // aprindem un led
  DDRB |= (1 << PB7);
  PORTB |= (1 << PB7);
  
  int flag = 1;
  while(1)
  {
    i=0;
    if(flag)
    {
      UDR = '*';
      flag = 0;
    }
    if(USART_Receive(&aux) == 0)
    {
      // daca s-a receptionat ceva <=> s-a activat anterior intreruperea, stingem led-ul, altfel il aprindem
      PORTB &= ~(1 << PD7);
      if(aux=='s'||aux=='S')
      {
        while(string[i]!='\0')
        { 
          //UCSRA &= ~(1 << TXC);
          USART_transmit(string[i]);
          i++;
        }
      }
    }
    else
    {
      PORTB |= (1 << PB7);
    }
  }
}