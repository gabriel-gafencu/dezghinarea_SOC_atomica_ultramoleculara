#include "usart.h"

extern char data;
extern char flag;

void USART_initialize(unsigned short int baud_rate)
{
  /* seteazã baud rate */
  UBRRH = (unsigned char)(baud_rate >> 8);
  UBRRL = (unsigned char)(baud_rate & 0xFF);
  
  UCSRB = (1 << RXEN) | (1 << TXEN); /* activeazã transmisia si receptia la iesire */
  
  /* seteazã pinul TXD: iesire */
  DDRD |= (1 << PD1);
  
  /* seteazã pinul RXD: intrare */
  DDRD &= ~(1 << PD0);
  
  /* activeazã întreruperea */
  UCSRB |= (1 << RXCIE);
}

void USART_transmit(unsigned char data)
{
  /* asteaptã pânã ce se terminã de transmis toate datele si dupã trece la urmãtoarele informatii */
  while (!( UCSRA & (1 << UDRE)))
  {
    ;
  }
  UDR = data;
}

int USART_Receive( char* d )
{
  int ret = -1; // codul de returnare (implicit, eroare)
  if (flag == 1) // daca exista ceva pe seriala
  {
    flag = 0; // marcam flag-ul pe 0 (am preluat datele)
    *d = data; // preluam data
    ret = 0; // codul de returnare este succes
  }
  return ret;
}