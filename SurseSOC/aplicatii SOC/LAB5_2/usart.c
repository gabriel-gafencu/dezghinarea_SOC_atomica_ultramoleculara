#include "usart.h"

// folosim volatile pentru date partajate (explicatie in caiet)
volatile char data;
int volatile flagDataReceived;

#pragma vector = USART_RXC_vect
__interrupt void ReceiveInterruptRoutine(void)
{
  data = UDR; //are ca efect secundar resetarea flag-ului RXC
  // daca primim un caracter si nu-l citim, flagul RXC ramane setat si intreruperea este reapelata la infinit
  flagDataReceived = 1;
}

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
  
  // activeaza intruperea 
  UCSRB |= (1 << RXCIE);
  __enable_interrupt();
}
void USART_transmit(unsigned char data)
{
  // asteapta pana ce se termina de transmis toate datele si dupa trece la urmatoarele informatii
  while (!( UCSRA & (1 << TXC)))
  {
    ;
  }
  
  UCSRA |= (1 << TXC);
  UDR = data;
  
}

// functia returneaza 0 daca s-au preluat date salvate in variabila globala 'data' si -1 in caz contrar
int USART_Receive(char *d)
{
  if(flagDataReceived == 1)
  {
    *d = data;
    flagDataReceived = 0;
    return 0;
  }
  return -1;
}
    