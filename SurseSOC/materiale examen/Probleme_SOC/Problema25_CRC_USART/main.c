#include<iom16.h>
#include<inavr.h>
#include"usart.h"
#include"crc.h"

unsigned char rxFlag;

#pragma vector = USART_RXC_vect
__interrupt void isr_USART_RXC_vect()
{
  rxFlag = 1;
  UCSRB = UCSRB ^ (1 << RXCIE);
}

void main( void )
{
  USART_init(BAUD_RATE);
  DDRD |= (1 << PD1) | (0 << PD0);
  UCSRB |= (1 << RXCIE);
  __enable_interrupt();
  
  unsigned char rec[32];
  unsigned int index = 0;
  while(1)
  {
   if(rxFlag == 1)
   {
     rxFlag = 0;
     rec[index] = USART_receive();
     while(rec[index] != '@')
     {
       index++;
       rec[index] = USART_receive();
     }
     unsigned int myCrc = crc16(CRC16_CCITT, 0, (unsigned int)(&rec[0]), index, MSBF);
    
     USART_transmit('<');
     int i;
     for(i = 0; i <= index; i++)
       USART_transmit(rec[i]);
     USART_transmit(' ');
     USART_transmit('C');
     USART_transmit('R');
     USART_transmit('C');
     USART_transmit('1');
     USART_transmit('6');
     USART_transmit(' ');
     hexadecimalTransmit(myCrc);
     USART_transmit('>');
     USART_transmit('\r');
     USART_transmit('\n');
     index = 0;
     UCSRB |= (1 << RXCIE);
   }
  }
}
