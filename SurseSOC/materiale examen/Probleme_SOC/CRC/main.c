#include<iom16.h>
#include<inavr.h>
#include"crc.h"

unsigned char min(unsigned char x, unsigned char y)
{
  if(x < y)
    return x;
  else
    return y;
}

void main( void )
{
  unsigned int real_crc = *(__flash unsigned int*)(0x4000-2);
  unsigned int my_crc16 = crc16(CRC16_CCITT, 0, 0, (0x4000-2), MSBF);
  unsigned int mycrc16_t = crc16wtable(0, 0, (0x4000-2));
  
  while(1)
    ;
}
