#include<iom16.h>
#include<inavr.h>
#include"crc.h"
#include"usart.h"

void main( void )
{
  USART_init(BAUD_RATE);
  DDRD |= (1 << PD1);
  
  unsigned int startAdress, finalAdress;
  unsigned int memLen;
  unsigned int my_crc16_calc;
  unsigned int my_crc16_tab;
  
  while(1)
  {
    startAdress = USART_receive_hexa_adress();
    finalAdress = USART_receive_hexa_adress();
    memLen = finalAdress - startAdress;
    
    my_crc16_calc = crc16(CRC16_CCITT, 0, startAdress, memLen, MSBF);
    my_crc16_tab = crc16wtable(0, startAdress, memLen);
    
    USART_transmit('C');
    USART_transmit('R');
    USART_transmit('C');
    USART_transmit('1');
    USART_transmit('6');
    USART_transmit(':');
    USART_transmit(' ');
    hexadecimalTransmit(my_crc16_calc);
    USART_transmit('\r');
    USART_transmit('\n');
  }
}
