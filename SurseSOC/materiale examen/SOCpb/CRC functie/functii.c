#include "functii.h"

unsigned int crc16(unsigned int polinom16, unsigned int init_val_16,
                   unsigned int adr_start,unsigned int len, int ord)
{
  //rezultatul final
  unsigned int crc = init_val_16;
  //reþine în octetul cel mai semnificativ datele
  //extrase din memoria flash
  unsigned int data = 0;
  while( len-- ) {
    unsigned int i;
    //se extrage valoarea unui octet de la adresa de start
    //din memoria flash
    data = *(__flash char *)adr_start;
    if ( ord == MSBF ) //opþiunea cu shiftare spre MSB
    {
      //octetul este shiftat la stânga pentru a se alinia
      //cu polinomul generator
      data <<= 8;
      //datele sunt "transferate" în rezultat
      crc ^= data;
      adr_start++;
      //pentru biþii de date se face XOR cu polinomul generator,
      //daca bitul cel mai semnificativ este 1 sau se shifteaza datele
      //la stânga, dacã bitul cel mai semnificativ este 0
      for( i = 0; i < 8; ++i ) {
        //se verificã dacã bitul cel mai semnificativ este 1
        if( crc & 0x8000 )
          crc = (crc << 1) ^ polinom16;
        else
          crc = crc << 1;
      }
    }
    else
      //opþiunea cu shiftare spre LSB
      //parametrul polinom16 al funcþiei trebuie sã aibã biþii inversaþi în
      //prealabil
    {
      crc ^= data;
      adr_start++;
      //se verificã bitul cel mai putin semnificativ .i dacã
      //acesta este 1 se face XOR cu polinomul generator,
      // altfel datele sunt shiftate la dreapta
      for( i = 0; i < 8; ++i ) {
        //se verificã daca cel mai puþin semnificativ bit este 1
        if( crc & 0x0001 )
          crc = (crc >> 1) ^ polinom16;
        else
          crc = crc >> 1;
      }
    }
  }
  return crc;
}

unsigned char min(unsigned char x, unsigned char y)
{
  if(x>y)
  {
   
    return y;
  }
  else 
    return x;
}


unsigned int lungimeFunctie(unsigned int adr)
{
  unsigned int nrOcteti=0;
  unsigned int data;
  unsigned int realAdr=adr<<1;
  
  
  do{
    data=*(__flash unsigned int*)realAdr;
    realAdr+=2;
    nrOcteti++;
  }while(data!=0X9508);
  return nrOcteti<<1;
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
  long x=*(( long *)(p3));
  char aux[10];
  int index=0;
  int p22=p2;
  USART_transmit('0');
  USART_transmit('x');
  while(x!=0 && p22)
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
    p22--;
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
  int dataIntreg;
  char aux[10];
  double x=*((double *)(p3));
  if(x<0)
  {USART_transmit('-');
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
//transmiterea unui numar de tip float
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
//transmiterea unui .ir de caractere pe seriala
void characterTransmit (unsigned int p1, unsigned int p2, void * p3)
{
  unsigned char *x=(unsigned char *)(p3);
  int index=p2;
  while(index!=0)
  {
    USART_transmit(x[p2-index]);
    index--;
  }
}