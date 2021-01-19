#include "functii.h"
#include "usart.h"

unsigned int mycrc16=0;
unsigned char str[]="CRC16ADR[";
unsigned int lung=0;
unsigned int adresa;
unsigned long long adresaA;
unsigned long long adresaSF;
unsigned long long* aStart;
unsigned long long* aFinal;
unsigned int *hexCrc;
int i;
void main(void)
{
  adresa=(unsigned int)min;
  adresaA=adresa<<1;
  lung=lungimeFunctie(adresa);
  adresaSF=adresaA+lung;
  aStart=&adresaA;
  aFinal=&adresaSF;
  
  
  mycrc16=crc16(CRC16_CCITT,0,adresa<<1,lung,MSBF);
  hexCrc=&mycrc16;
  
  USART_initialize(BAUD_RATE);
  myprint(3,9, str);
  myprint(1,4,aStart);
  USART_transmit('-');
  myprint(1,4,aFinal);
  USART_transmit(']');
  USART_transmit(' ');
  myprint(1,4,hexCrc);
  USART_transmit(' ');
  USART_transmit(0x0d);
  USART_transmit(0x0a);
  
  
  
  
  
  
  while(1)
  {
    
  }
  
}