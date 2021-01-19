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
  unsigned int funcAdress = (((unsigned int)min) << 1);
  unsigned int funcLen = lenTillRet(funcAdress, 2);
  unsigned long my_crc32 = crc32(CRC32_CCITT, 0, funcAdress, funcLen, MSBF);
  unsigned long my_crc32_t = crc32wtable(0, funcAdress, funcLen);
  
  while(1)
    ;
}
