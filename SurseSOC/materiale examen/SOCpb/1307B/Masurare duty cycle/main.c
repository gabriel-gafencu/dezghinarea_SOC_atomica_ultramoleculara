/*#include <inavr.h>
#include <iom16.h>

unsigned long nr_us_0;
unsigned long nr_us_1;

unsigned long nr_us;
unsigned int nr_cicli_1;
unsigned int nr_cicli;
unsigned int nr_cicli_0;*/
//mãsurare perioada fãrã întrerupere
/*void main( void )
{
  TCCR1A=0;//ini.ializare timer mod normal si dezactivare clock
  TCCR1B=0;
  
  DDRD&=~(1<<PD2);//pinul PD2 este de input
  while((PIND & (1<<PD2))==0);//a.teptãm frontul pozitiv
  //a apãrut frontul pozitiv
  TCNT1=0;
  TCCR1B|=(1<<CS12);//pornire clock timer cu prescaler de 256
  
  while((PIND & (1<<PD2))==4);//a.teptãm frontul negativ
 // nr_cicli_1=TCNT1;//salvãm numãrul de ciclii
  //a apãrut frontul negativ
  while((PIND & (1<<PD2))==0);//a.teptãm frontul pozitiv pentru a completa perioada
 // nr_cicli_0=TCNT1-nr_cicli_1;//salvãm numãrul de ciclii
    nr_cicli_1=TCNT1;
  //calculam perioada in microsecunde
 // nr_us_0=nr_cicli_0*64;
  nr_us_1=nr_cicli_1*64;
  //nr_us=nrciclii* (perioada unui ciclu, adicã 0.25 microsecunde) * prescaler
  //aici se poate trimite pe serialã perioada
  return;
}*/
/*
void main( void )
{
  ///USART_initialize(BAUD_RATE);
  TCCR1A=0;//ini.ializare timer mod normal .i dezactivare clock
  TCCR1B=0;
  DDRD&=~(1<<PD2);//pinul PD2 este de input
  while((PIND & (1<<PD2))==0);//a.teptam frontul pozitiv
  
  //a apãrut frontul pozitiv
  TCCR1B|=(1<<CS12);//pornire clock timer cu prescaler de 256
  while((PIND & (1<<PD2))==4);//a.teptam frontul negativ; este vorba despre pinul PD2
  //a apãrut frontul negativ
  nr_cicli=TCNT1;//salvãm numãrul de ciclii
  //calculam perioada în microsecunde
  nr_us=nr_cicli*64; //nr_us=nrciclii* (perioada unui ciclu, adicã 0.25 microsecunde) * prescaler
  //aici se poate trimite pe serialã perioada.
 
 
  
  return;
}*/




#include<inavr.h>
#include<intrinsics.h>
#include<iom16.h>
#include "usart.h"
#include "ir_printf.h"
long cnt,cicli1,cicli0,frecv1,frecv2,cnt1;
#pragma vector = TIMER0_OVF_vect
__interrupt void timer0_ovf_int()
{
 cnt++;
}



void main(void)
{
  char value ;
  __enable_interrupt();
  USART_Init(MYUBRR);
  DDRB=0;//<<PB2);
  TIMSK |= (1<<TOIE0);// intrerupere overflow
  TCCR0 = 0; // mod normal , fara prescaler , oc0 deconectat
  while(1)
  {
    value = (PINB & (1<<PB2));
    while( (PINB & (1<<PB2)) == value )
    {
      ;
    }
    value^= 1<<PB2;
    TCCR0 |= (1<<CS00);
    while( (PINB & (1<<PB2)) == value )
    {
      ;
    }
   
    cicli1 = TCNT0;
    cnt1=cnt;
    TCNT0 = 0;
    cnt=0;
   
    value^= 1<<PB2;
    while( (PINB & (1<<PB2)) == value )
    {
      ;
    }
   
   
    cicli0 = TCNT0;
    cicli1 = (cnt1*0xFF+cicli1);
    cicli0 = (cnt*0xFF+cicli0);
    
    if(!((value&1<<PB2)==1<<PB2))
    {
      cicli1^=cicli0;
      cicli0^=cicli1;
      cicli1^=cicli0;
   }
    TCCR0 = 0;
    USART_Transmit('1');
    USART_Transmit(':');
    PRINTF((void*)(&cicli1),LONG);
    USART_Transmit('\r');
    USART_Transmit('0');
    USART_Transmit(':');
    PRINTF((void*)(&cicli0),LONG);
    USART_Transmit('\r');
    cnt=0;
    
  }
}





