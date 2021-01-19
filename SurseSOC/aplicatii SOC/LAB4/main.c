#include "usart.h"

volatile char data;
int flagDataReceived;

void main( void )
{
  unsigned char string[]="Hello world!", aux;
  unsigned int i=0;
  
  USART_initialize(BAUD_RATE);
  
  while(1)
  {
    aux = USART_Receive();
    i=0;
    if(aux=='s'||aux=='S')
    {
      while(string[i] != '\0')
      { 
        USART_transmit(string[i]);
        i++;
      }
    }
    else
    {
      // free;
    }
  }
}