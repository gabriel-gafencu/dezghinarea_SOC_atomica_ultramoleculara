#include <inavr.h>
#include <iom16.h>
#include "usart.h"
unsigned long nr_us;
unsigned int nr_cicli;
unsigned char buffer[10];
int index;
unsigned int per;
unsigned int nr_cicli;
unsigned int flag_per;

#pragma vector=INT0_vect
__interrupt void isr_INT0(void){
  if((PIND&(1<<PD2))==4){
    TCNT1=0;///reset timer
    TCCR1B=(1<<CS12);//start timer cu prescaler de 256
  }
  else{
    nr_cicli=TCNT1;//salveazã nr cicli
    per=(nr_cicli/4)*256;//calculeazã perioada în microsecunde
    flag_per=1;
  }
}


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


int nrCifre(unsigned long num)
{
  int nrC=0;
  while(num)
  {
    num/=10;
    nrC++; 
  }
  return nrC;
}
//mãsurare impuls fãrã întrerupere

/*
void main( void )
{
USART_initialize(BAUD_RATE);
TCCR1A=0;//ini.ializare timer mod normal .i dezactivare clock
TCCR1B=0;
index=0;
DDRD&=~(1<<PD2);//pinul PD2 este de input

while(1)
{
while((PIND & (1<<PD2))==0);//a.teptam frontul pozitiv
//a apãrut frontul pozitiv
TCCR1B|=(1<<CS12);//pornire clock timer cu prescaler de 256
while((PIND & (1<<PD2))==4);//a.teptam frontul negativ
while((PIND & (1<<PD2))==0);//a.teptam frontul pozitiv
//a apãrut frontul negativ
nr_cicli=TCNT1;//salvãm numãrul de ciclii
//calculam perioada în microsecunde
nr_us=nr_cicli/4*256; //nr_us=nrciclii* (perioada unui ciclu, adicã 0.25 microsecunde) * prescaler
//aici se poate trimite pe serialã perioada.

unsigned long* val=&nr_us;
integerTransmit(0,nrCifre(nr_us),val);
USART_transmit(0xd);
USART_transmit(0xa);


TCCR1A=0;//ini.ializare timer mod normal .i dezactivare clock
TCCR1B=0;
TCNT1=0;

  }



}*/





void main( void )
{
  USART_initialize(BAUD_RATE);
  __enable_interrupt();
  unsigned int x;
  // ini.ializare întrerupere externã
  MCUCR |=(1<<ISC00);
  MCUCR &=~(1<<ISC01);
  GICR |=(1<<INT0); //activare întrerupere externã
  
  while(1){
    if(flag_per==1)//a.teapta masurare întrerupere
    USART_transmit('*');    
    flag_per=0;//reseteaza flag
    x=per;//se face ceva cu valoarea masurata
   
  }
}