#include "cod_lab.h"
#include "inavr.h"
#include "iom16.h"
#include "usart.h"

unsigned char min(unsigned char a, unsigned char b)
{
  if(a<b)
    return a;
  else
    return b;
}
unsigned int funtion_length(unsigned char (*pf)(unsigned char,unsigned char),unsigned int nr_returi)
{
  // preiau valoarea adresei functiei, inmultind valoarea pointer-ului cu 2 (lab CRC)
  // il fac pe p un pointer normal, la date, dar cu valoarea adresei din flash
  int *p = (int *)((int) pf * 2);
  while(nr_returi)
  {
    // il fac pe p sa se uite la flash si va indica spre instructiunea curenta
    // apoi il dereferentiez ca sa iau codul instructiunii si sa vad daca-i RET
    if(*(__flash int*)p == 0x9508)
      --nr_returi;
    ++p; // ++p => avans 2 octeti <=> avans o instructiune
  }
  return ((int)p-((int)pf) * 2);
}
void main(void)
{
  // initializare USART
  USART_initialize(BAUD_RATE);
  
  __enable_interrupt();
  
  unsigned char temp_string[] = "CRC16 ADR[";
  unsigned int adr_start,len;
  
  adr_start = (int)min * 2;
  len = funtion_length(min,2);
  
  unsigned int my_crc16 = crc16(CRC16_CCITT,0,adr_start,len,MSBF);
  unsigned int my_crc16_t = crc16wtable(0,adr_start,len);
  transmit_string(temp_string);
  transmit_hexa(4,adr_start);
  USART_transmit('-');
  transmit_hexa(4,adr_start + len - 2);
  USART_transmit(']');
  transmit_hexa(4,my_crc16);
  USART_transmit('\r');
  USART_transmit('\n');
  
  
  while(1)
  {
  }
}