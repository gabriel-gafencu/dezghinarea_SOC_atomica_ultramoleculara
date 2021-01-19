
#include "usart.h"
void USART_initialize(unsigned short int baud_rate)
{
  /* seteaz� baud rate */
  UBRRH = (unsigned char)(baud_rate >> 8);
  UBRRL = (unsigned char)(baud_rate & 0xFF);
  UCSRB = (1 << RXEN) | (1 << TXEN); /* activeaz� transmisia .i recep.ia la ie.ire */
  /* seteaz� pinul TXD: ie.ire */
  DDRD |= (1 << PD1);
  /* seteaz� pinul RXD: intrare */
  DDRD &= ~(1 << PD0);
  /* activeaz� �ntreruperea */
  UCSRB |= (1 << RXCIE);
  UCSRB |= (1 << TXCIE);
}

void USART_transmit( unsigned char data)
{
  /* a.teapt� p�n� ce se termin� de transmis toate datele .i dup� trece la urm�toarele informa.ii */
  while (!( UCSRA & (1 << UDRE)))
  {
    ;
  }
  UDR = data;
}
unsigned char USART_Receive(void)
{
  /* A.teapta recep.ionarea datelor */
  while ( !(UCSRA & (1<<RXC)) )
  {
    ;
  }
  /* Preia .i returneaza datele recep.ionate din buffer */
  return UDR;
}

#pragma vector = USART_TXC_vect
__interrupt void interrupt_routine_USART_TXC(void)
{
  unsigned char test = 0;
  test += 1;
}
#pragma vector = USART_RXC_vect
__interrupt void interrupt_routine_USART_RXC(void)
{
  
}
