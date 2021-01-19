#include "usart.h"

void USART_initialize(unsigned short int baud_rate)
{
  /* seteaz� baud rate */
  UBRRH = (unsigned char)(baud_rate >> 8);
  UBRRL = (unsigned char)(baud_rate & 0xFF);
  
  UCSRB = (1 << RXEN) | (1 << TXEN); /* activeaz� transmisia si receptia la iesire */
  
  /* seteaz� pinul TXD: iesire */
  DDRD |= (1 << PD1);
  /* seteaz� pinul RXD: intrare */
  DDRD &= ~(1 << PD0);
  
  /* activeaz� �ntreruperea */
  // UCSRB |= (1 << RXCIE);
}
void USART_transmit(unsigned char data)
{
  /* asteapt� p�n� ce se termin� de transmis toate datele si dup� trece la urm�toarele informatii */
  while (!(UCSRA & (1 << UDRE)))
  {
    ;
  }
  UDR = data;
}
unsigned char USART_Receive( void )
{
  /* asteapt� receptionarea datelor */
  while (!(UCSRA & (1 << RXC)))
  {
    ;
  }
  /* preia si returneaz� datele receptionate din buffer */
  return UDR;
}