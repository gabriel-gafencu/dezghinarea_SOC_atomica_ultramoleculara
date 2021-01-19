#include "usart.h"

void USART_initialize(unsigned short int baud_rate)
{
  // seteaza baud rate
  UBRRH = (unsigned char)(baud_rate >> 8);
  UBRRL = (unsigned char)(baud_rate & 0xFF);
  
  // activeaza transmisia si receptia la iesire
  UCSRB = (1 << RXEN) | (1 << TXEN); 
  
  // seteaza pinul RXD de intrare
  DDRD &= ~(1 << PD0);
  // seteaza pinul TXD de iesire
  DDRD |= (1 << PD1);
}
void USART_transmit(unsigned char data)
{
  // asteapta pana ce se termina de transmis toate datele si dupa se trasmite noul byte 'data'
  // folosim TXC = Transmit Complete
  /*
  UDR = data;
  while (!( UCSRA & (1 << TXC)))
  {
    ;
  }
  */
  // folosim UDRE = USART Data Register Empty
  
  while (!( UCSRA & (1 << UDRE)))
  {
    ;
  }
  UDR = data;
  
}
unsigned char USART_Receive( void )
{
  // Asteapta receptionarea datelor
  while ( !(UCSRA & (1 << RXC)) )
  {
    ;
  }
  // Preia si returneaza datele receptionate din buffer
  return UDR;
}