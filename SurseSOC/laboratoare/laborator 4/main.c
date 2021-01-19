#include "usart.h"

void main( void )
{
  // sirul ce trebuie transmis
  unsigned char string[]="Hello world!";

  // variabila care retine ce caracter s-a primit pe seriala
  unsigned char aux;

  // indicele in sirul de caractere ce trebuie trimis
  unsigned int i = 0;
  
  // initializam comunicatia seriala
  USART_initialize(BAUD_RATE);
  
  while(1)
  {
    // asteptam sa primim ceva pe seriala
    aux = USART_Receive();
    
    // dupa ce am primit, verificam daca e caracterul 's' sau 'S'
    i = 0;
    if(aux == 's' || aux == 'S')
    {
      // daca da, parcurgem sirul de trimis, caracter cu caracter
      while(string[i] != '\0')
      {
        // trimitem cate un caracter pe seriala
        USART_transmit(string[i]);
        i++;
      }
    }
  }
}