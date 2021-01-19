#include "inavr.h"
#include "iom16.h"
#include "cod_lab.h"
#include "usart.h"

void main(void)
{
  char caracter;
  unsigned int crc = 0;
  USART_initialize(BAUD_RATE);
  unsigned char sir[] = " CRC160 ";
  __enable_interrupt();
  
  
  while(1)
  {
    if (USART_receive(&caracter) == 0)
    {
      USART_transmit(caracter);
      if (caracter == '@')
      {
         transmit_string(sir);
         transmit_hexa(4, crc);
         USART_transmit('\r');
         USART_transmit('\n');
         crc = 0;
      }
      else
        crc = crc16 (CRC16_CCITT, crc, (unsigned int)&caracter, 1, MSBF);  
    }
  }
}