#include <inavr.h>
#include <iom16.h>
#include "usart.h"

//transmiterea unui întreg pe seriala
void integerTransmit (unsigned int p1, unsigned int p2, void * p3)
{
  int index=0;
  char aux[5];
  int x=*((int *)(p3));
  if(x<0)
  {
    USART_transmit('-');
    x*=(-1);
  }
  while(x!=0)
  {
    aux[index]=x%10+'0';
    index++;
    x=x/10;
  }
  while(p2>0 )
  {
    USART_transmit(aux[index-1]);
    index--;
    p2--;
  }
}

unsigned int per; // perioada semnalului
unsigned int nr_cicli; 
unsigned int flag_per;

// rutina de intrerupere pentru INT0
#pragma vector = INT0_vect
__interrupt void isr_INT0(void)
{
  if((PIND & (1<<PD2)) == 1) // frontul pozitiv
  {
    TCNT1 = 0; // reset timer
    TCCR1B = (1<<CS12); // start timer cu prescaler de 256
  }
  else // frontul negativ
  {
    nr_cicli = TCNT1; // salveazã nr de cicli
    per = (nr_cicli / 4) * 256; // calculeazã perioada în microsecunde
    flag_per = 1;
  }
}

int main( void )
{
  unsigned int T;
  
  // initializare întrerupere externa
  MCUCR |= (1<<ISC00);
  MCUCR &= ~(1<<ISC01); // ISC = 01, adica orice schimbare de front genereaza intrerupere
  
  GICR |= (1<<INT0); // activare întrerupere externa
  __enable_interrupt();
  
  while(1)
  {
    while(flag_per == 0); // asteapta actiunea intreruperii
    flag_per = 0; // reseteaza flag
    T = per; // se face ceva cu valoarea masurata
    
    // trimitem valoarea pe seriala
    // perioada se va obtine in microsecunde
    USART_initialize(BAUD_RATE); // rata de biti de pe seriala
    integerTransmit(0, 5, &T);
    USART_transmit('\r');
    USART_transmit('\n');
  }
}