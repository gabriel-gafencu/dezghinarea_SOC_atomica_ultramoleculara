#include "usart.h"

void USART_init(int baud_rate)
{
  UBRRH = (baud_rate>>8); //setez baudrate-ul
  UBRRL = baud_rate;
  UCSRB |= (1<<RXEN) | (1<<TXEN); //enable receive & transmit
}

void USART_transmit(unsigned char c)
{
  while( !(UCSRA & (1<<UDRE)) )
    ;
  UDR = c;
}

unsigned char USART_receive(void)
{
  while( !(UCSRA & (1<<RXC)) )
    ;
  return UDR;
}

void USART_transmit_uint(unsigned int n)
{
  unsigned char aux[16];
  int index = 0;
  while(n)
  {
    aux[index++] = n % 10 + '0';
    n /= 10; 
  }
  index--;
  while(index >= 0)
  {
    USART_transmit(aux[index--]);
  }
}

void USART_transmit_ulonglong(unsigned long long n)
{
  unsigned char aux[20];
  int index = 0;
  while(n)
  {
    aux[index++] = n % 10 + '0';
    n /= 10; 
  }
  index--;
  while(index >= 0)
  {
    USART_transmit(aux[index--]);
  }
}