#include <iom16.h>
#include <inavr.h>
#include "usart.h"

__no_init long long int NR;
__no_init long long int var;

void timer1_INIT()
{
/*
Modul Normal: WGM13:10=0000;
Fãrã prescaler: CS12:10=001;
*/
TCCR1B|=(1<<CS10);
TIMSK|=(1<<TOIE1);
}


#pragma vector = TIMER1_OVF_vect
__interrupt void T1_OVF()
{
    NR++;
}

unsigned  char vect[10];
float T; 
unsigned long parteIntreaga, parteFractionara;
void main( void )
{
  USART_initialize(BAUD_RATE);
  int j=9;
  
  __enable_interrupt();
  timer1_INIT();
  if((MCUCSR>>EXTRF)&1)
  {
    T=(NR*65535 + var)*0.00025; //ms
    parteIntreaga=(unsigned long)T;
    parteFractionara=(T-parteIntreaga)*100;
    
     
    vect[j--]=parteFractionara%10+'0';
    parteFractionara/=10;
    vect[j--]=parteFractionara%10+'0';
    vect[j--]='.';
   
    
    if(parteIntreaga<9)
    {
      vect[j--]=parteIntreaga%10+'0';
      vect[j]='0';
    }
    else
    {
      
      while(parteIntreaga!=0)
      {  
        vect[j]=parteIntreaga%10+'0';
        parteIntreaga/=10;
        j--;
      }
    }
    
    for(j=j+1;j<10;++j)
    {
     USART_transmit(vect[j]); 
    }
    USART_transmit('m');
    USART_transmit('s');
    USART_transmit(' ');
    NR=0;
           
  }
  
  
  
  while(1)
  {
   var=TCNT1; 
    
  }
}
