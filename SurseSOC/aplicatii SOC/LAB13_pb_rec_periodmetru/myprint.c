#include "usart.h"
#include "myprint.h"

// transmiterea unui intreg cu semn pe seriala
void integerTransmit (unsigned int nr_car, void * val)
{
  int index=0;
  char aux[5];
  int x = *((int *)(val));
  if(x < 0)
  {
    USART_transmit('-');
    x*=(-1);
  }
  while(x)
  {
    aux[index] = x%10 + '0';
    index++;
    x = x/10;
  }
  while(nr_car)
  {
    USART_transmit(aux[index-1]);
    index--;
    nr_car--;
  }
}

// transmiterea unui intreg fara semn pe seriala
void unsignedIntegerTransmit (unsigned int nr_car, void * val)
{
  int index=0;
  char aux[5];
  unsigned int x = *((unsigned int *)(val));
  while(x)
  {
    aux[index] = x%10 + '0';
    index++;
    x = x/10;
  }
  while(nr_car)
  {
    USART_transmit(aux[index-1]);
    index--;
    nr_car--;
  }
}

// transmiterea unui numar hexazecimal pe seriala
void hexadecimalTransmit (unsigned int nr_car, void * val)
{
  char aux[10];
  long long x = *((long long *)(val));
  int index=0;
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
    x >>= 4;
  }
  while(nr_car>0 && index>0)
  {
    USART_transmit(aux[index-1]);
    index--;
    nr_car--;
  }
}
// transmiterea unui numar de tip double pe seriala
void doubleTransmit(unsigned int nr_car, void * val)
{
  char aux[10];
  int index = 0;
  int dataIntreg;
  double x=*((double *)(val));
  if(x<0)
  {
    USART_transmit('-');
    x*=(-1);
  }
  dataIntreg=(int)x;
  while((int)dataIntreg!=0)
  {
    aux[index]=(int)dataIntreg % 10 + '0';
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
  while(x>0 && nr_car>0)
  {
    x=x*10;
    dataIntreg=(int)x;
    USART_transmit(dataIntreg+'0');
    x=x-dataIntreg;
    nr_car--;
  }
}
// transmiterea unui numar de tip float
void floatTransmit(unsigned int nr_car, void * val)
{
  char buff[30]={0};
  int j, nr_caract;
  long long nr;
  char nrNeg;
  int i;
  float floatNr;
  floatNr=*((float *)val);
  nr_caract=0;
  nr=(long long)floatNr;
  nrNeg=(nr<0);
  if (nrNeg)
    nr*=-1;
  while(nr!=0)
  {
    j=nr%10;
    nr=nr/10;
    buff[nr_caract]=j+'0';
    nr_caract++;
  }
  if(nr_caract ==0)
  {
    buff[0]='0';
    nr_caract=1;
  }
  if(nrNeg)
  {
    buff[nr_caract]='-';
    nr_caract++;
  }
  for(i=nr_caract-1; i>=0; i--)
  {
    USART_transmit(buff[i]);
  }
  USART_transmit('.');
  if (floatNr<0)
    floatNr*=-1;
  for (i=0; i<nr_car; i++)
  {
    floatNr=floatNr-(long long)floatNr;
    floatNr*=10;
    nr=(int)floatNr;
    USART_transmit(nr+'0');
  }
}

// transmiterea unui sir de caractere pe seriala
void characterTransmit (unsigned int nr_car, void * val)
{
  unsigned char *x=(unsigned char *)(val);
  int index = nr_car;
  while(index)
  {
    USART_transmit(x[index]);
    index--;
  }
}

void myprint(unsigned int tip, unsigned int nr_car, void * val)
{
  switch(tip)
  {
  case 0:
    integerTransmit(nr_car, val);
    break;
  case 1:
    hexadecimalTransmit(nr_car, val);
    break;
  case 2:
    doubleTransmit(nr_car, val);
    break;
  case 3:
    characterTransmit(nr_car, val);
    break;
  case 4:
    unsignedIntegerTransmit(nr_car,val);
    break;
  }
}