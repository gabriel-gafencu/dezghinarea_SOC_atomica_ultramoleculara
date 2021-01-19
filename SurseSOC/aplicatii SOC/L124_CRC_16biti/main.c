#include "cod_lab.h"
#include "inavr.h"
#include "iom16.h"

void main(void)
{
  unsigned char x = 0;
  unsigned int real_crc = *(__flash unsigned int*)(0x4000-2);
  unsigned int my_crc16 = crc16(CRC16_CCITT,0,0,(0x4000-2),MSBF);
  unsigned int my_crc16_t = crc16wtable(0,0,(0x4000-2));
  
  while(1)
  {
     UDR = x;
  }
}