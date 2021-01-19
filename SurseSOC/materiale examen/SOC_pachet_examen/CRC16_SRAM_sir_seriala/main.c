#include<iom16.h>
#include<inavr.h>
#include"crc.h"
#include"usart.h"

void main( void )
{
  USART_init(BAUD_RATE);
  DDRD |= (1 << PD1);
  
  unsigned int my_crc16_calc;
  unsigned int my_crc16_tab;
  
  unsigned int adrStr;
  unsigned char recStr[64];
  unsigned int index = 0;
  
  while(1)
  {
    recStr[index] = USART_receive();
    while(recStr[index] != '@')
    {
      index++;
      recStr[index] = USART_receive();
    }
    
    adrStr = (unsigned int)(recStr);
    my_crc16_calc = crc16(CRC16_CCITT, 0, adrStr, index, MSBF);
    my_crc16_tab = crc16wtable(0, adrStr, index);
    
    int i;
    USART_transmit('<');
    for(i = 0; i < index; i++)
    {
      USART_transmit(recStr[i]);
    }
    USART_transmit(' ');
    USART_transmit('C');
    USART_transmit('R');
    USART_transmit('C');
    USART_transmit('1');
    USART_transmit('6');
    USART_transmit(':');
    hexadecimalTransmit(my_crc16_calc);
    USART_transmit('>');
    USART_transmit('\r');
    USART_transmit('\n');
    
    index = 0;
  }
}
