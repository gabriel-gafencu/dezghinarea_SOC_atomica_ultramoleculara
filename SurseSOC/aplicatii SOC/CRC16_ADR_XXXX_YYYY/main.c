#include "cod_lab.h"
#include "inavr.h"
#include "iom16.h"
#include "usart.h"

void main(void)
{
  // initializare USART
  USART_initialize(BAUD_RATE);
  
  __enable_interrupt();
  
  unsigned char temp_string[] = "CRC16 ADR[";
  unsigned char pos, flag_error;
  char aux;
  unsigned int adr_start,adr_stop;
  // 0x001A 0x002F 
  while(1)
  {
    pos = 0;
    adr_start = 0;
    adr_stop = 0;
    flag_error = 0;
    while(pos < 13 && !flag_error)
    {
      if(USART_receive(&aux) == 0)
      {
        if(((pos == 0 || pos == 7) && aux != '0') || ((pos == 1 || pos == 8) && aux != 'x') || (pos == 6 && aux != ' '))
          flag_error = 1;
        else 
        {
          if(pos > 1 && pos < 6)
          {
            if ((aux >= '0' && aux <= '9'))
              adr_start = (adr_start << 4) | (aux - '0');
            else
            {
              if((aux >= 'A' && aux <= 'F'))
                adr_start = (adr_start << 4) | (aux - 'A' + 10);
              else
                flag_error = 1;
            }
          }
          else
          {
            if (pos >8 && pos < 13)
            {
              if ((aux >= '0' && aux <= '9'))
                adr_stop = (adr_stop << 4) | (aux - '0');
              else
              {
                if((aux >= 'A' && aux <= 'F'))
                  adr_stop = (adr_stop << 4) | (aux - 'A' + 10);
                else
                  flag_error = 1;
              }
            }
          }
        }
        ++pos;
      }
    }   
    if(!flag_error)
    {    
      unsigned int my_crc16 = crc16(CRC16_CCITT,0,adr_start,adr_stop - adr_start + 1,MSBF);
      unsigned int my_crc16_t = crc16wtable(0,adr_start,adr_stop - adr_start + 1);
      
      transmit_string(temp_string);
      transmit_hexa(4,adr_start);
      USART_transmit('-');
      transmit_hexa(4,adr_stop);
      USART_transmit(']');
      transmit_hexa(4,my_crc16);
      
      USART_transmit('\r');
      USART_transmit('\n');
    }
    else
    {     
      // golire buffer
      while(USART_receive(&aux) == 0);
    }
  }
}