#include "usart.h"

char data;
char flag;

// rutina de intrerupere la primirea de date pe seriala
#pragma vector = USART_RXC_vect
__interrupt void interrupt_routine_USART_RXC(void)
{
  // cand am primit date, le salvam in variabila data
  data = UDR;
  
  // setam flagul pe 1, adica exista ceva pe seriala
  flag = 1;
}

void main( void )
{
  unsigned char string[] = "Hello world!";
  unsigned int i = 0;
  char c;
  
  // initializare transmisie seriala
  USART_initialize(BAUD_RATE); // rata de biti de pe seriala
  
  // activam intreruperea globala
  __enable_interrupt();
  
  while(1)
  {
    if (USART_Receive(&c) == 0) // primim data de pe serial (daca exista)
    {
      // tratam intreruperea
      i = 0; // indicele caracterului curent din string
      if(c == 's' || c == 'S') // doar daca am primit caracterele 's' sau 'S' afisam 'Hello World'
      {
        while(string[i] != '\0') // cat timp nu s-a terminat sirul
        {
          USART_transmit(string[i]); // trimitem pe seriala acel caracter
          i++;
        }
      }
    }
    else
    {
      // procesorul este liber
    }
  }
}