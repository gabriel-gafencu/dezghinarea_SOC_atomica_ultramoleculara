#include "usart.h"

void USART_init(int baud_rate)
{
  UBRRH = (baud_rate>>8); //setez baudrate-ul
  UBRRL = baud_rate;
  UCSRB |= (1<<RXEN) | (1<<TXEN); //enable receive & transmit
}

void USART_transmit(unsigned char c)
{
  while( !(UCSRA & (1<<UDRE)) )
    ;
  UDR = c;
}

unsigned char USART_receive(void)
{
  while( !(UCSRA & (1<<RXC)) )
    ;
  return UDR;
}

void USART_transmit_uint(unsigned int n)
{
  unsigned char aux[16];
  int index = 0;
  while(n)
  {
    aux[index++] = n % 10 + '0';
    n /= 10; 
  }
  index--;
  while(index >= 0)
  {
    USART_transmit(aux[index--]);
  }
}

void USART_transmit_ulonglong(unsigned long long n)
{
  unsigned char aux[20];
  int index = 0;
  while(n)
  {
    aux[index++] = n % 10 + '0';
    n /= 10; 
  }
  index--;
  while(index >= 0)
  {
    USART_transmit(aux[index--]);
  }
}

void hexadecimalTransmit(unsigned int numarDecimal)
{
  unsigned int numarCaractere = 0;
  unsigned int auxaux = numarDecimal;
  while(auxaux != 0)
  {
    numarCaractere++;
    auxaux /= 10;
  } 
  
  long long x = numarDecimal;
  int index=0;
  USART_transmit('0');
  USART_transmit('x');
  unsigned char aux[7];
  while(x!=0)
  {
    aux[index]=x&0x0F;
    if(aux[index]<=9)
    {
      aux[index]+='0';
    }
    else
    {
      aux[index]=aux[index]+'A'-10;
    }
    index++;
    x>>=4;
  }
  while(numarCaractere>0 && index>0)
  {
    USART_transmit(aux[index-1]);
    index--;
    numarCaractere--;
  }
}

unsigned int USART_receive_hexa_adress() // format : 0x????
{
  unsigned int myNo = 0;
  unsigned char recC = USART_receive(); // 0
  recC = USART_receive();               // x
  
  unsigned int i;
  for(i = 0; i < 4; i++)
  {
    recC = USART_receive();
    if(recC <= '9')
      myNo = myNo * 16 + (recC - '0');
    if(recC >= 'A')
      myNo = myNo * 16 + (recC - 'A' + 10);
  }
  return myNo;
}