#include "usart.h"
typedef unsigned char uint8_t;
extern char aux[20];

void USART_initialize(unsigned short int baud_rate)
{
  /* seteazã baud rate */
  UBRRH = (unsigned char)(baud_rate >> 8);
  UBRRL = (unsigned char)(baud_rate & 0xFF);
  UCSRB = (1 << RXEN) | (1 << TXEN); /* activeazã transmisia .i recep.ia la ie.ire */
  /* seteazã pinul TXD: ie.ire */
  DDRD |= (1 << PD1);
  /* seteazã pinul RXD: intrare */
  DDRD &= ~(1 << PD0);
  /* activeazã întreruperea */
  //UCSRB |= (1 << RXCIE);
  /* Set frame format: 8data, 1stop bit */
  UCSRC = (1<<URSEL)|(0<<USBS)|(3<<UCSZ0);
 }

void USART_transmit(unsigned char data)
{
  /* a.teaptã pânã ce se terminã de transmis toate datele .i dupã trece la urmãtoarele informa.ii */
  while (!( UCSRA & (1 << UDRE)))
  {
  ;
  } 
  UDR = data;
}

unsigned char USART_Receive( void )
{
  /* A.teaptã recep.ionarea datelor */
  while ( !(UCSRA & (1<<RXC)) )
  {
  ; 
  }
  /* Preia .i returneazã datele recep.ionate din buffer */
  return UDR;
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
      floatTransmit(tip, nr_car, val);
      break;      
    case 4:
      characterTransmit(tip, nr_car, val);
      
    case 5:
      longTransmit(tip, nr_car, val);
      break;
    break;
  }  
}

//transmiterea unui întreg pe serialã
void integerTransmit (unsigned int p1, unsigned int p2, void * p3)
{
  int index=0;
  //char aux[5];
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
  while(p2 > 0 && index > 0)
  {
    USART_transmit(aux[index-1]);
    index--;
    p2--;
  }
}  

void longTransmit (unsigned int p1, unsigned int p2, void * p3)
{
  int index=0;
  //char aux[5];
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
  while(p2 > 0 && index > 0)
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
  USART_transmit('0');
  USART_transmit('x');
  //char aux[5];
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
  int index=0;
  while(index!=p2)
  {
    USART_transmit(x[index]);
    index++;
  }
}
