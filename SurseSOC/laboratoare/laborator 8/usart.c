#include "usart.h"

extern char data;
extern char flag;

void USART_initialize(unsigned short int baud_rate)
{
  /* seteaza baud rate */
  UBRRH = (unsigned char)(baud_rate >> 8);
  UBRRL = (unsigned char)(baud_rate & 0xFF);
  
  UCSRB = (1 << RXEN) | (1 << TXEN); /* activeaza transmisia si receptia la iesire */
  
  /* seteaza pinul TXD: iesire */
  DDRD |= (1 << PD1);
  
  /* seteaza pinul RXD: intrare */
  DDRD &= ~(1 << PD0);
  
  /* activeaza întreruperea */
  UCSRB |= (1 << RXCIE);
}
void USART_transmit(unsigned char data)
{
  /* asteapta pâna ce se termina de transmis toate datele si dupa trece la urmatoarele informatii */
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
    flag = 0; // marcam flag-ul pe 0
    *d = data; // preluam data
    ret = 0; // codul de returnare este succes
  }
  return ret;
}