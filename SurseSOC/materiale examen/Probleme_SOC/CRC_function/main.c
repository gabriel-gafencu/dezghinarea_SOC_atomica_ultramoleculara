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

unsigned int lenTillRet(unsigned int startAdress, unsigned int retNo)
{
  unsigned int len = 0;
  unsigned int aux = *(__flash unsigned int*)(startAdress);
  while(retNo > 0)
  {
    len = len + 2;
    aux = *(__flash unsigned int*)(startAdress + len);
    while(aux != 0x9508)
    {
      len = len + 2;
      aux = *(__flash unsigned int*)(startAdress + len);
    }
    retNo--;
  }
  len = len + 2;
  return len;
}

void main( void )
{
  unsigned int funcAdress = ((unsigned int)min)<<1;
  unsigned int aux = *(__flash unsigned int*)(funcAdress+12);
  unsigned int funcLen = lenTillRet(funcAdress, 2);
  unsigned int my_crc16 = crc16(CRC16_CCITT, 0, funcAdress, funcLen, MSBF);
  unsigned int mycrc16_t = crc16wtable(0, funcAdress, funcLen);
  
  while(1)
    ;
}
