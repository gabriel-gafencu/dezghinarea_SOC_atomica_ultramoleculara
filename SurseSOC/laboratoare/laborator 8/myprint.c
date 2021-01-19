#include "usart.h"

//transmiterea unui întreg pe serialã
void integerTransmit (unsigned int p1, unsigned int p2, void * p3)
{
  int index=0;
  char aux[5];
  int x=*((int *)(p3));
  if(x<0)
  {
    USART_transmit('-');
    x*=(-1);
  }
  while(x!=0)
  {
    aux[index]=x%10+'0';
    index++;
    x=x/10;
  }
  while(p2>0 )
  {
    USART_transmit(aux[index-1]);
    index--;
    p2--;
  }
}

//transmiterea unui numãr hexazecimal pe serialã
void hexadecimalTransmit (unsigned int p1, unsigned int p2, void * p3)
{
  long long x=*((long long *)(p3));
  int index=0;
  char aux[5];
  USART_transmit('0');
  USART_transmit('x');
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
  while(p2>0 && index>0)
  {
    USART_transmit(aux[index-1]);
    index--;
    p2--;
  }
}

//transmiterea unui numãr de tip double pe serialã
void doubleTransmit(unsigned int p1, unsigned int p2, void * p3)
{
  int index=0;
  char aux[5];
  int dataIntreg;
  double x=*((double *)(p3));
  if(x<0)
  {
    USART_transmit('-');
    x*=(-1);
  }
  dataIntreg=(int)x;
  while((int)dataIntreg!=0)
  {
    aux[index]=(int)dataIntreg%10+'0';
    index++;
    dataIntreg=dataIntreg/10;
  }
  while(index>0 )
  {
    USART_transmit(aux[index-1]);
    index--;
  }
  dataIntreg=(int)x;
  x=x-dataIntreg;
  if(x>0)
  {
    USART_transmit('.');
  }
  while(x>0 && p2>0)
  {
    x=x*10;
    dataIntreg=(int)x;
    USART_transmit(dataIntreg+'0');
    x=x-dataIntreg;
    p2--;
  }
}

//transmiterea unui numãr de tip float
void floatTransmit(unsigned int p1, unsigned int p2, void * p3)
{
  char buff[30]={0};
  int j, nrCaract;
  long long nr;
  char nrNeg;
  int i;
  float floatNr;
  floatNr=*((float *)p3);
  nrCaract=0;
  nr=(long long)floatNr;
  nrNeg=(nr<0);
  if (nrNeg)
    nr*=-1;
  while(nr!=0)
  {
    j=nr%10;
    nr=nr/10;
    buff[nrCaract]=j+'0';
    nrCaract++;
  }
  if(nrCaract ==0)
  {
    buff[0]='0';
    nrCaract=1;
  }
  if(nrNeg)
  {
    buff[nrCaract]='-';
    nrCaract++;
  }
  for(i=nrCaract-1; i>=0; i--)
  {
    USART_transmit(buff[i]);
  }
  USART_transmit('.');
  if (floatNr<0)
    floatNr*=-1;
  for (i=0; i<p2; i++)
  {
    floatNr=floatNr-(long long)floatNr;
    floatNr*=10;
    nr=(int)floatNr;
    USART_transmit(nr+'0');
  }
}

//transmiterea unui sir de caractere pe serialã
void characterTransmit (unsigned int p1, unsigned int p2, void * p3)
{
  unsigned char *x=(unsigned char *)(p3);
  int index=0;
  while(index!=p2)
  {
    USART_transmit(x[index]);
    index++;
  }
}

void myprint(unsigned int tip, unsigned int nr_car, void * val)
{
  switch(tip)
  {
  case 0:
    integerTransmit(tip, nr_car, val);
    break;
  case 1:
    hexadecimalTransmit(tip, nr_car, val);
    break;
  case 2:
    doubleTransmit(tip, nr_car, val);
    break;
  case 3:
    characterTransmit(tip, nr_car, val);
    break;
  }
}